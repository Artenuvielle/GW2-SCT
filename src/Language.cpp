#include "Language.h"

namespace GW2_SCT {
	DEFINE_ENUM(LanguageCategory, LANGUAGE_CATEGORY)
	DEFINE_ENUM(LanguageKey, LANGUAGE_KEY)
};

#define forCategoryAndType(cat, typ) GW2_SCT::LanguageCategory::Message_ ## cat ## _ ## typ, GW2_SCT::LanguageCategory::Message_Category_ ## cat, GW2_SCT::LanguageCategory::Message_Type_ ## typ

std::pair<GW2_SCT::LanguageCategory, std::map<GW2_SCT::LanguageKey, GW2_SCT::Language::LanguageEntry>> createRequiredMessagePair(
    GW2_SCT::LanguageCategory mainCategory, GW2_SCT::LanguageCategory categoryCategroy, GW2_SCT::LanguageCategory typeCategory, std::vector<GW2_SCT::LanguageKey> keyList
) {
    std::map<GW2_SCT::LanguageKey, GW2_SCT::Language::LanguageEntry> map;
    for (const auto& key : keyList) {
        map.insert(std::pair<GW2_SCT::LanguageKey, GW2_SCT::Language::LanguageEntry>(key, GW2_SCT::Language::LanguageEntry({
            { typeCategory, key },
            { categoryCategroy, key },
            { GW2_SCT::LanguageCategory::Message_General, key }
        })));
    }
    return { mainCategory, map };
}


bool GW2_SCT::Language::loadSuccessful = false;
nlohmann::ordered_json GW2_SCT::Language::loadedLanguage;
std::map<GW2_SCT::LanguageCategory, std::map<GW2_SCT::LanguageKey, GW2_SCT::Language::LanguageEntry>> GW2_SCT::Language::languageStringsImguiSafe = {};
std::map<GW2_SCT::LanguageCategory, std::map<GW2_SCT::LanguageKey, GW2_SCT::Language::LanguageEntry>> GW2_SCT::Language::languageStrings = {
    { GW2_SCT::LanguageCategory::General, {
        { GW2_SCT::LanguageKey::Outdated_Popup_Title, {} },
        { GW2_SCT::LanguageKey::Outdated_Popup_Content, {} },
        { GW2_SCT::LanguageKey::Outdated_Popup_Found_Version, {} },
        { GW2_SCT::LanguageKey::Outdated_Popup_Required_Version, {} },
        { GW2_SCT::LanguageKey::Outdated_Popup_Confirmation, {} },
        { GW2_SCT::LanguageKey::Font, {} },
        { GW2_SCT::LanguageKey::Font_Size, {} },
        { GW2_SCT::LanguageKey::Font_Size_Type, {} },
        { GW2_SCT::LanguageKey::Default_Font_Size, {} },
        { GW2_SCT::LanguageKey::Default_Crit_Font_Size, {} },
        { GW2_SCT::LanguageKey::Custom_Font_Size, {} },
        { GW2_SCT::LanguageKey::Skill_Filter_Type_ID, {} },
        { GW2_SCT::LanguageKey::Skill_Filter_Type_Name, {} },
        { GW2_SCT::LanguageKey::Font_Master, {} },
        { GW2_SCT::LanguageKey::Font_Default, {} },
        { GW2_SCT::LanguageKey::Text_Align_Left, {} },
        { GW2_SCT::LanguageKey::Text_Align_Center, {} },
        { GW2_SCT::LanguageKey::Text_Align_Right, {} },
        { GW2_SCT::LanguageKey::Text_Curve_Left, {} },
        { GW2_SCT::LanguageKey::Text_Curve_Straight, {} },
        { GW2_SCT::LanguageKey::Text_Curve_Right, {} },
        { GW2_SCT::LanguageKey::Old_Save_Popup_Title, {} },
        { GW2_SCT::LanguageKey::Old_Save_Popup_Content, {} },
        { GW2_SCT::LanguageKey::Old_Save_Popup_Backup_Made, {} },
        { GW2_SCT::LanguageKey::Old_Save_Popup_Confirmation, {} },
        { GW2_SCT::LanguageKey::Unknown_Skill_Source, {} },
        { GW2_SCT::LanguageKey::Unknown_Skill_Target, {} },
        { GW2_SCT::LanguageKey::Unknown_Skill_Name, {} },
        { GW2_SCT::LanguageKey::Default_Scroll_Area_Incoming, {} },
        { GW2_SCT::LanguageKey::Default_Scroll_Area_Outgoing, {} },
    } },
    { GW2_SCT::LanguageCategory::Option_UI, {
        { GW2_SCT::LanguageKey::Title, {} },
        { GW2_SCT::LanguageKey::Menu_Bar_General, {} },
        { GW2_SCT::LanguageKey::Menu_Bar_Scroll_Areas, {} },
        { GW2_SCT::LanguageKey::Menu_Bar_Profession_Colors, {} },
        { GW2_SCT::LanguageKey::Menu_Bar_Filtered_Skills, {} },
        { GW2_SCT::LanguageKey::Menu_Bar_Skill_Icons, {} },
        { GW2_SCT::LanguageKey::Menu_Bar_Profiles, {} },
        { GW2_SCT::LanguageKey::General_Enabled, {} },
        { GW2_SCT::LanguageKey::General_Scrolling_Speed, {} },
        { GW2_SCT::LanguageKey::General_Scrolling_Speed_Toolip, {} },
        { GW2_SCT::LanguageKey::General_Drop_Shadow, {} },
        { GW2_SCT::LanguageKey::General_Max_Messages, {} },
        { GW2_SCT::LanguageKey::General_Max_Messages_Toolip, {} },
        { GW2_SCT::LanguageKey::General_Combine_Messages, {} },
        { GW2_SCT::LanguageKey::General_Combine_Messages_Toolip, {} },
        { GW2_SCT::LanguageKey::General_Self_Only_As_Incoming, {} },
        { GW2_SCT::LanguageKey::General_Self_Only_As_Incoming_Toolip, {} },
        { GW2_SCT::LanguageKey::General_Out_Only_For_Target, {} },
        { GW2_SCT::LanguageKey::General_Out_Only_For_Target_Toolip, {} },
        { GW2_SCT::LanguageKey::Scroll_Areas_Name, {} },
        { GW2_SCT::LanguageKey::Receiver_Name, {} },
        { GW2_SCT::LanguageKey::Messages_Category, {} },
        { GW2_SCT::LanguageKey::Messages_Category_Player_Out, {} },
        { GW2_SCT::LanguageKey::Messages_Category_Player_In, {} },
        { GW2_SCT::LanguageKey::Messages_Category_Pet_Out, {} },
        { GW2_SCT::LanguageKey::Messages_Category_Pet_In, {} },
        { GW2_SCT::LanguageKey::Messages_Type, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Physical, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Crit, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Bleeding, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Burning, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Poison, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Confusion, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Retaliation, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Torment, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Dot, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Heal, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Hot, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Shield_Receive, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Shield_Remove, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Block, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Evade, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Invulnerable, {} },
        { GW2_SCT::LanguageKey::Messages_Type_Miss, {} },
        { GW2_SCT::LanguageKey::Profession_Colors_Guardian, {} },
        { GW2_SCT::LanguageKey::Profession_Colors_Warrior, {} },
        { GW2_SCT::LanguageKey::Profession_Colors_Engineer, {} },
        { GW2_SCT::LanguageKey::Profession_Colors_Ranger, {} },
        { GW2_SCT::LanguageKey::Profession_Colors_Thief, {} },
        { GW2_SCT::LanguageKey::Profession_Colors_Elementalist, {} },
        { GW2_SCT::LanguageKey::Profession_Colors_Mesmer, {} },
        { GW2_SCT::LanguageKey::Profession_Colors_Necromancer, {} },
        { GW2_SCT::LanguageKey::Profession_Colors_Revenant, {} },
        { GW2_SCT::LanguageKey::Profession_Colors_Undetectable, {} },
    } },
    { GW2_SCT::LanguageCategory::Example_Message_UI, {
        { GW2_SCT::LanguageKey::Title, {} },
        { GW2_SCT::LanguageKey::Start_Recording, {} },
        { GW2_SCT::LanguageKey::Stop_Recording, {} },
        { GW2_SCT::LanguageKey::Start_Emitting, {} },
        { GW2_SCT::LanguageKey::Stop_Emitting, {} },
        { GW2_SCT::LanguageKey::Clear_Recorded_Messages, {} }
    } },
    { GW2_SCT::LanguageCategory::Message, {
        { GW2_SCT::LanguageKey::Multiple_Sources, {} },
        { GW2_SCT::LanguageKey::Number_Of_Hits, {} },
    } },
    { GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, {
        { GW2_SCT::LanguageKey::Scroll_Areas_New, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Title, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Content, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Confirmation, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Cancel, {} },
        { GW2_SCT::LanguageKey::Horizontal_Offset, {} },
        { GW2_SCT::LanguageKey::Vertical_Offset, {} },
        { GW2_SCT::LanguageKey::Width, {} },
        { GW2_SCT::LanguageKey::Height, {} },
        { GW2_SCT::LanguageKey::Text_Align, {} },
        { GW2_SCT::LanguageKey::Text_Flow, {} },
        { GW2_SCT::LanguageKey::All_Receivers, {} },
        { GW2_SCT::LanguageKey::New_Receiver, {} },
    } },
    { GW2_SCT::LanguageCategory::Receiver_Option_UI, {
        { GW2_SCT::LanguageKey::Delete_Confirmation_Title, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Content, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Confirmation, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Cancel, {} },
        { GW2_SCT::LanguageKey::Template, {} },
        { GW2_SCT::LanguageKey::Available_Parameters, {} },
    } },
    { GW2_SCT::LanguageCategory::Skill_Filter_Option_UI, {
        { GW2_SCT::LanguageKey::Filter_By, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Title, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Content, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Confirmation, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Cancel, {} },
    } },
    { GW2_SCT::LanguageCategory::Skill_Icons_Option_UI, {
        { GW2_SCT::LanguageKey::Skill_Icons_Warning, {} },
        { GW2_SCT::LanguageKey::Skill_Icons_Enable, {} },
        { GW2_SCT::LanguageKey::Skill_Icons_Preload_Description, {} },
        { GW2_SCT::LanguageKey::Skill_Icons_Preload, {} },
        { GW2_SCT::LanguageKey::Skill_Icons_Display_Type, {} },
        { GW2_SCT::LanguageKey::Skill_Icons_Display_Type_Normal, {} },
        { GW2_SCT::LanguageKey::Skill_Icons_Display_Type_Black_Culled, {} },
        { GW2_SCT::LanguageKey::Skill_Icons_Display_Type_Black_Border_Culled, {} },
        { GW2_SCT::LanguageKey::Skill_Icons_Display_Type_Black_Border_Touching_Culled, {} },
    } },
    { GW2_SCT::LanguageCategory::Profile_Option_UI, {
        { GW2_SCT::LanguageKey::Profile_Description, {} },
        { GW2_SCT::LanguageKey::Master_Profile, {} },
        { GW2_SCT::LanguageKey::Character_Specific_Profile_Heading, {} },
        { GW2_SCT::LanguageKey::Character_Specific_Profile_Enabled, {} },
        { GW2_SCT::LanguageKey::Current_Profile_Heading, {} },
        { GW2_SCT::LanguageKey::Current_Profile_Name, {} },
        { GW2_SCT::LanguageKey::Create_Profile_Copy, {} },
        { GW2_SCT::LanguageKey::Profile_Copy_Suffix, {} },
        { GW2_SCT::LanguageKey::Delete_Profile, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Title, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Content, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Confirmation, {} },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Cancel, {} },
    } },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Physical), {
        GW2_SCT::LanguageKey::Parameter_Description_v,
        GW2_SCT::LanguageKey::Parameter_Description_n,
        GW2_SCT::LanguageKey::Parameter_Description_s,
        GW2_SCT::LanguageKey::Parameter_Description_i,
        GW2_SCT::LanguageKey::Default_Value,
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Crit), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Bleeding), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Burning), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Poison), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Confusion), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Retaliation), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Torment), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, DoT), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Heal), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, HoT), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Shield_Got), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Shield), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Block), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Evade), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Invulnerable), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_Out, Miss), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Physical), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Crit), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Bleeding), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Burning), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Poison), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Confusion), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Retaliation), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Torment), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, DoT), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Heal), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, HoT), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Shield_Got), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Shield), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Block), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Evade), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Invulnerable), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Player_In, Miss), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Physical), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Crit), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Bleeding), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Burning), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Poison), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Confusion), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Retaliation), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Torment), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, DoT), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Heal), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, HoT), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Shield_Got), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Shield), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Block), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Evade), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Invulnerable), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_Out, Miss), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Physical), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Crit), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Bleeding), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Burning), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Poison), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Confusion), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Retaliation), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Torment), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, DoT), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Heal), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, HoT), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Shield_Got), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Shield), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_v,
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Block), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Evade), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Invulnerable), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) },
    { createRequiredMessagePair(forCategoryAndType(Pet_In, Miss), {
        {
            GW2_SCT::LanguageKey::Parameter_Description_n,
            GW2_SCT::LanguageKey::Parameter_Description_s,
            GW2_SCT::LanguageKey::Parameter_Description_p,
            GW2_SCT::LanguageKey::Parameter_Description_c,
            GW2_SCT::LanguageKey::Parameter_Description_r,
            GW2_SCT::LanguageKey::Parameter_Description_i,
            GW2_SCT::LanguageKey::Default_Value,
        },
    }) }
};

nlohmann::ordered_json convertLanguageCategoryAndKeyStrings(std::map<GW2_SCT::LanguageCategory, std::map<GW2_SCT::LanguageKey,const char*>> in) {
    nlohmann::ordered_json out;
    for (auto& catPair : in) {
        nlohmann::ordered_json cat;
        for (auto& keyPair : catPair.second) {
            cat[GW2_SCT::GetString(keyPair.first)] = keyPair.second;
        }
        out[GW2_SCT::GetString(catPair.first)] = cat;
    }
    return out;
}

const nlohmann::ordered_json GW2_SCT::Language::defaultLanguage = convertLanguageCategoryAndKeyStrings({
    { GW2_SCT::LanguageCategory::General, {
        { GW2_SCT::LanguageKey::Outdated_Popup_Title, "ArcDPS outdated" },
        { GW2_SCT::LanguageKey::Outdated_Popup_Content, "You are running an old version of ArcDPS some features of SCT\nmay not work properly on can cause crashes. Updating ArcDPS is advised." },
        { GW2_SCT::LanguageKey::Outdated_Popup_Found_Version, "Found version" },
        { GW2_SCT::LanguageKey::Outdated_Popup_Required_Version, "Required version" },
        { GW2_SCT::LanguageKey::Outdated_Popup_Confirmation, "OK" },
        { GW2_SCT::LanguageKey::Font, "Font" },
        { GW2_SCT::LanguageKey::Font_Size, "Font Size" },
        { GW2_SCT::LanguageKey::Font_Size_Type, "Font Size Type" },
        { GW2_SCT::LanguageKey::Default_Font_Size, "Default Font Size" },
        { GW2_SCT::LanguageKey::Default_Crit_Font_Size, "Default Crit Font Size" },
        { GW2_SCT::LanguageKey::Custom_Font_Size, "Custom Font Size" },
        { GW2_SCT::LanguageKey::Skill_Filter_Type_ID, "Skill ID" },
        { GW2_SCT::LanguageKey::Skill_Filter_Type_Name, "Skill Name" },
        { GW2_SCT::LanguageKey::Font_Master, "Master Font" },
        { GW2_SCT::LanguageKey::Font_Default, "Default Font" },
        { GW2_SCT::LanguageKey::Text_Align_Left, "Left" },
        { GW2_SCT::LanguageKey::Text_Align_Center, "Middle" },
        { GW2_SCT::LanguageKey::Text_Align_Right, "Right" },
        { GW2_SCT::LanguageKey::Text_Curve_Left, "Left" },
        { GW2_SCT::LanguageKey::Text_Curve_Straight, "Straight" },
        { GW2_SCT::LanguageKey::Text_Curve_Right, "Right" },
        { GW2_SCT::LanguageKey::Old_Save_Popup_Title, "Old Save File Version" },
        { GW2_SCT::LanguageKey::Old_Save_Popup_Content, "The found options file was saved with an older version of GW2 SCT.\nSome of your settings may have been reverted to default." },
        { GW2_SCT::LanguageKey::Old_Save_Popup_Backup_Made, "A backup of your old options save file can be found at:" },
        { GW2_SCT::LanguageKey::Old_Save_Popup_Confirmation, "OK" },
        { GW2_SCT::LanguageKey::Unknown_Skill_Source, "<Area>" },
        { GW2_SCT::LanguageKey::Unknown_Skill_Target, "<Area>" },
        { GW2_SCT::LanguageKey::Unknown_Skill_Name, "<Skill>" },
        { GW2_SCT::LanguageKey::Default_Scroll_Area_Incoming, "Incoming Events" },
        { GW2_SCT::LanguageKey::Default_Scroll_Area_Outgoing, "Outgoing Events" },
    } },
    { GW2_SCT::LanguageCategory::Option_UI, {
        { GW2_SCT::LanguageKey::Title, "SCT Options" },
        { GW2_SCT::LanguageKey::Menu_Bar_General, "General" },
        { GW2_SCT::LanguageKey::Menu_Bar_Scroll_Areas, "Scroll Areas" },
        { GW2_SCT::LanguageKey::Menu_Bar_Profession_Colors, "Profession Colors" },
        { GW2_SCT::LanguageKey::Menu_Bar_Filtered_Skills, "Filtered Skills" },
        { GW2_SCT::LanguageKey::Menu_Bar_Skill_Icons, "Skill Icons" },
        { GW2_SCT::LanguageKey::Menu_Bar_Profiles, "Profiles" },
        { GW2_SCT::LanguageKey::General_Enabled, "SCT enabled" },
        { GW2_SCT::LanguageKey::General_Scrolling_Speed, "Scrolling Speed" },
        { GW2_SCT::LanguageKey::General_Scrolling_Speed_Toolip, "The velocity at which each message is scrolling downwards in pixels per seconds." },
        { GW2_SCT::LanguageKey::General_Drop_Shadow, "Drop Shadow" },
        { GW2_SCT::LanguageKey::General_Max_Messages, "Maximal Messages in Stack" },
        { GW2_SCT::LanguageKey::General_Max_Messages_Toolip, "Messages that want to appear on screen but there is not enough room for them yet\nremain in a stack. When the size of the stack would exceed this given number all\nmessages will be forced to teleport a bit downwards for a new message to appear." },
        { GW2_SCT::LanguageKey::General_Combine_Messages, "Combine all Messages" },
        { GW2_SCT::LanguageKey::General_Combine_Messages_Toolip, "If enabled new messages will only be combined with the newest message in the\nstack if possible.\nIf disabled new messages will be combined with any of the messages in stack\nif possible, starting from newest to oldest." },
        { GW2_SCT::LanguageKey::General_Self_Only_As_Incoming, "Show self messages only as incoming" },
        { GW2_SCT::LanguageKey::General_Self_Only_As_Incoming_Toolip, "This option effects only messages with both the source and destination\nbeing yourself.\nIf enabled these messages will only be triggered as 'Player Incoming'\nmessages.\nIf disabled these messages will be triggered as both 'Player Incoming'\nand 'Player outgoing' messages." },
        { GW2_SCT::LanguageKey::General_Out_Only_For_Target, "Show outgoing messages only for target" },
        { GW2_SCT::LanguageKey::General_Out_Only_For_Target_Toolip, "If enabled only messages that have your current target as destination will\nbe triggered in both categories 'Player Outgoing' and 'Pet Outgoing'." },
        { GW2_SCT::LanguageKey::Scroll_Areas_Name, "Scroll Area Name" },
        { GW2_SCT::LanguageKey::Receiver_Name, "Receiver Name" },
        { GW2_SCT::LanguageKey::Messages_Category, "Message Category" },
        { GW2_SCT::LanguageKey::Messages_Category_Player_Out, "Player Outgoing" },
        { GW2_SCT::LanguageKey::Messages_Category_Player_In, "Player Incoming" },
        { GW2_SCT::LanguageKey::Messages_Category_Pet_Out, "Pet Outgoing" },
        { GW2_SCT::LanguageKey::Messages_Category_Pet_In, "Pet Incoming" },
        { GW2_SCT::LanguageKey::Messages_Type, "Message Type" },
        { GW2_SCT::LanguageKey::Messages_Type_Physical, "Physical Hit" },
        { GW2_SCT::LanguageKey::Messages_Type_Crit, "Physical Crit" },
        { GW2_SCT::LanguageKey::Messages_Type_Bleeding, "Bleeding Damage" },
        { GW2_SCT::LanguageKey::Messages_Type_Burning, "Burning Damage" },
        { GW2_SCT::LanguageKey::Messages_Type_Poison, "Poison Damage" },
        { GW2_SCT::LanguageKey::Messages_Type_Confusion, "Confusion Damage" },
        { GW2_SCT::LanguageKey::Messages_Type_Retaliation, "Retaliation Damage" },
        { GW2_SCT::LanguageKey::Messages_Type_Torment, "Torment Damage" },
        { GW2_SCT::LanguageKey::Messages_Type_Dot, "Other Continuous Damage" },
        { GW2_SCT::LanguageKey::Messages_Type_Heal, "Heal" },
        { GW2_SCT::LanguageKey::Messages_Type_Hot, "Heal over Time" },
        { GW2_SCT::LanguageKey::Messages_Type_Shield_Receive, "Shield Created" },
        { GW2_SCT::LanguageKey::Messages_Type_Shield_Remove, "Shielded Damage" },
        { GW2_SCT::LanguageKey::Messages_Type_Block, "Block" },
        { GW2_SCT::LanguageKey::Messages_Type_Evade, "Evade" },
        { GW2_SCT::LanguageKey::Messages_Type_Invulnerable, "Invulnerable" },
        { GW2_SCT::LanguageKey::Messages_Type_Miss, "Miss" },
        { GW2_SCT::LanguageKey::Profession_Colors_Guardian, "Guardian" },
        { GW2_SCT::LanguageKey::Profession_Colors_Warrior, "Warrior" },
        { GW2_SCT::LanguageKey::Profession_Colors_Engineer, "Engineer" },
        { GW2_SCT::LanguageKey::Profession_Colors_Ranger, "Ranger" },
        { GW2_SCT::LanguageKey::Profession_Colors_Thief, "Thief" },
        { GW2_SCT::LanguageKey::Profession_Colors_Elementalist, "Elementalist" },
        { GW2_SCT::LanguageKey::Profession_Colors_Mesmer, "Mesmer" },
        { GW2_SCT::LanguageKey::Profession_Colors_Necromancer, "Necromancer" },
        { GW2_SCT::LanguageKey::Profession_Colors_Revenant, "Revenant" },
        { GW2_SCT::LanguageKey::Profession_Colors_Undetectable, "Undetectable profession" },
    } },
    { GW2_SCT::LanguageCategory::Example_Message_UI, {
        { GW2_SCT::LanguageKey::Title, "SCT Example Messages" },
        { GW2_SCT::LanguageKey::Start_Recording, "Start recording messages" },
        { GW2_SCT::LanguageKey::Stop_Recording, "Stop recording messages" },
        { GW2_SCT::LanguageKey::Start_Emitting, "Start emitting messages" },
        { GW2_SCT::LanguageKey::Stop_Emitting, "Stop emitting messages" },
        { GW2_SCT::LanguageKey::Clear_Recorded_Messages, "Clear all" }
    } },
    { GW2_SCT::LanguageCategory::Message, {
        { GW2_SCT::LanguageKey::Multiple_Sources, "Multiple" },
        { GW2_SCT::LanguageKey::Number_Of_Hits, "Hits" },
    } },
    { GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, {
        { GW2_SCT::LanguageKey::Scroll_Areas_New, "New Scroll Area" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Title, "Delete?" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Content, "Are you sure you want to delete this scroll area?\nAll receivers registered within this scroll area\nwill be deleted aswell.\n\n" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Confirmation, "OK" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Cancel, "Cancel" },
        { GW2_SCT::LanguageKey::Horizontal_Offset, "Horizontal Offset" },
        { GW2_SCT::LanguageKey::Vertical_Offset, "Vertical Offset" },
        { GW2_SCT::LanguageKey::Width, "Width" },
        { GW2_SCT::LanguageKey::Height, "Height" },
        { GW2_SCT::LanguageKey::Text_Align, "Text Align" },
        { GW2_SCT::LanguageKey::Text_Flow, "Text Flow" },
        { GW2_SCT::LanguageKey::All_Receivers, "Message receivers:" },
        { GW2_SCT::LanguageKey::New_Receiver, "New receiver:" },
    } },
    { GW2_SCT::LanguageCategory::Receiver_Option_UI, {
        { GW2_SCT::LanguageKey::Delete_Confirmation_Title, "Delete?" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Content, "Are you sure you want to delete this message receiver?\n\n" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Confirmation, "OK" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Cancel, "Cancel" },
        { GW2_SCT::LanguageKey::Template, "Template" },
        { GW2_SCT::LanguageKey::Available_Parameters, "Available template parameters" },
    } },
    { GW2_SCT::LanguageCategory::Skill_Filter_Option_UI, {
        { GW2_SCT::LanguageKey::Filter_By, "Filter by" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Title, "Delete?" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Content, "Are you sure you want to delete this skill id filter?" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Confirmation, "OK" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Cancel, "Cancel" },
    } },
    { GW2_SCT::LanguageCategory::Skill_Icons_Option_UI, {
        { GW2_SCT::LanguageKey::Skill_Icons_Warning, "Warning: Using skill icons will establish an additional connection to the GW2 rendering API. All downloaded icons will be saved in" },
        { GW2_SCT::LanguageKey::Skill_Icons_Enable, "Enable skill icons" },
        { GW2_SCT::LanguageKey::Skill_Icons_Preload_Description, "Preloading skill icons will download every possible skill icon from the GW2 rendering API. This will require ~25MB of free memory space available. If not enabled, icons will be only downloaded as needed." },
        { GW2_SCT::LanguageKey::Skill_Icons_Preload, "Preload all skill icons" },
        { GW2_SCT::LanguageKey::Skill_Icons_Display_Type, "Transparency" },
        { GW2_SCT::LanguageKey::Skill_Icons_Display_Type_Normal, "None" },
        { GW2_SCT::LanguageKey::Skill_Icons_Display_Type_Black_Culled, "Cull dark pixels" },
        { GW2_SCT::LanguageKey::Skill_Icons_Display_Type_Black_Border_Culled, "Cull border squares of dark pixels" },
        { GW2_SCT::LanguageKey::Skill_Icons_Display_Type_Black_Border_Touching_Culled, "Cull dark pixels touching border or other transparent pixels" },
    } },
    { GW2_SCT::LanguageCategory::Profile_Option_UI, {
        { GW2_SCT::LanguageKey::Profile_Description, "Profiles store all your other SCT options and can be quickly changed in this tab. You can either define which profile is used for the in game character you are currently playing or otherwise the profile selectected as master profile will be loaded." },
        { GW2_SCT::LanguageKey::Master_Profile, "Master Profile" },
        { GW2_SCT::LanguageKey::Character_Specific_Profile_Heading, "Character specific profile:" },
        { GW2_SCT::LanguageKey::Character_Specific_Profile_Enabled, "enable specific profile for " },
        { GW2_SCT::LanguageKey::Current_Profile_Heading, "Current profile information:" },
        { GW2_SCT::LanguageKey::Current_Profile_Name, "Profile Name" },
        { GW2_SCT::LanguageKey::Create_Profile_Copy, "Create a Copy" },
        { GW2_SCT::LanguageKey::Profile_Copy_Suffix, "Copy" },
        { GW2_SCT::LanguageKey::Delete_Profile, "Delete this profile" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Title, "Delete Profile?" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Content, "Are you sure you want to delete this profile filter? All options set within will be lost." },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Confirmation, "OK" },
        { GW2_SCT::LanguageKey::Delete_Confirmation_Cancel, "Cancel" },
    } },
    { GW2_SCT::LanguageCategory::Message_General, {
        { GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
        { GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
        { GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
        { GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
        { GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
        { GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
        { GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
        { GW2_SCT::LanguageKey::Default_Value, "%v %i" },
    } },
    { GW2_SCT::LanguageCategory::Message_Category_Player_In, {
        { GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
    } },
    { GW2_SCT::LanguageCategory::Message_Category_Pet_In, {
        { GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
        { GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone being attacked" },
        { GW2_SCT::LanguageKey::Default_Value, "%i Pet -%v" },
    } },
    { GW2_SCT::LanguageCategory::Message_Type_Heal, {
        { GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of heal" },
        { GW2_SCT::LanguageKey::Default_Value, "+%v %i" },
    } },
    { GW2_SCT::LanguageCategory::Message_Type_HoT, {
        { GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of heal" },
        { GW2_SCT::LanguageKey::Default_Value, "+%v %i" },
    } },
    { GW2_SCT::LanguageCategory::Message_Type_Shield_Got, {
        { GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of shield" },
        { GW2_SCT::LanguageKey::Default_Value, "+%v %i" },
    } },
    { GW2_SCT::LanguageCategory::Message_Type_Shield, {
        { GW2_SCT::LanguageKey::Default_Value, "%v -=absorb=- %i" },
    } },
    { GW2_SCT::LanguageCategory::Message_Type_Block, {
        { GW2_SCT::LanguageKey::Default_Value, "Block! %i" },
    } },
    { GW2_SCT::LanguageCategory::Message_Type_Evade, {
        { GW2_SCT::LanguageKey::Default_Value, "Evade! %i" },
    } },
    { GW2_SCT::LanguageCategory::Message_Type_Invulnerable, {
        { GW2_SCT::LanguageKey::Default_Value, "Invulnerable! %i" },
    } },
    { GW2_SCT::LanguageCategory::Message_Type_Miss, {
        { GW2_SCT::LanguageKey::Default_Value, "Miss! %i" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Physical, {
        { GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=FFFFFF]%v[/col]" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Crit, {
        { GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=FFFFFF]%v[/col]" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Bleeding, {
        { GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=E84B30]%v[/col]" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Burning, {
        { GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=FF9E32]%v[/col]" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Poison, {
        { GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=00C400]%v[/col]" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Confusion, {
        { GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=B243FF]%v[/col]" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Retaliation, {
        { GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=FFED00]%v[/col]" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Torment, {
        { GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=24451F]%v[/col]" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_DoT, {
        { GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=45CDFF]%v[/col]" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Heal, {
        { GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the healing source" },
        { GW2_SCT::LanguageKey::Default_Value, "%i +%v" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_HoT, {
        { GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the healing source" },
        { GW2_SCT::LanguageKey::Default_Value, "%i +%v" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Shield_Got, {
        { GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the shield source" },
        { GW2_SCT::LanguageKey::Default_Value, "%i +%v" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Shield, {
        { GW2_SCT::LanguageKey::Default_Value, "%i %v -=absorb=-" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Block, {
        { GW2_SCT::LanguageKey::Default_Value, "%i Block!" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Evade, {
        { GW2_SCT::LanguageKey::Default_Value, "%i Evade!" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Invulnerable, {
        { GW2_SCT::LanguageKey::Default_Value, "%i Invulnerable!" },
    } },
    { GW2_SCT::LanguageCategory::Message_Player_In_Miss, {
        { GW2_SCT::LanguageKey::Default_Value, "%i Miss!" },
    } },
    { GW2_SCT::LanguageCategory::Message_Pet_Out_Heal, {
        { GW2_SCT::LanguageKey::Parameter_Description_v, "Name of the pet/clone healing" },
    } },
    { GW2_SCT::LanguageCategory::Message_Pet_Out_HoT, {
        { GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone healing" },
    } },
    { GW2_SCT::LanguageCategory::Message_Pet_Out_Shield_Got, {
        { GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone shielding" },
    } },
    { GW2_SCT::LanguageCategory::Message_Pet_In_Heal, {
        { GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the healing source" },
        { GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone being healed" },
        { GW2_SCT::LanguageKey::Default_Value, "%i Pet +%v" },
    } },
    { GW2_SCT::LanguageCategory::Message_Pet_In_HoT, {
        { GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the healing source" },
        { GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone being healed" },
        { GW2_SCT::LanguageKey::Default_Value, "%i Pet +%v" },
    } },
    { GW2_SCT::LanguageCategory::Message_Pet_In_Shield_Got, {
        { GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the shield source" },
        { GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone being shielded" },
        { GW2_SCT::LanguageKey::Default_Value, "%i Pet +%v" },
    } },
    { GW2_SCT::LanguageCategory::Message_Pet_In_Shield, {
        { GW2_SCT::LanguageKey::Default_Value, "%i Pet %v -=absorb=-" },
    } },
    { GW2_SCT::LanguageCategory::Message_Pet_In_Block, {
        { GW2_SCT::LanguageKey::Default_Value, "%i Pet Block!" },
    } },
    { GW2_SCT::LanguageCategory::Message_Pet_In_Evade, {
        { GW2_SCT::LanguageKey::Default_Value, "%i Pet Evade!" },
    } },
    { GW2_SCT::LanguageCategory::Message_Pet_In_Invulnerable, {
        { GW2_SCT::LanguageKey::Default_Value, "%i Pet Invulnerable!" },
    } },
    { GW2_SCT::LanguageCategory::Message_Pet_In_Miss, {
        { GW2_SCT::LanguageKey::Default_Value, "%i Pet Miss!" },
    } },
});

void replace(std::string& str, const std::string& find, const std::string& replace) {
	std::size_t position = 0;
	while ((position = str.find(find, position)) != std::string::npos) {
		str.erase(position, find.size());
		str.insert(position, replace);
		position += replace.size();
	}
}

void GW2_SCT::Language::load() {
	if (!loadSuccessful) {
		std::string filename = getSCTPath() + "lang.json";
		if (file_exist(filename)) {
			LOG("Loading lang.json");
			std::string line, text;
			std::ifstream in(filename);
			while (std::getline(in, line)) {
				text += line + "\n";
			}
			in.close();
			bool needsSaving = false;
			try {
				loadedLanguage = nlohmann::json::parse(text);

                for (auto& catPair : loadedLanguage.items()) {
                    LanguageCategory category = GetLanguageCategoryValue(catPair.key().c_str());
                    if (category != LanguageCategory::No_Category) {
                        for (auto& keyPair : catPair.value().items()) {
                            LanguageKey key = GetLanguageKeyValue(keyPair.key().c_str());
                            if (key != LanguageKey::No_Key) {
                                if (languageStrings[category].find(key) != languageStrings[category].end()) {
                                    languageStrings[category][key].setValue(keyPair.value());
                                } else {
                                    languageStrings[category].insert(std::pair<LanguageKey, LanguageEntry>(key, LanguageEntry((std::string)keyPair.value())));
                                }
                            }
                        }
                    }
                }

                for (auto& category : languageStrings) {
                    for (auto& key : category.second) {
                        if (!key.second.isOptional() && !key.second.resolve()) {
                            needsSaving = true;
                        }
                    }
                }
			} catch (std::exception& e) {
				needsSaving = true;
			}
			if (needsSaving) {
				LOG("Saving a new lang.json file since some needed elements were not found.");
				saveDefault(filename.c_str());
			}
		} else {
			saveDefault(filename.c_str());
		}
		for (auto category : languageStrings) {
			for (auto key : category.second) {
				std::string str(key.second.operator std::string());
				replace(str, "%", "%%");
				languageStringsImguiSafe[category.first][key.first] = str;
			}
		}
		loadSuccessful = true;
	}
}

void GW2_SCT::Language::reload() {
	loadSuccessful = false;
	load();
}

const char* GW2_SCT::Language::get(GW2_SCT::LanguageKey key, bool imgui_safe) {
	return get(GW2_SCT::LanguageCategory::General, key, imgui_safe);
}

const char* GW2_SCT::Language::get(GW2_SCT::LanguageCategory section, GW2_SCT::LanguageKey key, bool imgui_safe) {
	if (!loadSuccessful) load();
    auto languageMapToUse = imgui_safe ? &languageStringsImguiSafe : &languageStrings;
    auto sectionIt = languageMapToUse->find(section);
	if (sectionIt == languageMapToUse->end()) {
		return "Missing a language section";
	}
    auto keyIt = sectionIt->second.find(key);
	if (keyIt == sectionIt->second.end()) {
		return "Missing a language key";
	}
	return keyIt->second;
}

void GW2_SCT::Language::saveDefault(const char* filename) {
	std::ofstream out(filename);

    for (auto& catPair : defaultLanguage.items()) {
        for (auto& keyPair : catPair.value().items()) {
            if (loadedLanguage[catPair.key()].find(keyPair.key()) == loadedLanguage[catPair.key()].end()) {
                loadedLanguage[catPair.key()][keyPair.key()] = keyPair.value();
            }
        }
    }

    for (auto& catPair : loadedLanguage.items()) {
        LanguageCategory category = GetLanguageCategoryValue(catPair.key().c_str());
        if (category != LanguageCategory::No_Category) {
            for (auto& keyPair : catPair.value().items()) {
                LanguageKey key = GetLanguageKeyValue(keyPair.key().c_str());
                if (key != LanguageKey::No_Key) {
                    if (languageStrings[category].find(key) != languageStrings[category].end()) {
                        languageStrings[category][key].setValue(keyPair.value());
                    }
                    else {
                        languageStrings[category].insert(std::pair<LanguageKey, LanguageEntry>(key, LanguageEntry((std::string)keyPair.value())));
                    }
                }
            }
        }
    }
    
    for (auto& category : languageStrings) {
        for (auto& key : category.second) {
            if (!key.second.isOptional() && !key.second.resolve()) {
                LOG("Could not correctly resolve language entry after setting default: ", GetString(category.first), " -> ", GetString(key.first));
            }
        }
    }

	out << loadedLanguage.dump(2);
	out.close();
}
