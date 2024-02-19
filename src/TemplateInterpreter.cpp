#include "TemplateInterpreter.h"
#include <string>
#include <vector>
#include <regex>
#include "Common.h"
#include "Options.h"

ImVec2 CalcTextSizeWithFontSize(const char* text, ImFont* font, float font_size)
{
	ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, 0.0f, text);

	// Cancel out character spacing for the last character of a line (it is baked into glyph->XAdvance field)
	const float font_scale = font_size / font->FontSize;
	const float character_spacing_x = 1.0f * font_scale;
	if (text_size.x > 0.0f)
		text_size.x -= character_spacing_x;
	text_size.x = (float)(int)(text_size.x + 0.95f);

	return text_size;
}

ImVec2 getTextSize(const char* t, GW2_SCT::FontType* font, float fontSize, bool use_bbc) {
	std::string text = std::string(t);
	std::string currentText = "";
	auto it = text.begin();

	if (use_bbc) {
		while (it != text.end()) {
			if (*it == '[') {
				++it;
				switch (*it) {
				case '[':
					currentText += *it;
					break;
				default:
					while (*it != ']') {
						++it;
					}
				}
			}
			else if (*it == ']') {
				currentText += *it;
				++it;
			}
			else {
				currentText += *it;
			}
			++it;
		}
	} else {
		currentText = text;
	}

	return font->calcRequiredSpaceForTextAtSize(currentText, fontSize);
}

bool GW2_SCT::TemplateInterpreter::validate(std::string t, std::map<char, std::string> params) {
	int openColors = 0;
	std::vector<std::string> currentCommands;
	std::string tempText;

	for (auto it = t.begin(); it != t.end(); it++) {
		switch (*it)
		{
		case ']':
			it++;
			if (it == t.end() || *it != ']') return false;
			break;
		case '[':
			it++;
			if (it == t.end()) return false;
			switch (*it)
			{
			case '[':
				break;
			case '/':
				it++;
				if (it == t.end()) return false;
				tempText = "";
				while (it != t.end() && *it != ']') {
					tempText += *it;
					it++;
				}
				if (it == t.end()) return false;
				if (currentCommands.empty()) return false;
				if (tempText.compare(currentCommands.back()) != 0) return false;
				currentCommands.pop_back();
				if (tempText == "col") {
					openColors--;
				}
				break;
			default:
				tempText = "";
				while (it != t.end() && *it != '=' && *it != ']') {
					tempText += *it;
					it++;
				}
				if (it == t.end()) return false;
				currentCommands.push_back(tempText);
				if (tempText == "col") {
					tempText = "";
					if (*it != '=') return false;
					it++;
					while (it != t.end() && *it != ']') {
						tempText += *it;
						it++;
					}
					if (it == t.end()) return false;
					if (std::regex_match(tempText, std::regex("(%c|[0-9A-F]{6})"))) {
						openColors++;
					}
					else {
						return false;
					}
				}
				else if (tempText == "icon") {
					tempText = "";
					if (*it != '=') return false;
					it++;
					while (it != t.end() && *it != ']') {
						tempText += *it;
						it++;
					}
					if (it == t.end()) return false;
					if (std::regex_match(tempText, std::regex("[0-9]+"))) {
						openColors++;
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
				break;
			}
			break;
		case '%':
			it++;
			if (it == t.end()) return false;
			if (*it != '%' && params.find(*it) == params.end()) {
				return false;
			}
			break;
		}
	}
	return openColors == 0;
}

const std::vector<GW2_SCT::TemplateInterpreter::InterpretedText> emptyInterpreted = {};

std::vector<GW2_SCT::TemplateInterpreter::InterpretedText> GW2_SCT::TemplateInterpreter::interpret(GW2_SCT::FontType* font, float fontSize, ImU32 defaultColor, std::string t) {
	std::vector<GW2_SCT::TemplateInterpreter::InterpretedText> interpreted;
	std::vector<std::string> currentCommands;
	std::vector<ImU32> colors{ defaultColor };
	std::string tempText;
	std::string currentText = "";
	ImVec2 currentOffset = ImVec2(0.f, 0.f);

	for (auto it = t.begin(); it != t.end(); it++) {
		switch (*it)
		{
		case ']':
			it++;
			if (it == t.end() || *it != ']') return emptyInterpreted;
			currentText += *it;
			break;
		case '[':
			it++;
			if (it == t.end()) return emptyInterpreted;
			switch (*it)
			{
			case '[':
				currentText += *it;
				break;
			case '/':
				it++;
				if (it == t.end()) return emptyInterpreted;
				tempText = "";
				while (it != t.end() && *it != ']') {
					tempText += *it;
					it++;
				}
				if (it == t.end()) return emptyInterpreted;
				if (currentCommands.empty()) return emptyInterpreted;
				if (tempText.compare(currentCommands.back()) != 0) return emptyInterpreted;
				currentCommands.pop_back();
				if (tempText == "col") {
					if (currentText != "") {
						ImVec2 s = getTextSize(currentText.c_str(), font, fontSize, false);
						interpreted.push_back({ currentText, s, currentOffset, colors.back() });
						currentOffset.x += s.x;
						currentText = "";
					}
					colors.pop_back();
				}
				else {
					currentText = "";
				}
				break;
			default:
				if (currentText != "") {
					ImVec2 s = getTextSize(currentText.c_str(), font, fontSize, false);
					interpreted.push_back({ currentText, s, currentOffset, colors.back() });
					currentOffset.x += s.x;
					currentText = "";
				}
				tempText = "";
				while (it != t.end() && *it != '=' && *it != ']') {
					tempText += *it;
					it++;
				}
				if (it == t.end()) return emptyInterpreted;
				currentCommands.push_back(tempText);
				if (tempText == "col") {
					tempText = "";
					if (*it != '=') return emptyInterpreted;
					it++;
					while (it != t.end() && *it != ']') {
						tempText += *it;
						it++;
					}
					if (it == t.end()) return emptyInterpreted;
					if (std::regex_match(tempText, std::regex("([0-9A-F]{6})"))) {
						colors.push_back(stoc(tempText));
					}
					else {
						return emptyInterpreted;
					}
				}
				else if (tempText == "icon") {
					tempText = "";
					if (*it != '=') return emptyInterpreted;
					it++;
					while (it != t.end() && *it != ']') {
						tempText += *it;
						it++;
					}
					if (it == t.end()) return emptyInterpreted;
					if (std::regex_match(tempText, std::regex("([0-9]+)"))) {
						SkillIcon* icon = SkillIconManager::getIcon(std::stoi(tempText.c_str()));
						if (icon != nullptr) {
							interpreted.push_back({ "", ImVec2(fontSize, fontSize), currentOffset, defaultColor, icon});
							currentOffset.x += fontSize;
						}
					}
					else {
						return emptyInterpreted;
					}
				}
				else {
					return emptyInterpreted;
				}
				break;
			}
			break;
		default:
			currentText += *it;
		}
	}
	if (currentText != "") {
		ImVec2 s = getTextSize(currentText.c_str(), font, fontSize, false);
		interpreted.push_back({ currentText, s, currentOffset, colors.back() });
		currentOffset.x += s.x;
		currentText = "";
	}
	return interpreted;
}
