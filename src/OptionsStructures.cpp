#include "OptionsStructures.h"

namespace nlohmann {
	template <typename T>
	struct adl_serializer<std::shared_ptr<T>> {
		static void to_json(json& j, const std::shared_ptr<T>& opt) {
			if (opt.get()) {
				j = *opt;
			}
			else {
				j = nullptr;
			}
		}
		static void from_json(const json& j, std::shared_ptr<T>& opt) {
			if (j.is_null()) {
				opt = nullptr;
			} else {
				opt = std::make_shared<T>(j.get<T>());
			}
		}
	};
}

inline int GW2_SCT::textAlignToInt(GW2_SCT::TextAlign type) {
	return (int)type;
}

inline GW2_SCT::TextAlign GW2_SCT::intToTextAlign(int i) {
	if (i < 0 || i > 1) return (GW2_SCT::TextAlign)0;
	return (GW2_SCT::TextAlign)i;
}

inline int GW2_SCT::textCurveToInt(GW2_SCT::TextCurve type) {
	return (int)type;
}

inline GW2_SCT::TextCurve GW2_SCT::intToTextCurve(int i) {
	if (i < 0 || i > 1) return (GW2_SCT::TextCurve)0;
	return (GW2_SCT::TextCurve)i;
}

inline int GW2_SCT::filterTypeToInt(GW2_SCT::FilterType type) {
	return (int)type;
}

inline GW2_SCT::FilterType GW2_SCT::intToFilterType(int i) {
	if (i < 0 || i > 1) return (GW2_SCT::FilterType)0;
	return (GW2_SCT::FilterType)i;
}

inline int GW2_SCT::skillIconDisplayTypeToInt(GW2_SCT::SkillIconDisplayType type) {
	return (int)type;
}

inline GW2_SCT::SkillIconDisplayType GW2_SCT::intSkillIconDisplayType(int i) {
	if (i < 0 || i > 3) return (GW2_SCT::SkillIconDisplayType)0;
	return (GW2_SCT::SkillIconDisplayType)i;
}

void GW2_SCT::to_json(nlohmann::json& j, const options_struct& p) {
	j = nlohmann::json{
		{"revision", p.revision},
		{"globalProfile", p.globalProfile},
		{"profiles", p.profiles},
		{"characterProfileMap", p.characterProfileMap}
	};
}

void GW2_SCT::from_json(const nlohmann::json& j, options_struct& p) {
	j.at("revision").get_to(p.revision);
	j.at("globalProfile").get_to(p.globalProfile);
	j.at("profiles").get_to(p.profiles);
	j.at("characterProfileMap").get_to(p.characterProfileMap);
}

void GW2_SCT::to_json(nlohmann::json& j, const profile_options_struct& p) {
	j = nlohmann::json{
		{"sctEnabled", p.sctEnabled},
		{"scrollingSpeed", p.scrollSpeed},
		{"dropShadow", p.dropShadow},
		{"maximalMessagesInStack", p.messagesInStack},
		{"combineAllMessages", p.combineAllMessages},
		{"masterFont", fotos(p.masterFont, false)},
		{"defaultFontSize", p.defaultFontSize},
		{"defaultCritFontSize", p.defaultCritFontSize},
		{"selfMessageOnlyIncoming", p.selfMessageOnlyIncoming},
		{"outgoingOnlyToTarget", p.outgoingOnlyToTarget},
		{"professionColorGuardian", p.professionColorGuardian},
		{"professionColorWarrior", p.professionColorWarrior},
		{"professionColorEngineer", p.professionColorEngineer},
		{"professionColorRanger", p.professionColorRanger},
		{"professionColorThief", p.professionColorThief},
		{"professionColorElementalist", p.professionColorElementalist},
		{"professionColorMesmer", p.professionColorMesmer},
		{"professionColorNecromancer", p.professionColorNecromancer},
		{"professionColorRevenant", p.professionColorRevenant},
		{"professionColorDefault", p.professionColorDefault},
		{"scrollAreas", p.scrollAreaOptions},
		{"filteredIDs", p.skillFilters},
		{"skillIconsEnabled", p.skillIconsEnabled},
		{"skillIconsPreload", p.preloadAllSkillIcons},
		{"skillIconsDisplayType", p.skillIconsDisplayType}
	};
}

void GW2_SCT::from_json(const nlohmann::json& j, profile_options_struct& p) {
	j.at("sctEnabled").get_to(p.sctEnabled);
	j.at("scrollingSpeed").get_to(p.scrollSpeed);
	j.at("dropShadow").get_to(p.dropShadow);
	j.at("maximalMessagesInStack").get_to(p.messagesInStack);
	j.at("combineAllMessages").get_to(p.combineAllMessages);
	p.masterFont = stofo(j.at("masterFont").get<std::string>(), false);
	j.at("defaultFontSize").get_to(p.defaultFontSize);
	j.at("defaultCritFontSize").get_to(p.defaultCritFontSize);
	j.at("selfMessageOnlyIncoming").get_to(p.selfMessageOnlyIncoming);
	j.at("outgoingOnlyToTarget").get_to(p.outgoingOnlyToTarget);
	j.at("professionColorGuardian").get_to(p.professionColorGuardian);
	j.at("professionColorWarrior").get_to(p.professionColorWarrior);
	j.at("professionColorEngineer").get_to(p.professionColorEngineer);
	j.at("professionColorRanger").get_to(p.professionColorRanger);
	j.at("professionColorThief").get_to(p.professionColorThief);
	j.at("professionColorElementalist").get_to(p.professionColorElementalist);
	j.at("professionColorMesmer").get_to(p.professionColorMesmer);
	j.at("professionColorNecromancer").get_to(p.professionColorNecromancer);
	j.at("professionColorRevenant").get_to(p.professionColorRevenant);
	j.at("professionColorDefault").get_to(p.professionColorDefault);
	j.at("scrollAreas").get_to(p.scrollAreaOptions);
	j.at("filteredIDs").get_to(p.skillFilters);
	j.at("skillIconsEnabled").get_to(p.skillIconsEnabled);
	j.at("skillIconsPreload").get_to(p.preloadAllSkillIcons);
	j.at("skillIconsDisplayType").get_to(p.skillIconsDisplayType);
}

void GW2_SCT::to_json(nlohmann::json& j, const scroll_area_options_struct& p) {
	j = nlohmann::json{
		{"name", p.name},
		{"horrizontalOffset", p.offsetX},
		{"verticalOffset", p.offsetY},
		{"width", p.width},
		{"height", p.height},
		{"textAlign", p.textAlign},
		{"textFlow", p.textCurve},
		{"messageReceivers", p.receivers}
	};
}

void GW2_SCT::from_json(const nlohmann::json& j, scroll_area_options_struct& p) {
	j.at("name").get_to(p.name);
	j.at("horrizontalOffset").get_to(p.offsetX);
	j.at("verticalOffset").get_to(p.offsetY);
	j.at("width").get_to(p.width);
	j.at("height").get_to(p.height);
	j.at("textAlign").get_to(p.textAlign);
	j.at("textFlow").get_to(p.textCurve);
	j.at("messageReceivers").get_to(p.receivers);
}

void GW2_SCT::to_json(nlohmann::json& j, const message_receiver_options_struct& p) {
	j = nlohmann::json{
		{"name", p.name},
		{"category", p.category},
		{"type", p.type},
		{"enabled", p.enabled},
		{"outputTemplate", p.outputTemplate},
		{"color", p.color},
		{"font", fotos(p.font, true)},
		{"fontSize", p.fontSize}
	};
}

void GW2_SCT::from_json(const nlohmann::json& j, message_receiver_options_struct& p) {
	j.at("name").get_to(p.name);
	j.at("category").get_to(p.category);
	j.at("type").get_to(p.type);
	j.at("enabled").get_to(p.enabled);
	j.at("outputTemplate").get_to(p.outputTemplate);
	j.at("color").get_to(p.color);
	p.font = stofo(j.at("font").get<std::string>(), true);
	j.at("fontSize").get_to(p.fontSize);
}

void GW2_SCT::to_json(nlohmann::json& j, const filter_options_struct& p) {
	j = nlohmann::json{
		{"type", p.type},
		{"id", p.skillId},
		{"name", p.skillName}
	};
}

void GW2_SCT::from_json(const nlohmann::json& j, filter_options_struct& p) {
	j.at("type").get_to(p.type);
	j.at("id").get_to(p.skillId);
	j.at("name").get_to(p.skillName);
}