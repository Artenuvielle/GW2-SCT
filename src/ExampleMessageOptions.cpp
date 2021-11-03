#include <cmath>
#include "ExampleMessageOptions.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_sct_widgets.h"
#include "Options.h"
#include "Language.h"

bool GW2_SCT::ExampleMessageOptions::windowIsOpen = false;
GW2_SCT::ExampleMessageOptions::State GW2_SCT::ExampleMessageOptions::state = GW2_SCT::ExampleMessageOptions::State::READY_TO_RECORD;
std::chrono::system_clock::time_point GW2_SCT::ExampleMessageOptions::recordingStart = std::chrono::system_clock::now();
GW2_SCT::SCTMain* GW2_SCT::ExampleMessageOptions::main = nullptr;
std::thread* GW2_SCT::ExampleMessageOptions::emitterThread = nullptr;

#ifdef _DEBUG
using namespace std::chrono_literals;
cbtevent1 minimalEvent(int32_t value, int32_t buff_dmg, uint32_t overstack_value, uint32_t skillid) {
	cbtevent1 ret;
	ret.value = value;
	ret.overstack_value = buff_dmg;
	ret.buff_dmg = overstack_value;
	ret.skillid = skillid;
	return ret;
}
ag agMe { "Me", 1, profession::PROFESSION_ELEMENTALIST, 0, 1 };
ag agFoe { "Foe", 2, profession::PROFESSION_ENGINEER, 0, 0 };
ag agFriend { "Friend", 3, profession::PROFESSION_GUARDIAN, 0, 0 };
ag agPet { "Pet", 4, profession::PROFESSION_UNDEFINED, 0, 0 };
#endif // _DEBUG


std::multimap<std::chrono::system_clock::duration, GW2_SCT::ExampleMessageOptions::MessageInformation> GW2_SCT::ExampleMessageOptions::messagesToEmmit = {
#ifdef _DEBUG
	{ std::chrono::system_clock::duration(0ms), { GW2_SCT::MessageCategory::PLAYER_OUT, GW2_SCT::MessageType::PHYSICAL, std::make_shared<GW2_SCT::MessageData>(-1234, 0, 0, 5489, &agMe, &agFoe, "Lightning Whip")}},
	{ std::chrono::system_clock::duration(100ms), { GW2_SCT::MessageCategory::PLAYER_OUT, GW2_SCT::MessageType::CRIT, std::make_shared<GW2_SCT::MessageData>(-1855, 0, 0, 5489, &agMe, &agFoe, "Lightning Whip")}},
	{ std::chrono::system_clock::duration(0ms), { GW2_SCT::MessageCategory::PLAYER_IN, GW2_SCT::MessageType::PHYSICAL, std::make_shared<GW2_SCT::MessageData>(-123, 0, 0, 5827, &agFoe, &agMe, "Fragmentation Shot")}},
	{ std::chrono::system_clock::duration(250ms), { GW2_SCT::MessageCategory::PLAYER_IN, GW2_SCT::MessageType::BLEEDING, std::make_shared<GW2_SCT::MessageData>(0, -789, 0, 5489, &agFoe, &agMe, "Fragmentation Shot")}},
	{ std::chrono::system_clock::duration(500ms), { GW2_SCT::MessageCategory::PLAYER_IN, GW2_SCT::MessageType::BLEEDING, std::make_shared<GW2_SCT::MessageData>(0, -789, 0, 5489, &agFoe, &agMe, "Fragmentation Shot")}},
	{ std::chrono::system_clock::duration(750ms), { GW2_SCT::MessageCategory::PLAYER_IN, GW2_SCT::MessageType::BLEEDING, std::make_shared<GW2_SCT::MessageData>(0, -456, 0, 5489, &agFoe, &agMe, "Fragmentation Shot")}},
	{ std::chrono::system_clock::duration(1000ms), { GW2_SCT::MessageCategory::PLAYER_IN, GW2_SCT::MessageType::BLEEDING, std::make_shared<GW2_SCT::MessageData>(0, -34, 0, 5489, &agFoe, &agMe, "Fragmentation Shot")}}
#endif // _DEBUG
};

void GW2_SCT::ExampleMessageOptions::open() {
	windowIsOpen = true;
}

void GW2_SCT::ExampleMessageOptions::paint() {
	if (windowIsOpen) {
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.60f, 0.60f, 0.60f, 0.30f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
		ImGui::Begin(langString(LanguageCategory::Example_Message_UI, LanguageKey::Title), &windowIsOpen, ImGuiWindowFlags_NoCollapse);

		const float square_size = ImGui::GetFontSize();
		ImGuiStyle style = ImGui::GetStyle();

		State initialState = state;

		if (initialState != State::READY_TO_RECORD) ImGui::BeginDisabled();
		if (ImGui::Button("Start recording messages")) {
			state = State::RECORDING;
			recordingStart = std::chrono::system_clock::now();
		}
		if (initialState != State::READY_TO_RECORD) ImGui::EndDisabled();

		ImGui::SameLine();
		
		if (initialState != State::RECORDING) ImGui::BeginDisabled();
		if (ImGui::Button("Stop recording messages")) {
			state = State::READY_TO_RECORD;
		}
		if (initialState != State::RECORDING) ImGui::EndDisabled();

		ImGui::BeginChild("messagesToEmitPane", ImVec2(ImGui::GetWindowWidth(), -ImGui::GetFrameHeightWithSpacing()), true);
		int i = 0;
		static MessageInformation* currentlyEditing = nullptr;
		for (auto messageToEmmit = messagesToEmmit.begin(); messageToEmmit != messagesToEmmit.end(); messageToEmmit++) {
			std::string s = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(messageToEmmit->first).count()) + "." + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(messageToEmmit->first).count() % 1000 / 10) + "s";
			s += " - " + categoryNames.at(messageToEmmit->second.category);
			s += " | " + typeNames.at(messageToEmmit->second.type) + " - ";
			if (messageToEmmit->second.data->entityName != nullptr) {
				s += " " + std::string(messageToEmmit->second.data->entityName);
			}
			if (messageToEmmit->second.data->otherEntityName != nullptr) {
				s += " -> " + std::string(messageToEmmit->second.data->otherEntityName);
			}
			if (messageToEmmit->second.data->skillName != nullptr) {
				s += " (" + std::string(messageToEmmit->second.data->skillName) + ")";
			}
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + style.FramePadding.y);
			ImGui::Text(s.c_str());
			ImGui::SameLineEnd(square_size + style.FramePadding.y + style.WindowPadding.x * 2);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - style.FramePadding.y);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.67f, 0.40f, 0.40f, 0.60f));
			if (ImGui::Button(ImGui::BuildLabel("-", "delete-example-message", i).c_str(), ImVec2(square_size + style.FramePadding.y * 2, square_size + style.FramePadding.y * 2))) {
				messageToEmmit = messagesToEmmit.erase(messageToEmmit);
			}
			ImGui::PopStyleColor();
			i++;
		}
		static int newExampleMessageTime = 0;
		ImGui::InputInt("after milliseconds", &newExampleMessageTime, 100, 1000);
		ImGui::SameLineEnd(square_size + style.FramePadding.y + style.WindowPadding.x * 2);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.67f, 0.40f, 0.40f, 0.60f));
		if (ImGui::Button(ImGui::BuildLabel("+", "add-example-message", i).c_str(), ImVec2(square_size + style.FramePadding.y * 2, square_size + style.FramePadding.y * 2))) {
			messagesToEmmit.insert(std::pair<std::chrono::system_clock::duration,MessageInformation>(
				std::chrono::system_clock::duration(newExampleMessageTime),
				{ MessageCategory::PLAYER_OUT, MessageType::PHYSICAL, std::make_shared<MessageData>() }
			));
		}
		ImGui::PopStyleColor();
		ImGui::EndChild();
		
		if (initialState == State::EMITTING) ImGui::BeginDisabled();
		if (ImGui::Button("Start emitting messages")) {
			state = State::EMITTING;
			emitterThread = new std::thread(ExampleMessageOptions::emitMessages);
		}
		if (initialState == State::EMITTING) ImGui::EndDisabled();

		ImGui::SameLine();

		if (initialState != State::EMITTING) ImGui::BeginDisabled();
		if (ImGui::Button("Stop emitting messages")) {
			state = State::READY_TO_RECORD;
			emitterThread->join();
			delete emitterThread;
		}
		if (initialState != State::EMITTING) ImGui::EndDisabled();

		ImGui::SameLine();
		
		bool messagesEmpty = messagesToEmmit.empty();
		if (messagesEmpty) ImGui::BeginDisabled();
		if (ImGui::Button("Clear all")) {
			messagesToEmmit.clear();
		}
		if (messagesEmpty) ImGui::EndDisabled();

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	} else if (state == State::RECORDING) {
		state = State::READY_TO_RECORD;
	}
}

void GW2_SCT::ExampleMessageOptions::receiveMessage(std::shared_ptr<EventMessage> m) {
	if (state == State::RECORDING) {
		auto messageDataCopy = m->getCopyOfFirstData();
		if (!messageDataCopy->hasToBeFiltered) {
			messagesToEmmit.insert(std::pair<std::chrono::system_clock::duration, MessageInformation>(std::chrono::system_clock::now() - recordingStart, { m->getCategory(), m->getType(), messageDataCopy }));
		}
	}
}

void GW2_SCT::ExampleMessageOptions::emitMessages() {
	auto start = std::chrono::system_clock::now();
	for (auto it = messagesToEmmit.begin(); state == State::EMITTING && it != messagesToEmmit.end(); it++) {
		while (it->first > std::chrono::system_clock::now() - start) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			if (state != State::EMITTING) return;
		}
		if (main != nullptr) {
			main->emitMessageToScrollAreas(std::make_shared<EventMessage>(it->second.category, it->second.type, it->second.data));
		}
	}
	state = State::READY_TO_RECORD;
}
