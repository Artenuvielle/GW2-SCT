#include "ScrollArea.h"
#include "imgui.h"
#include "Common.h"
#include "Options.h"

using namespace std::chrono;

GW2_SCT::ScrollArea::ScrollArea(std::shared_ptr<scroll_area_options_struct> options) : options(options) {
	paintedMessages = std::list<std::pair<MessagePrerender, time_point<system_clock>>>();
}

void GW2_SCT::ScrollArea::receiveMessage(std::shared_ptr<EventMessage> m) {
	for (auto& receiver : options->receivers) {
		if (receiver->enabled && m->getCategory() == receiver->category && m->getType() == receiver->type) {
			std::unique_lock<std::mutex> mlock(messageQueueMutex);
			if (!messageQueue.empty()) {
				if (Options::get()->combineAllMessages) {
					for (auto it = messageQueue.rbegin(); it != messageQueue.rend(); ++it) {
						if (it->options == receiver && it->message->tryToCombineWith(m)) {
							it->update();
							mlock.unlock();
							return;
						}
					}
				} else {
					auto backMessage = messageQueue.rend();
					if (backMessage->options == receiver && backMessage->message->tryToCombineWith(m)) {
						backMessage->update();
						mlock.unlock();
						return;
					}
				}
			}
			MessagePrerender preMessage = MessagePrerender(m, receiver);
			if (preMessage.options != nullptr)
				messageQueue.push_back(std::move(preMessage));
			mlock.unlock();
			return;
		}
	}
}

void GW2_SCT::ScrollArea::paintOutline() {
	if (options->outlineState != ScrollAreaOutlineState::NONE) {
		FLOAT x = windowWidth * 0.5f + options->offsetX;
		FLOAT y = windowHeight * 0.5f + options->offsetY;
		FLOAT w = options->width;
		FLOAT h = options->height;

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		if (options->outlineState == ScrollAreaOutlineState::FULL) {
			draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::GetColorU32(ImVec4(.15f, .15f, .15f, .66f)));
			draw_list->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, .66f)));
		} else {
			draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::GetColorU32(ImVec4(.15f, .15f, .15f, .33f)));
			draw_list->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, .33f)));
		}
	}
}

void GW2_SCT::ScrollArea::paint() {

	std::unique_lock<std::mutex> mlock(messageQueueMutex);
	if (!messageQueue.empty()) {
		MessagePrerender& m = messageQueue.front();
		time_point<system_clock> now = system_clock::now();
		if (paintedMessages.empty()) {
			paintedMessages.push_back(std::pair<MessagePrerender, time_point<system_clock>>(m, now));
			messageQueue.pop_front();
		} else {
			float spaceRequiredForNextMessage = getTextSize(m.str.c_str(), m.font, m.fontSize).y;
			float msForSpaceToClear = spaceRequiredForNextMessage / Options::get()->scrollSpeed * 1000;
			__int64 msSinceLastPaintedMessage = duration_cast<milliseconds>(now - paintedMessages.back().second).count();
			__int64 msUntilNextMessageCanBePainted = (__int64)msForSpaceToClear - msSinceLastPaintedMessage;
			if (msUntilNextMessageCanBePainted > 0) {
				if (messageQueue.size() > Options::get()->messagesInStack) {
					for (auto it = paintedMessages.rbegin(); it != paintedMessages.rend(); ++it) {
						it->second = it->second - milliseconds(msUntilNextMessageCanBePainted);
					}
					paintedMessages.push_back(std::pair<MessagePrerender, time_point<system_clock>>(m, now));
					messageQueue.pop_front();
				}
			} else {
				paintedMessages.push_back(std::pair<MessagePrerender, time_point<system_clock>>(m, now));
				messageQueue.pop_front();
			}
		}
	}
	mlock.unlock();

	paintOutline();

	auto it = paintedMessages.begin();
	while(it != paintedMessages.end()) {
		__int64 t = duration_cast<milliseconds>(system_clock::now() - it->second).count();
		if (paintMessage(it->first, t)) {
			it++;
		} else {
			it = paintedMessages.erase(it);
		}
	}
}

bool GW2_SCT::ScrollArea::paintMessage(MessagePrerender& m, __int64 time) {
	float animatedHeight = time * 0.001f * Options::get()->scrollSpeed;
	float alpha = 1;
	float percentage = animatedHeight / options->height;
	float fadeLength = 0.2f;
	if (percentage > 1.f) {
		return false;
	}
	else if (percentage > 1.f - fadeLength) {
		alpha = 1 - (percentage - 1.f + fadeLength) / fadeLength;
	}

	if (m.prerenderNeeded) m.prerenderText();

	ImVec2 pos(windowWidth * 0.5f + options->offsetX, windowHeight * 0.5f + options->offsetY);

	switch (options->textCurve)
	{
	case GW2_SCT::TextCurve::LEFT:
		pos.x += options->width * (2 * percentage - 1) * (2 * percentage - 1);
		pos.y += animatedHeight;
		break;
	case GW2_SCT::TextCurve::RIGHT:
		pos.x += options->width * (1 - (2 * percentage - 1) * (2 * percentage - 1));
		pos.y += animatedHeight;
		break;
	case GW2_SCT::TextCurve::STRAIGHT:
		pos.y += animatedHeight;
		break;
	case GW2_SCT::TextCurve::STATIC:
		break;
	}

	if (options->textAlign != TextAlign::LEFT) {
		switch (options->textAlign)
		{
		case GW2_SCT::TextAlign::CENTER:
			pos.x -= 0.5f * m.interpretedTextWidth;
			break;
		case GW2_SCT::TextAlign::RIGHT:
			pos.x -= m.interpretedTextWidth;
			break;
		}
	}

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	for (TemplateInterpreter::InterpretedText text : m.interpretedText) {
		ImVec2 curPos = ImVec2(pos.x + text.offset.x, pos.y += text.offset.y);
		if (text.icon == nullptr) {
			ImU32 blackWithAlpha = ImGui::GetColorU32(ImVec4(0, 0, 0, alpha));
			if (GW2_SCT::Options::get()->dropShadow) {
				m.font->drawAtSize(text.str, m.fontSize, ImVec2(curPos.x + 2, curPos.y + 2), blackWithAlpha);
			}
			ImU32 actualCol = text.color & ImGui::GetColorU32(ImVec4(1, 1, 1, alpha));
			m.font->drawAtSize(text.str, m.fontSize, curPos, actualCol);
		}
		else {
			text.icon->draw(curPos, text.size, ImGui::GetColorU32(ImVec4(1, 1, 1, alpha)));
		}
	}

	return true;
}

GW2_SCT::ScrollArea::MessagePrerender::MessagePrerender(std::shared_ptr<EventMessage> message, std::shared_ptr<message_receiver_options_struct> options)
	: message(message), options(options) {
	category = message->getCategory();
	type = message->getType();
	update();
	templateObserverId = options->outputTemplate.onAssign([this](const std::string& oldVal, const std::string& newVal) { this->update(); });
	colorObserverId = options->color.onAssign([this](const std::string& oldVal, const std::string& newVal) { this->update(); });
	fontObserverId = options->font.onAssign([this](const FontId& oldVal, const FontId& newVal) { this->update(); });
	fontSizeObserverId = options->fontSize.onAssign([this](const float& oldVal, const float& newVal) { this->update(); });
	// TODO: add callbacks when changing other values like general font size
}

GW2_SCT::ScrollArea::MessagePrerender::MessagePrerender(const MessagePrerender& copy) {
	message = copy.message;
	options = copy.options;
	category = copy.category;
	type = copy.type;
	if (copy.prerenderNeeded) {
		update();
	} else {
		str = copy.str;
		font = copy.font;
		fontSize = copy.fontSize;
		options = copy.options;
		interpretedText = std::vector<TemplateInterpreter::InterpretedText>(copy.interpretedText);
		interpretedTextWidth = copy.interpretedTextWidth;
		prerenderNeeded = copy.prerenderNeeded;
	}
	templateObserverId = options->outputTemplate.onAssign([this](const std::string& oldVal, const std::string& newVal) { this->update(); });
	colorObserverId = options->color.onAssign([this](const std::string& oldVal, const std::string& newVal) { this->update(); });
	fontObserverId = options->font.onAssign([this](const FontId& oldVal, const FontId& newVal) { this->update(); });
	fontSizeObserverId = options->fontSize.onAssign([this](const float& oldVal, const float& newVal) { this->update(); });
}

GW2_SCT::ScrollArea::MessagePrerender::~MessagePrerender() {
	options->outputTemplate.removeOnAssign(templateObserverId);
	options->color.removeOnAssign(colorObserverId);
	options->font.removeOnAssign(fontObserverId);
	options->fontSize.removeOnAssign(fontSizeObserverId);
}

void GW2_SCT::ScrollArea::MessagePrerender::update() {
	if (message.get() == nullptr) {
		LOG("ERROR: calling update on pre-render without message");
		str = "";
		prerenderNeeded = false;
		return;
	}
	str = message->getStringForOptions(options);
	font = getFontType(options->font);
	fontSize = options->fontSize;
	if (fontSize < 0) {
		if (floatEqual(fontSize, -1.f)) fontSize = GW2_SCT::Options::get()->defaultFontSize;
		else if (floatEqual(fontSize, -2.f)) fontSize = GW2_SCT::Options::get()->defaultCritFontSize;
	}
	font->bakeGlyphsAtSize(str, fontSize);
	prerenderNeeded = true;
}

void GW2_SCT::ScrollArea::MessagePrerender::prerenderText() {
	if (options != nullptr) {
		interpretedText = TemplateInterpreter::interpret(font, fontSize, stoc(options->color), str);
	} else {
		interpretedText = {};
	}
	if (interpretedText.size() > 0) {
		interpretedTextWidth = interpretedText.back().offset.x + interpretedText.back().size.x;
	}
	prerenderNeeded = false;
}
