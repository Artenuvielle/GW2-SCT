#pragma once
#include <string>
#include <algorithm>
#include "Common.h"
#include "json.hpp"
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
        struct LanguageEntry {
        private:
            std::string value;
            bool optional = false;
            bool resolved = false;
            bool hasValue = false;
            LanguageEntry* reference = nullptr;
            std::vector<LanguageEntry*> referenced = {};
            std::vector<std::pair<LanguageCategory, LanguageKey>> entriesToResolve = {};
        public:
            LanguageEntry(std::vector<std::pair<LanguageCategory, LanguageKey>> resolveList) : entriesToResolve(resolveList) {};
            LanguageEntry() {};
            LanguageEntry(std::string s) : value(s), optional(true), resolved(true), hasValue(true) {};
            bool resolve() {
                if (!resolved) {
                    if (reference != nullptr) {
                        reference->referenced.erase(std::find(reference->referenced.begin(), reference->referenced.end(), this));
                        reference = nullptr;
                    }
                    for (auto& entry : entriesToResolve) {
                        auto catIt = languageStrings.find(entry.first);
                        if (catIt != languageStrings.end()) {
                            auto keyIt = catIt->second.find(entry.second);
                            if (keyIt != catIt->second.end() && keyIt->second.resolve()) {
                                value = keyIt->second.value;
                                hasValue = true;
                                reference = &keyIt->second;
                                keyIt->second.referenced.push_back(this);
                                break;
                            }
                        }
                    }
                    resolved = true;
                }
                return hasValue;
            }
            void resetResolve() {
                resolved = false;
            }
            bool isOptional() { return optional; }
            bool isReference() { return reference != nullptr; }
            bool isReferenced() { return referenced.size() > 0; }
            void setValue(std::string v) {
                value = v;
                hasValue = true;
                resolved = true;
                if (reference != nullptr) {
                    reference->referenced.erase(std::find(reference->referenced.begin(), reference->referenced.end(), this));
                    reference = nullptr;
                }
            }
            operator std::string() const {
                return value;
            }
            operator const char*() const {
                return value.c_str();
            }
        };
    private:
        Language() {}
        static std::map<LanguageCategory, std::map<LanguageKey, LanguageEntry>> languageStrings;
        static std::map<LanguageCategory, std::map<LanguageKey, LanguageEntry>> languageStringsImguiSafe;
        static const nlohmann::ordered_json defaultLanguage;
        static nlohmann::ordered_json loadedLanguage;
        static void saveDefault(const char*);
        static bool loadSuccessful;
    };
}