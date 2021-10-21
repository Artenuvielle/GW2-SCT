#pragma once
#include <vector>
#include <string>
#include "json.hpp"
#include "Common.h"
#include "UtilStructures.h"

namespace GW2_SCT {
	enum class TextAlign {
		LEFT = 0,
		CENTER,
		RIGHT
	};
	extern int textAlignToInt(TextAlign type);
	extern TextAlign intToTextAlign(int i);

	enum class TextCurve {
		LEFT = 0,
		STRAIGHT,
		RIGHT,
		STATIC
	};
	extern int textCurveToInt(TextCurve type);
	extern TextCurve intToTextCurve(int i);

	enum class FilterType {
		SKILL_ID = 0,
		SKILL_NAME
	};
	extern int filterTypeToInt(FilterType type);
	extern FilterType intToFilterType(int i);

	class scroll_area_options_struct;
	class message_receiver_options_struct;
	class filter_options_struct;

	class options_struct {
	public:
		std::string revision = "1.0";
		bool sctEnabled = true;
		float scrollSpeed = 90.f;
		bool dropShadow = true;
		int messagesInStack = 3;
		bool combineAllMessages = true;
		FontId masterFont = 0;
		float defaultFontSize = 22.f;
		float defaultCritFontSize = 30.f;
		bool selfMessageOnlyIncoming = false;
		bool outgoingOnlyToTarget = false;
		std::string professionColorGuardian = "72C1D9";
		std::string professionColorWarrior = "FFD166";
		std::string professionColorEngineer = "D09C59";
		std::string professionColorRanger = "8EDF44";
		std::string professionColorThief = "C08F95";
		std::string professionColorElementalist = "F68A87";
		std::string professionColorMesmer = "B679D5";
		std::string professionColorNecromancer = "52A76F";
		std::string professionColorRevenant = "D16E5A";
		std::string professionColorDefault = "FF0000";
		ObservableVector<std::shared_ptr<scroll_area_options_struct>> scrollAreaOptions = {};
		std::vector<filter_options_struct> skillFilters = {};
		ObservableValue<bool> skillIconsEnabled = false;
		ObservableValue<bool> preloadAllSkillIcons = false;
	};
	void to_json(nlohmann::json& j, const options_struct& p);
	void from_json(const nlohmann::json& j, options_struct& p);

	enum class ScrollAreaOutlineState {
		NONE = 0,
		LIGHT,
		FULL
	};

	class scroll_area_options_struct {
	public:
		std::string name = "";
		float offsetX = 0;
		float offsetY = 0;
		float width = 0;
		float height = 0;
		TextAlign textAlign = TextAlign::LEFT;
		TextCurve textCurve = TextCurve::LEFT;
		ScrollAreaOutlineState outlineState = ScrollAreaOutlineState::NONE;
		ObservableVector<std::shared_ptr<message_receiver_options_struct>> receivers = {};
	};
	void to_json(nlohmann::json& j, const scroll_area_options_struct& p);
	void from_json(const nlohmann::json& j, scroll_area_options_struct& p);

	class message_receiver_options_struct {
	public:
		std::string name = "";
		MessageCategory category = MessageCategory::PLAYER_OUT;
		MessageType type = MessageType::PHYSICAL;
		bool enabled = true;
		ObservableValue<std::string> outputTemplate = std::string("");
		ObservableValue<std::string> color = std::string("#FFFFFF");
		ObservableValue<FontId> font = 0;
		ObservableValue<float> fontSize = 22.f;
	};
	void to_json(nlohmann::json& j, const message_receiver_options_struct& p);
	void from_json(const nlohmann::json& j, message_receiver_options_struct& p);

	class filter_options_struct {
	public:
		FilterType type = FilterType::SKILL_ID;
		uint32_t skillId = 0;
		std::string skillName = "";
	};
	void to_json(nlohmann::json& j, const filter_options_struct& p);
	void from_json(const nlohmann::json& j, filter_options_struct& p);
}