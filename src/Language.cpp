#include "Language.h"

namespace GW2_SCT {
	DEFINE_ENUM(LanguageCategory, LANGUAGE_CATEGORY)
	DEFINE_ENUM(LanguageKey, LANGUAGE_KEY)
};

CSimpleIniA GW2_SCT::Language::languageFile(true, false, false);
bool GW2_SCT::Language::loadSuccessful = false;
std::map<GW2_SCT::LanguageCategory, std::map<GW2_SCT::LanguageKey, const char*>> GW2_SCT::Language::languageStringsImguiSafe = {};
std::map<GW2_SCT::LanguageCategory, std::map<GW2_SCT::LanguageKey, const char*>> GW2_SCT::Language::languageStrings = {
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
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingPhysical, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingCrit, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingBleeding, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingBurning, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingPoison, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingConfusion, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingRetaliation, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingTorment, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingDoT, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingHeal, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of heal" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "+%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingHoT, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of heal" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "+%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingShieldGot, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of shield" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "+%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingShield, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v -=absorb=- %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingBlock, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "Block! %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingEvade, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "Evade! %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingInvulnerable, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "Invulnerable! %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerOutgoingMiss, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "Miss! %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingPhysical, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=FFFFFF]%v[/col]" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingCrit, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=FFFFFF]%v[/col]" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingBleeding, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=E84B30]%v[/col]" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingBurning, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=FF9E32]%v[/col]" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingPoison, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=00C400]%v[/col]" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingConfusion, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=B243FF]%v[/col]" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingRetaliation, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=FFED00]%v[/col]" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingTorment, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=24451F]%v[/col]" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingDoT, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i ([col=%c]%n[/col]) -[col=45CDFF]%v[/col]" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingHeal, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of heal" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the healing source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i +%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingHoT, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of heal" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the healing source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i +%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingShieldGot, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the shield source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i +%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingShield, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i %v -=absorb=-" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingBlock, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Block!" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingEvade, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Evade!" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingInvulnerable, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Invulnerable!" },
	} },
	{ GW2_SCT::LanguageCategory::messagePlayerIncomingMiss, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Miss!" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingPhysical, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingCrit, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingBleeding, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingBurning, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingPoison, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingConfusion, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingRetaliation, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingTorment, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingDoT, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingHeal, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of heal" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone healing" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "+%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingHoT, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of heal" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone healing" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "+%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingShieldGot, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of shield" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone shielding" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "+%v %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingShield, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%v -=absorb=- %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingBlock, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "Block! %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingEvade, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "Evade! %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingInvulnerable, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "Invulnerable! %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetOutgoingMiss, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the skills target" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "Miss! %i" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingPhysical, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet -%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingCrit, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet -%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingBleeding, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet -%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingBurning, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet -%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingPoison, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet -%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingConfusion, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet -%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingRetaliation, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet -%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingTorment, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet -%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingDoT, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet -%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingHeal, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of healing" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the healing source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone healing" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet +%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingHoT, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of healing" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the healing source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone healing" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet +%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingShieldGot, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of shield" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the shield source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet +%v" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingShield, {
		{ GW2_SCT::LanguageKey::Parameter_Description_v, "Amount of damage" },
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet %v -=absorb=-" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingBlock, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet Block!" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingEvade, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet Evade!" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingInvulnerable, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet Invulnerable!" },
	} },
	{ GW2_SCT::LanguageCategory::messagePetIncomingMiss, {
		{ GW2_SCT::LanguageKey::Parameter_Description_n, "Name of the attacker" },
		{ GW2_SCT::LanguageKey::Parameter_Description_s, "Skillname" },
		{ GW2_SCT::LanguageKey::Parameter_Description_p, "Name of the pet/clone attacking" },
		{ GW2_SCT::LanguageKey::Parameter_Description_c, "Color for the skills source profession (use: [col=%c]...[/col])" },
		{ GW2_SCT::LanguageKey::Parameter_Description_r, "Profession name of source" },
		{ GW2_SCT::LanguageKey::Parameter_Description_i, "Skillicon" },
		{ GW2_SCT::LanguageKey::Default_Value, "%i Pet Miss!" },
	} }
};

void replace(std::string& str, const std::string& find, const std::string& replace) {
	std::size_t position = 0;
	while ((position = str.find(find, position)) != std::string::npos) {
		str.erase(position, find.size());
		str.insert(position, replace);
		position += replace.size();
	}
}

static inline void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
}

void GW2_SCT::Language::load() {
	if (!loadSuccessful) {
		std::string filename = getSCTPath() + "lang.ini";
		SI_Error rc = languageFile.LoadFile(filename.c_str());
		if (rc < 0) {
			saveDefault(filename.c_str());
		}
		else {
			bool needsSaving = false;
			for (auto category : languageStrings) {
				for (auto key : category.second) {
					const char* val = languageFile.GetValue(GetString(category.first), GetString(key.first));
					if (val == nullptr) {
						needsSaving = true;
					}
					else {
						std::string str(val);
						replace(str, "\\n", "\n");
						languageStrings[category.first][key.first] = _strdup(str.c_str());
					}
				}
			}
			if (needsSaving) {
				LOG("Saving a new lang.ini file since some needed elements were not found.");
				saveDefault(filename.c_str());
			}
		}
		for (auto category : languageStrings) {
			if (languageStringsImguiSafe.count(category.first) == 0) languageStringsImguiSafe[category.first] = {};
			for (auto key : category.second) {
				std::string str(languageStrings[category.first][key.first]);
				replace(str, "%", "%%");
				languageStringsImguiSafe[category.first][key.first] = _strdup(str.c_str());
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
	if (imgui_safe) {
		if (languageStringsImguiSafe.count(section) == 0) {
			return "Missing a language section";
		}
		if (languageStringsImguiSafe[section].count(key) == 0) {
			return "Missing a language key";
		}
		return languageStringsImguiSafe[section][key];
	}
	else {
		if (languageStrings.count(section) == 0) {
			return "Missing a language section";
		}
		if (languageStrings[section].count(key) == 0) {
			return "Missing a language key";
		}
		return languageStrings[section][key];
	}
}

void GW2_SCT::Language::saveDefault(const char* filename) {
	std::stringstream sstream;
	for (auto category : languageStrings) {
		sstream << "[" << GetString(category.first) << "]" << std::endl;
		for (auto key : category.second) {
			std::string str(key.second);
			replace(str, "\n", "\\n");
			sstream << GetString(key.first) << " = " << str << std::endl;
		}
	}
	languageFile.LoadData(sstream.str().c_str());
	languageFile.SaveFile(filename);
}
