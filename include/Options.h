#pragma once
#include <vector>
#include <functional>
#include <map>
#include <string>
#include "UtilStructures.h"
#include "OptionsStructures.h"

namespace GW2_SCT {
	std::map<char, std::string> mapParameterListToLanguage(const char* section, std::vector<char> list);

	class Options {
	public:
		static bool getIsSkillFiltered(uint32_t skillId, const char* skillName);
		static options_struct* get() { return &opt; }
		static void paint();
		static void open();
		static void save();
		static void load();
		static void setDefault();
		static std::string getFontSelectionString(bool withMaster = true) { return withMaster ? fontSelectionStringWithMaster : fontSelectionString; };
		static std::string getFontSizeTypeSelectionString() { return fontSizeTypeSelectionString; };
		static std::string getSkillFilterTypeSelectionString() { return skillFilterTypeSelectionString; };
		static bool isOptionsWindowOpen();
	private:
		static void paintGeneral();
		static void paintScrollAreas();
		static void paintProfessionColors();
		static void paintSkillFilters();
		static void paintSkillIcons();
		static options_struct opt;
		static bool windowIsOpen;
		static std::string fontSelectionString;
		static std::string fontSizeTypeSelectionString;
		static std::string fontSelectionStringWithMaster;
		static std::string skillFilterTypeSelectionString;
		static std::string backupFileName;
	};

	struct receiver_information {
		std::string iniPrefix;
		std::map<char, std::string> options;
		message_receiver_options_struct defaultReceiver;
	};
	extern const std::map<MessageCategory, const std::map<MessageType, receiver_information>> receiverInformationPerCategoryAndType;
	extern const std::map<MessageCategory, std::string> categoryNames;
	extern const std::map<MessageType, std::string> typeNames;
}
