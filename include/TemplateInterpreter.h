#pragma once
#include <string>
#include <vector>
#include <map>
#include "imgui.h"
#include "SkillIconManager.h"
#include "FontManager.h"

ImVec2 getTextSize(const char* text, GW2_SCT::FontType* font, float fontSize, bool use_bbc = true);

namespace GW2_SCT {
	class TemplateInterpreter {
	public:
		struct InterpretedText {
			std::string str;
			ImVec2 offset;
			ImVec2 size;
			ImU32 color;
			SkillIcon* icon;
			InterpretedText(std::string str, ImVec2 size, ImVec2 offset, ImU32 color, SkillIcon* icon = nullptr)
				: str(str), size(size), offset(offset), color(color), icon(icon) {}
		};
		static bool validate(std::string, std::map<char, std::string>);
		static std::vector<InterpretedText> interpret(GW2_SCT::FontType* font, float fontSize, ImU32 defaultColor, std::string t);
	};
}
