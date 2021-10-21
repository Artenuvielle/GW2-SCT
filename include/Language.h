#pragma once
#include "Common.h"
#include "SimpleIni.h"
#include "LanguageDefinitions.h"

// from https://stackoverflow.com/questions/147267/easy-way-to-use-variables-of-enum-types-as-string-in-c#202511
// expansion macro for enum value definition
#define ENUM_VALUE(name,assign) name assign,

// expansion macro for enum to string conversion
#define ENUM_CASE(name,assign) case name: return #name;

// expansion macro for string to enum conversion
#define ENUM_STRCMP(name,assign) if (!strcmp(str,#name)) return name;

/// declare the access function and define enum values
#define DECLARE_ENUM(EnumType,ENUM_DEF) \
  enum EnumType { \
    ENUM_DEF(ENUM_VALUE) \
  }; \
  const char *GetString(EnumType dummy); \
  EnumType Get##EnumType##Value(const char *string); \

/// define the access function names
#define DEFINE_ENUM(EnumType,ENUM_DEF) \
  const char *GetString(EnumType value) \
  { \
    switch(value) \
    { \
      ENUM_DEF(ENUM_CASE) \
      default: return ""; /* handle input error */ \
    } \
  } \
  EnumType Get##EnumType##Value(const char *str) \
  { \
    ENUM_DEF(ENUM_STRCMP) \
    return (EnumType)0; /* handle input error */ \
  } \

namespace GW2_SCT {
    DECLARE_ENUM(LanguageCategory, LANGUAGE_CATEGORY)
    DECLARE_ENUM(LanguageKey, LANGUAGE_KEY)

	class Language {
	public:
		static void load();
		static void reload();
		static const char* get(LanguageKey key, bool imgui_safe = false);
		static const char* get(LanguageCategory section, LanguageKey key, bool imgui_safe = false);
	private:
		Language() {}
        static std::map<LanguageCategory, std::map<LanguageKey, const char*>> languageStrings;
        static std::map<LanguageCategory, std::map<LanguageKey, const char*>> languageStringsImguiSafe;
		static void saveDefault(const char*);
		static CSimpleIniA languageFile;
		static bool loadSuccessful;
	};
}