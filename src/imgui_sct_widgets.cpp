#include "imgui_sct_widgets.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <imgui.h>
#include <imgui_stdlib.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include "Language.h"
#include "Common.h"
#include "TemplateInterpreter.h"
#include "Options.h"

constexpr const float& clampf(const float& v, const float& lo, const float& hi) {
	return (v < lo) ? lo : (hi < v) ? hi : v;
}
constexpr const int& clampi(const int& v, const int& lo, const int& hi) {
	return (v < lo) ? lo : (hi < v) ? hi : v;
}

bool ImGui::ClampingDragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* display_format, float power) {
	float start = *v;
	DragFloat(label, v, v_speed, v_min, v_max, display_format, power);
	*v = clampf(*v, v_min, v_max);
	return start != *v;
}

bool ImGui::ClampingDragInt(const char* label, int* v, float v_speed, int v_min, int v_max, const char* display_format) {
	int start = *v;
	DragInt(label, v, v_speed, v_min, v_max, display_format);
	*v = clampi(*v, v_min, v_max);
	return start != *v;
}

void ImGui::HexColorEdit(const char* label, std::string* color) {
	int num = std::stoi(*color, 0, 16);
	int red = num / 0x10000;
	int green = (num / 0x100) % 0x100;
	int blue = num % 0x100;
	float col[4] = { red / 255.f, green / 255.f, blue / 255.f, 1.0 };
	if (ImGui::ColorEdit3(label, col, ImGuiColorEditFlags_NoInputs)) {
		std::stringstream stm;
		stm << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(col[0] * 255.f);
		stm << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(col[1] * 255.f);
		stm << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(col[2] * 255.f);
		*color = stm.str();
	}
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Color:\n(%.2f,%.2f,%.2f)\n#%s", col[0], col[1], col[2], color->c_str());
}

void ImGui::SameLineEnd(float offset_from_end_x) {
	const ImGuiStyle& style = GImGui->Style;
	SameLine(GImGui->CurrentWindow->Size.x - offset_from_end_x - (GImGui->CurrentWindow->ScrollbarY ? style.ScrollbarSize : 0), 0);
}

int ImGui::ReceiverCollapsible(int index, std::shared_ptr<GW2_SCT::message_receiver_options_struct> receiverOptions) {
	int returnFlags = 0;
	const ImGuiStyle& style = GImGui->Style;
	const float square_size = GImGui->FontSize;

	bool isOpen = false;
	std::string indexString = std::to_string(index);
	std::string deleteModalLabel = BuildLabel(langString(GW2_SCT::LanguageCategory::Receiver_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Title), "receiver-delete-modal", indexString);
	if (TreeNodeEx(BuildLabel("receiver-collapsible", std::to_string(index)).c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_NoAutoOpenOnLog)) {
		isOpen = true;
		SameLine(style.FramePadding.x * 3 + GImGui->FontSize);
		Text(receiverOptions->name.c_str());
		SameLineEnd(2 * (square_size + style.FramePadding.y * 2) + style.ItemInnerSpacing.x);
		HexColorEdit(BuildLabel("receiver-color-picker", indexString).c_str(), &receiverOptions->color);
		SameLine(0, style.ItemInnerSpacing.x);
		PushStyleColor(ImGuiCol_Button, ImVec4(0.67f, 0.40f, 0.40f, 0.60f));
		if (Button(BuildLabel("-", "receiver-delete-button", indexString).c_str(), ImVec2(square_size + style.FramePadding.y * 2, square_size + style.FramePadding.y * 2))) {
			OpenPopup(deleteModalLabel.c_str());
		}
		PopStyleColor();

		InputText(BuildLabel(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Receiver_Name), "receiver-name", indexString).c_str(), &receiverOptions->name);
		
		if (BeginCombo(BuildLabel(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Category), "receiver-category-combo", indexString).c_str(), GW2_SCT::categoryNames.at(receiverOptions->category).c_str())) {
			int categoryIterator = 0;
			for (auto& categoryAndNamePair : GW2_SCT::categoryNames) {
				if (Selectable(BuildLabel(categoryAndNamePair.second, "receiver-category-combo", indexString + std::to_string(categoryIterator)).c_str(), receiverOptions->category == categoryAndNamePair.first)) {
					receiverOptions->category = categoryAndNamePair.first;
				}
				categoryIterator++;
			}
			EndCombo();
		}
		if (BeginCombo(BuildLabel(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Category), "receiver-type-combo", indexString).c_str(), GW2_SCT::typeNames.at(receiverOptions->type).c_str())) {
			int typeIterator = 0;
			for (auto& typeAndNamePair : GW2_SCT::typeNames) {
				if (Selectable(BuildLabel(typeAndNamePair.second, "receiver-type-combo", indexString + std::to_string(typeIterator)).c_str(), receiverOptions->type == typeAndNamePair.first)) {
					receiverOptions->type = typeAndNamePair.first;
				}
				typeIterator++;
			}
			EndCombo();
		}

		{
			std::string edit = receiverOptions->outputTemplate;
			struct UserData {
				bool changedBG = false;
				std::map<char, std::string> options;
			} ud;
			ud.options = GW2_SCT::receiverInformationPerCategoryAndType.at(receiverOptions->category).at(receiverOptions->type).options;
			if (InputText(BuildLabel(langString(GW2_SCT::LanguageCategory::Receiver_Option_UI, GW2_SCT::LanguageKey::Template), "receiver-template-input", indexString).c_str(), &edit, ImGuiInputTextFlags_CallbackAlways, [](ImGuiInputTextCallbackData* data) {
				UserData* d = static_cast<UserData*>(data->UserData);
				if (!GW2_SCT::TemplateInterpreter::validate(std::string(data->Buf), d->options)) { //validate here
					PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.f, 0.f, 0.f, .6f));
					d->changedBG = true;
				}
				return 0;
			}, &ud)) {
				if (!ud.changedBG) {
					receiverOptions->outputTemplate = edit;
				}
			}
			if (IsItemHovered() && ud.options.size() > 0) {
				std::stringstream stb;
				stb << langString(GW2_SCT::LanguageCategory::Receiver_Option_UI, GW2_SCT::LanguageKey::Available_Parameters) << ":";
				for (std::pair<char, std::string> i : ud.options) {
					stb << "\n%%" << i.first << "\t" << i.second;
				}
				BeginTooltip();
				Text(stb.str().c_str());
				EndTooltip();
			}
			if (ud.changedBG) {
				PopStyleColor();
			}

			Combo(langStringG(GW2_SCT::LanguageKey::Font), &receiverOptions->font, GW2_SCT::Options::getFontSelectionString().c_str());
			int selected = 2;
			if (receiverOptions->fontSize < 0) {
				if (floatEqual(receiverOptions->fontSize, -1.f)) selected = 0;
				else if (floatEqual(receiverOptions->fontSize, -2.f)) selected = 1;
			}
			if (Combo(langStringG(GW2_SCT::LanguageKey::Font_Size_Type), &selected, GW2_SCT::Options::getFontSizeTypeSelectionString().c_str())) {
				switch (selected)
				{
				case 0:
					receiverOptions->fontSize = -1.f;
					break;
				case 1:
					receiverOptions->fontSize = -2.f;
					break;
				case 2:
					receiverOptions->fontSize = receiverOptions->type == GW2_SCT::MessageType::CRIT ? GW2_SCT::Options::get()->defaultCritFontSize : GW2_SCT::Options::get()->defaultFontSize;
					break;
				}
			}
			if (selected == 2) {
				DragFloat(langStringG(GW2_SCT::LanguageKey::Font_Size), &receiverOptions->fontSize, 1.f, 5.f, 100.f);
			}
			else {
				float f = (selected == 0) ? GW2_SCT::Options::get()->defaultFontSize : GW2_SCT::Options::get()->defaultCritFontSize;
				PushStyleVar(ImGuiStyleVar_Alpha, style.Alpha * 0.5f);
				ClampingDragFloat(langStringG(GW2_SCT::LanguageKey::Font_Size), &f, 0.f, f, f);
				PopStyleVar();
			}
		}
		TreePop();
	}
	if (!isOpen) {
		SameLine(style.FramePadding.x * 3 + GImGui->FontSize);
		Text(receiverOptions->name.c_str());
		SameLineEnd(2 * (square_size + style.FramePadding.y * 2) + style.ItemInnerSpacing.x);
		HexColorEdit(BuildLabel("receiver-color-picker", indexString).c_str(), &receiverOptions->color);
		SameLine(0, style.ItemInnerSpacing.x);
		PushStyleColor(ImGuiCol_Button, ImVec4(0.67f, 0.40f, 0.40f, 0.60f));
		if (Button(BuildLabel("-", "receiver-delete-button", indexString).c_str(), ImVec2(square_size + style.FramePadding.y * 2, square_size + style.FramePadding.y * 2))) {
			OpenPopup(deleteModalLabel.c_str());
		}
		PopStyleColor();
	}

	if (ImGui::BeginPopupModal(deleteModalLabel.c_str())) {
		ImGui::Text(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Content));
		ImGui::Separator();
		if (ImGui::Button(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Confirmation), ImVec2(120, 0))) {
			returnFlags |= ReceiverCollapsible_Remove;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Cancel), ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	return returnFlags;
}

bool ImGui::NewReceiverLine(GW2_SCT::MessageCategory* categoryOut, GW2_SCT::MessageType* typeOut) {
	const ImGuiStyle& style = GImGui->Style;
	const float button_size = GImGui->FontSize + style.FramePadding.y * 2;
	const float item_width = (GImGui->CurrentWindow->Size.x - 1 * button_size - 2 * style.ItemInnerSpacing.x - (GImGui->CurrentWindow->ScrollbarY ? style.ScrollbarSize : 0)) / 2;
	PushItemWidth(item_width);

	if (BeginCombo("##new-receiver-category-select", GW2_SCT::categoryNames.at(*categoryOut).c_str())) {
		int categoryIterator = 0;
		for (auto& categoryAndNamePair : GW2_SCT::categoryNames) {
			if (Selectable(BuildLabel(categoryAndNamePair.second, "new-receiver-category-select", categoryIterator).c_str(), *categoryOut == categoryAndNamePair.first)) {
				*categoryOut = categoryAndNamePair.first;
			}
			categoryIterator++;
		}
		EndCombo();
	}
	SameLine();
	if (BeginCombo("##new-receiver-type-select", GW2_SCT::typeNames.at(*typeOut).c_str())) {
		int typeIterator = 0;
		for (auto& typeAndNamePair : GW2_SCT::typeNames) {
			if (Selectable(BuildLabel(typeAndNamePair.second, "new-receiver-type-select", typeIterator).c_str(), *typeOut == typeAndNamePair.first)) {
				*typeOut = typeAndNamePair.first;
			}
		}
		EndCombo();
	}
	PopItemWidth();
	SameLineEnd(button_size);
	bool ret = false;
	PushStyleColor(ImGuiCol_Button, ImVec4(0.67f, 0.40f, 0.40f, 0.60f));
	if (Button("+", ImVec2(button_size, button_size))) {
		ret = true;
	}
	PopStyleColor();
	SetCursorPosX(GetCursorPosX() + style.ItemInnerSpacing.x);
	Text(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Category));
	SameLine(item_width + style.ItemInnerSpacing.x);
	Text(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type));
	return ret;
}

int ImGui::FilterOptionLine(uint32_t i, GW2_SCT::filter_options_struct* opt) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;
	std::string iStr = std::to_string(i);
	std::string labelStr = "##filter-id-line-" + iStr;
	const char* label = labelStr.c_str();

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const float square_size = g.FontSize;
	const float available_size = window->Size.x - (square_size + style.FramePadding.y * 2) - (window->ScrollbarY ? style.ScrollbarSize : 0) - 2 * style.WindowPadding.x;
	int value_changed = 0;

	BeginGroup();
	PushID(label);

	int typeInt = filterTypeToInt(opt->type);
	ImGui::PushItemWidth((available_size - style.ItemInnerSpacing.x) * 0.3f);
	if (Combo(("##filter-" + iStr + "-type").c_str(), &typeInt, GW2_SCT::Options::getSkillFilterTypeSelectionString().c_str())) {
		opt->type = GW2_SCT::intToFilterType(typeInt);
	}
	ImGui::PopItemWidth();
	SameLine((available_size - style.ItemInnerSpacing.x) * 0.3f, style.ItemInnerSpacing.x);
	ImGui::PushItemWidth((available_size - style.ItemInnerSpacing.x) * 0.7f);
	if (opt->type == GW2_SCT::FilterType::SKILL_ID) {
		std::vector<char> arr(512);
		std::string idStr = std::to_string(opt->skillId);
		std::copy(idStr.begin(), idStr.size() < 512 ? idStr.end() : idStr.begin() + 512, arr.begin());

		struct UserData {
			bool changedBG = false;
		} ud;

		if (InputText(("##filter-" + iStr + "-id").c_str(), arr.data(), 512, ImGuiInputTextFlags_CallbackAlways, [](ImGuiInputTextCallbackData* data) {
			UserData* d = static_cast<UserData*>(data->UserData);
			std::string buf(data->Buf);
			if (buf.empty() || buf.find_first_not_of("0123456789") != std::string::npos) { //validate here
				PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.f, 0.f, 0.f, .6f));
				d->changedBG = true;
			}
			return 0;
			}, &ud)) {
			if (!ud.changedBG) {
				opt->skillId = stoi(std::string(arr.data()));
				value_changed = FilterOptionLine_Value;
			}
		}
		if (ud.changedBG) {
			PopStyleColor();
		}
	} else if (opt->type == GW2_SCT::FilterType::SKILL_NAME) {
		std::vector<char> arr(512);
		std::string idStr(opt->skillName);
		std::copy(idStr.begin(), idStr.size() < 512 ? idStr.end() : idStr.begin() + 512, arr.begin());

		if (InputText(("##filter-" + iStr + "-skillname").c_str(), arr.data(), 512)) {
			opt->skillName = std::string(arr.data());
			value_changed = FilterOptionLine_Value;
		}
	}
	ImGui::PopItemWidth();

	SameLine(available_size, style.ItemInnerSpacing.x);

	PushStyleColor(ImGuiCol_Button, ImVec4(0.67f, 0.40f, 0.40f, 0.60f));
	if (Button("-", ImVec2(square_size + style.FramePadding.y * 2, square_size + style.FramePadding.y * 2))) {
		OpenPopup(langString(GW2_SCT::LanguageCategory::Skill_Filter_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Title));
	}
	PopStyleColor();

	if (BeginPopupModal(langString(GW2_SCT::LanguageCategory::Skill_Filter_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Title))) {
		Text(langString(GW2_SCT::LanguageCategory::Skill_Filter_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Content));
		Separator();
		if (Button(langString(GW2_SCT::LanguageCategory::Skill_Filter_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Confirmation), ImVec2(120, 0))) {
			value_changed |= FilterOptionLine_Remove;
			CloseCurrentPopup();
		}
		SameLine();
		if (Button(langString(GW2_SCT::LanguageCategory::Skill_Filter_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Cancel), ImVec2(120, 0))) {
			CloseCurrentPopup();
		}
		EndPopup();
	}

	PopID();
	EndGroup();

	return value_changed;
}

void ImGui::BeginDisabled() {
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
}

void ImGui::EndDisabled() {
	ImGui::PopStyleColor();
	ImGui::PopItemFlag();
}