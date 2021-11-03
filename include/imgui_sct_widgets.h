#pragma once
#include "OptionsStructures.h"

enum ReceiverCollapsibleChangeFlags {
	ReceiverCollapsible_Remove = 1 << 0
};

enum FilterIDOptionLineChangeFlags {
	FilterOptionLine_Value = 1 << 0,
	FilterOptionLine_Remove = 1 << 1
};

namespace ImGui {
	inline std::string BuildLabel(std::string visibleLabelPart, std::string invisibleLabelPart, std::string indexString) {
		return visibleLabelPart + "##" + invisibleLabelPart + "-" + indexString;
	}
	inline std::string BuildLabel(const char* visibleLabelPart, std::string invisibleLabelPart, std::string indexString) {
		return BuildLabel(std::string(visibleLabelPart), invisibleLabelPart, indexString);
	}
	inline std::string BuildLabel(std::string visibleLabelPart, std::string invisibleLabelPart, int index) {
		return BuildLabel(visibleLabelPart, invisibleLabelPart, std::to_string(index));
	}
	inline std::string BuildLabel(const char* visibleLabelPart, std::string invisibleLabelPart, int index) {
		return BuildLabel(std::string(visibleLabelPart), invisibleLabelPart, index);
	}
	inline std::string BuildLabel(std::string invisibleLabelPart, std::string indexString) {
		return "##" + invisibleLabelPart + "-" + indexString;
	}
	inline std::string BuildLabel(std::string invisibleLabelPart, int index) {
		return BuildLabel(invisibleLabelPart, std::to_string(index));
	}
	inline std::string BuildVisibleLabel(std::string visibleLabelPart, std::string invisibleLabelPart) {
		return visibleLabelPart + "##" + invisibleLabelPart;
	}
	inline std::string BuildVisibleLabel(const char* visibleLabelPart, std::string invisibleLabelPart) {
		return BuildVisibleLabel(std::string(visibleLabelPart), invisibleLabelPart);
	}

	void HexColorEdit(const char* label, std::string* color);
	void SameLineEnd(float offset_from_end_x);
	int ReceiverCollapsible(int index, std::shared_ptr<GW2_SCT::message_receiver_options_struct> receiverOptions);
	bool NewReceiverLine(GW2_SCT::MessageCategory* categoryOut, GW2_SCT::MessageType* typeOut);
	int FilterOptionLine(uint32_t id, GW2_SCT::filter_options_struct* opt);
	bool ClampingDragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* display_format = "%.3f", float power = 1.0f);
	bool ClampingDragInt(const char* label, int* v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* display_format = "%.0f");

	void BeginDisabled();
	void EndDisabled();
}
