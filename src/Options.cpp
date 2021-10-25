#include "Options.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <regex>
#include "json.hpp"
#include "Common.h"
#include "SimpleIni.h"
#include "imgui.h"
#include "imgui_sct_widgets.h"
#include "TemplateInterpreter.h"
#include "Language.h"

const char* TextAlignTexts[] = { langStringG(GW2_SCT::LanguageKey::Text_Align_Left), langStringG(GW2_SCT::LanguageKey::Text_Align_Center), langStringG(GW2_SCT::LanguageKey::Text_Align_Right) };
const char* TextCurveTexts[] = { langStringG(GW2_SCT::LanguageKey::Text_Curve_Left), langStringG(GW2_SCT::LanguageKey::Text_Curve_Straight), langStringG(GW2_SCT::LanguageKey::Text_Curve_Right) };

GW2_SCT::options_struct GW2_SCT::Options::opt;
bool GW2_SCT::Options::windowIsOpen = false;
std::string GW2_SCT::Options::fontSelectionString = "";
std::string GW2_SCT::Options::fontSelectionStringWithMaster = "";
std::string GW2_SCT::Options::fontSizeTypeSelectionString = "";
std::string GW2_SCT::Options::skillFilterTypeSelectionString = "";
std::string GW2_SCT::Options::backupFileName = "";

const std::map<GW2_SCT::MessageCategory, std::string> messageCategorySections = {
	{ GW2_SCT::MessageCategory::PLAYER_OUT, "Messages_Player_Out" },
	{ GW2_SCT::MessageCategory::PLAYER_IN, "Messages_Player_In" },
	{ GW2_SCT::MessageCategory::PET_OUT, "Messages_Pet_Out" },
	{ GW2_SCT::MessageCategory::PET_IN, "Messages_Pet_In" }
};

#define P99_PROTECT(...) __VA_ARGS__
#define ReceiverInformation(Category, CategoryLanguage, Type, TypeLanguage, IniName, CategoryAndTypeLanguage, PossibileParameters, Enabled, Color, Font, FontSize) \
		{ Type, { \
			IniName, \
			mapParameterListToLanguage(#CategoryAndTypeLanguage, PossibileParameters), \
			message_receiver_options_struct{ \
				std::string(langString(LanguageCategory::Option_UI, LanguageKey::CategoryLanguage)) + " - " + std::string(langString(LanguageCategory::Option_UI, LanguageKey::TypeLanguage)), \
				Category, \
				Type, \
				Enabled, \
				std::string(langString(LanguageCategory::CategoryAndTypeLanguage, LanguageKey::Default_Value)), \
				std::string(Color), \
				Font, \
				FontSize \
			} \
		} }

namespace GW2_SCT {
	const std::map<MessageCategory, const std::map<MessageType, receiver_information>> receiverInformationPerCategoryAndType = {
		{ MessageCategory::PLAYER_OUT, {
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::PHYSICAL, Messages_Type_Physical, "Message_Player_Outgoing_Physical", messagePlayerOutgoingPhysical, P99_PROTECT({'v', 'n', 's', 'i'}), true, "FFFFFF", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::CRIT, Messages_Type_Crit, "Message_Player_Outgoing_Crit", messagePlayerOutgoingCrit, P99_PROTECT({ 'v', 'n', 's', 'i' }), true, "FFFFFF", 0, -2.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::BLEEDING, Messages_Type_Bleeding, "Message_Player_Outgoing_Bleeding", messagePlayerOutgoingBleeding, P99_PROTECT({ 'v', 'n', 's', 'i' }), true, "E84B30", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::BURNING, Messages_Type_Burning, "Message_Player_Outgoing_Burning", messagePlayerOutgoingBurning, P99_PROTECT({ 'v', 'n', 's', 'i' }), true, "FF9E32", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::POISON, Messages_Type_Poison, "Message_Player_Outgoing_Poison", messagePlayerOutgoingPoison, P99_PROTECT({ 'v', 'n', 's', 'i' }), true, "00C400", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::CONFUSION, Messages_Type_Confusion, "Message_Player_Outgoing_Confusion", messagePlayerOutgoingConfusion, P99_PROTECT({ 'v', 'n', 's', 'i' }), true, "B243FF", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::RETALIATION, Messages_Type_Retaliation, "Message_Player_Outgoing_Retaliation", messagePlayerOutgoingRetaliation, P99_PROTECT({ 'v', 'n', 's', 'i' }), true, "FFED00", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::TORMENT, Messages_Type_Torment, "Message_Player_Outgoing_Torment", messagePlayerOutgoingTorment, P99_PROTECT({ 'v', 'n', 's', 'i' }), true, "24451F", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::DOT, Messages_Type_Dot, "Message_Player_Outgoing_DoT", messagePlayerOutgoingDoT, P99_PROTECT({ 'v', 'n', 's', 'i' }), true, "45CDFF", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::HEAL, Messages_Type_Heal, "Message_Player_Outgoing_Heal", messagePlayerOutgoingHeal, P99_PROTECT({ 'v', 'n', 's', 'i' }), true, "10FB10", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::HOT, Messages_Type_Hot, "Message_Player_Outgoing_HoT", messagePlayerOutgoingHoT, P99_PROTECT({ 'v', 'n', 's', 'i' }), true, "10FB10", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::SHIELD_RECEIVE, Messages_Type_Shield_Receive, "Message_Player_Outgoing_Shield_Got", messagePlayerOutgoingShieldGot, P99_PROTECT({ 'v', 'n', 's', 'i' }), true, "FFFF00", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::SHIELD_REMOVE, Messages_Type_Shield_Remove, "Message_Player_Outgoing_Shield", messagePlayerOutgoingShield, P99_PROTECT({ 'v', 'n', 's', 'i' }), true, "FFFF00", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::BLOCK, Messages_Type_Block, "Message_Player_Outgoing_Block", messagePlayerOutgoingBlock, P99_PROTECT({ 'n', 's', 'i' }), true, "0000FF", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::EVADE, Messages_Type_Evade, "Message_Player_Outgoing_Evade", messagePlayerOutgoingEvade, P99_PROTECT({ 'n', 's', 'i' }), true, "0000FF", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::INVULNERABLE, Messages_Type_Invulnerable, "Message_Player_Outgoing_Invulnerable", messagePlayerOutgoingInvulnerable, P99_PROTECT({ 'n', 's', 'i' }), true, "0000FF", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_OUT, Messages_Category_Player_Out, MessageType::MISS, Messages_Type_Miss, "Message_Player_Outgoing_Miss", messagePlayerOutgoingMiss, P99_PROTECT({ 'n', 's', 'i' }), true, "0000FF", 0, -1.f)
		} },
		{ MessageCategory::PLAYER_IN, {
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::PHYSICAL, Messages_Type_Physical, "Message_Player_Incoming_Physical", messagePlayerIncomingPhysical, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "FF0000", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::CRIT, Messages_Type_Crit, "Message_Player_Incoming_Crit", messagePlayerIncomingCrit, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "FF0000", 0, -2.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::BLEEDING, Messages_Type_Bleeding, "Message_Player_Incoming_Bleeding", messagePlayerIncomingBleeding, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "FF0000", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::BURNING, Messages_Type_Burning, "Message_Player_Incoming_Burning", messagePlayerIncomingBurning, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "FF0000", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::POISON, Messages_Type_Poison, "Message_Player_Incoming_Poison", messagePlayerIncomingPoison, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "FF0000", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::CONFUSION, Messages_Type_Confusion, "Message_Player_Incoming_Confusion", messagePlayerIncomingConfusion, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "FF0000", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::RETALIATION, Messages_Type_Retaliation, "Message_Player_Incoming_Retaliation", messagePlayerIncomingRetaliation, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "FF0000", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::TORMENT, Messages_Type_Torment, "Message_Player_Incoming_Torment", messagePlayerIncomingTorment, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "FF0000", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::DOT, Messages_Type_Dot, "Message_Player_Incoming_DoT", messagePlayerIncomingDoT, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "FF0000", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::HEAL, Messages_Type_Heal, "Message_Player_Incoming_Heal", messagePlayerIncomingHeal, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "10FB10", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::HOT, Messages_Type_Hot, "Message_Player_Incoming_HoT", messagePlayerIncomingHoT, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "10FB10", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::SHIELD_RECEIVE, Messages_Type_Shield_Receive, "Message_Player_Incoming_Shield_Got", messagePlayerIncomingShieldGot, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "FFFF00", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::SHIELD_REMOVE, Messages_Type_Shield_Remove, "Message_Player_Incoming_Shield", messagePlayerIncomingShield, P99_PROTECT({ 'v', 'n', 's', 'c', 'r', 'i' }), true, "FFFF00", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::BLOCK, Messages_Type_Block, "Message_Player_Incoming_Block", messagePlayerIncomingBlock, P99_PROTECT({ 'n', 's', 'c', 'r', 'i' }), true, "0000FF", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::EVADE, Messages_Type_Evade, "Message_Player_Incoming_Evade", messagePlayerIncomingEvade, P99_PROTECT({ 'n', 's', 'c', 'r', 'i' }), true, "0000FF", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::INVULNERABLE, Messages_Type_Invulnerable, "Message_Player_Incoming_Invulnerable", messagePlayerIncomingInvulnerable, P99_PROTECT({ 'n', 's', 'c', 'r', 'i' }), true, "0000FF", 0, -1.f),
			ReceiverInformation(MessageCategory::PLAYER_IN, Messages_Category_Player_In, MessageType::MISS, Messages_Type_Miss, "Message_Player_Incoming_Miss", messagePlayerIncomingMiss, P99_PROTECT({ 'n', 's', 'c', 'r', 'i' }), true, "0000FF", 0, -1.f)
		} },
		{ MessageCategory::PET_OUT, {
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::PHYSICAL, Messages_Type_Physical, "Message_Pet_Outgoing_Physical", messagePetOutgoingPhysical, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::CRIT, Messages_Type_Crit, "Message_Pet_Outgoing_Crit", messagePetOutgoingCrit, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), true, "80FB80", 0, -2.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::BLEEDING, Messages_Type_Bleeding, "Message_Pet_Outgoing_Bleeding", messagePetOutgoingBleeding, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::BURNING, Messages_Type_Burning, "Message_Pet_Outgoing_Burning", messagePetOutgoingBurning, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::POISON, Messages_Type_Poison, "Message_Pet_Outgoing_Poison", messagePetOutgoingPoison, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::CONFUSION, Messages_Type_Confusion, "Message_Pet_Outgoing_Confusion", messagePetOutgoingConfusion, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::RETALIATION, Messages_Type_Retaliation, "Message_Pet_Outgoing_Retaliation", messagePetOutgoingRetaliation, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::TORMENT, Messages_Type_Torment, "Message_Pet_Outgoing_Torment", messagePetOutgoingTorment, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::DOT, Messages_Type_Dot, "Message_Pet_Outgoing_DoT", messagePetOutgoingDoT, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::HEAL, Messages_Type_Heal, "Message_Pet_Outgoing_Heal", messagePetOutgoingHeal, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::HOT, Messages_Type_Hot, "Message_Pet_Outgoing_HoT", messagePetOutgoingHoT, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::SHIELD_RECEIVE, Messages_Type_Shield_Receive, "Message_Pet_Outgoing_Shield_Got", messagePetOutgoingShieldGot, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), true, "FFFF00", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::SHIELD_REMOVE, Messages_Type_Shield_Remove, "Message_Pet_Outgoing_Shield", messagePetOutgoingShield, P99_PROTECT({ 'v', 'n', 's', 'p', 'i' }), false, "FFFF00", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::BLOCK, Messages_Type_Block, "Message_Pet_Outgoing_Block", messagePetOutgoingBlock, P99_PROTECT({ 'n', 's', 'p', 'i' }), false, "0000FF", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::EVADE, Messages_Type_Evade, "Message_Pet_Outgoing_Evade", messagePetOutgoingEvade, P99_PROTECT({ 'n', 's', 'p', 'i' }), false, "0000FF", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::INVULNERABLE, Messages_Type_Invulnerable, "Message_Pet_Outgoing_Invulnerable", messagePetOutgoingInvulnerable, P99_PROTECT({ 'n', 's', 'p', 'i' }), false, "0000FF", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_OUT, Messages_Category_Pet_Out, MessageType::MISS, Messages_Type_Miss, "Message_Pet_Outgoing_Miss", messagePetOutgoingMiss, P99_PROTECT({ 'n', 's', 'p', 'i' }), false, "0000FF", 0, -1.f)
		} },
		{ MessageCategory::PET_IN, {
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::PHYSICAL, Messages_Type_Physical, "Message_Pet_Incoming_Physical", messagePetIncomingPhysical, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::CRIT, Messages_Type_Crit, "Message_Pet_Incoming_Crit", messagePetIncomingCrit, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -2.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::BLEEDING, Messages_Type_Bleeding, "Message_Pet_Incoming_Bleeding", messagePetIncomingBleeding, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::BURNING, Messages_Type_Burning, "Message_Pet_Incoming_Burning", messagePetIncomingBurning, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::POISON, Messages_Type_Poison, "Message_Pet_Incoming_Poison", messagePetIncomingPoison, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::CONFUSION, Messages_Type_Confusion, "Message_Pet_Incoming_Confusion", messagePetIncomingConfusion, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::RETALIATION, Messages_Type_Retaliation, "Message_Pet_Incoming_Retaliation", messagePetIncomingRetaliation, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::TORMENT, Messages_Type_Torment, "Message_Pet_Incoming_Torment", messagePetIncomingTorment, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::DOT, Messages_Type_Dot, "Message_Pet_Incoming_DoT", messagePetIncomingDoT, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::HEAL, Messages_Type_Heal, "Message_Pet_Incoming_Heal", messagePetIncomingHeal, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::HOT, Messages_Type_Hot, "Message_Pet_Incoming_HoT", messagePetIncomingHoT, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::SHIELD_RECEIVE, Messages_Type_Shield_Receive, "Message_Pet_Incoming_Shield_Got", messagePetIncomingShieldGot, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), true, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::SHIELD_REMOVE, Messages_Type_Shield_Remove, "Message_Pet_Incoming_Shield", messagePetIncomingShield, P99_PROTECT({ 'v', 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::BLOCK, Messages_Type_Block, "Message_Pet_Incoming_Block", messagePetIncomingBlock, P99_PROTECT({ 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::EVADE, Messages_Type_Evade, "Message_Pet_Incoming_Evade", messagePetIncomingEvade, P99_PROTECT({ 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::INVULNERABLE, Messages_Type_Invulnerable, "Message_Pet_Incoming_Invulnerable", messagePetIncomingInvulnerable, P99_PROTECT({ 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f),
			ReceiverInformation(MessageCategory::PET_IN, Messages_Category_Pet_In, MessageType::MISS, Messages_Type_Miss, "Message_Pet_Incoming_Miss", messagePetIncomingMiss, P99_PROTECT({ 'n', 's', 'p', 'c', 'r', 'i' }), false, "80FB80", 0, -1.f)
		} }
	};


	const std::map<MessageCategory, std::string> categoryNames = {
		{ MessageCategory::PLAYER_OUT, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Category_Player_Out)) },
		{ MessageCategory::PLAYER_IN, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Category_Player_In)) },
		{ MessageCategory::PET_OUT, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Category_Pet_Out)) },
		{ MessageCategory::PET_IN, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Category_Pet_In)) }
	};

	const std::map<MessageType, std::string> typeNames = {
			{ MessageType::PHYSICAL, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Physical)) },
			{ MessageType::CRIT, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Crit)) },
			{ MessageType::BLEEDING, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Bleeding)) },
			{ MessageType::BURNING, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Burning)) },
			{ MessageType::POISON, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Poison)) },
			{ MessageType::CONFUSION, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Confusion)) },
			{ MessageType::RETALIATION, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Retaliation)) },
			{ MessageType::TORMENT, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Torment)) },
			{ MessageType::DOT, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Dot)) },
			{ MessageType::HEAL, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Heal)) },
			{ MessageType::HOT, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Hot)) },
			{ MessageType::SHIELD_RECEIVE, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Shield_Receive)) },
			{ MessageType::SHIELD_REMOVE, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Shield_Remove)) },
			{ MessageType::BLOCK, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Block)) },
			{ MessageType::EVADE, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Evade)) },
			{ MessageType::INVULNERABLE, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Invulnerable)) },
			{ MessageType::MISS, std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Messages_Type_Miss)) }
	};
}

void GW2_SCT::Options::open() {
	windowIsOpen = true;
}

bool GW2_SCT::Options::getIsSkillFiltered(uint32_t skillId, const char* skillName) {
	for (const auto& it : opt.skillFilters) {
		if (it.type == FilterType::SKILL_ID) {
			if (it.skillId == skillId) {
				return true;
			}
		}
		else if (it.type == FilterType::SKILL_NAME) {
			if (std::strcmp(it.skillName.c_str(), skillName) == 0) {
				return true;
			}
		}
	}
	return false;
}

void GW2_SCT::Options::paint() {
	if (opt.revision.compare(SCT_VERSION_STRING) != 0) {
		ImGui::OpenPopup(langStringG(LanguageKey::Old_Save_Popup_Title));
	}
	if (ImGui::BeginPopupModal(langStringG(LanguageKey::Old_Save_Popup_Title))) {
		std::string s(langStringG(LanguageKey::Old_Save_Popup_Content));
		if (backupFileName != "") {
			s += std::string(langStringG(LanguageKey::Old_Save_Popup_Backup_Made)) + "\n" + backupFileName + "\n";
		}
		s += "\n";
		ImGui::Text(s.c_str());
		ImGui::Separator();
		if (ImGui::Button(langStringG(LanguageKey::Old_Save_Popup_Confirmation), ImVec2(120, 0))) {
			opt.revision = SCT_VERSION_STRING;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (windowIsOpen) {
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.60f, 0.60f, 0.60f, 0.30f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
		ImGui::Begin(langString(LanguageCategory::Option_UI, LanguageKey::Title), &windowIsOpen, ImGuiWindowFlags_NoCollapse);

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("##options-tab-bar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem(langString(LanguageCategory::Option_UI, LanguageKey::Menu_Bar_General))) {
				paintGeneral();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(langString(LanguageCategory::Option_UI, LanguageKey::Menu_Bar_Scroll_Areas))) {
				paintScrollAreas();
				if (!windowIsOpen) {
					for (auto scrollAreaOptions : opt.scrollAreaOptions) {
						if (scrollAreaOptions->outlineState != ScrollAreaOutlineState::NONE) scrollAreaOptions->outlineState = ScrollAreaOutlineState::NONE;
					}
				}
				ImGui::EndTabItem();
			} else {
				for (auto scrollAreaOptions : opt.scrollAreaOptions) {
					if (scrollAreaOptions->outlineState != ScrollAreaOutlineState::NONE) scrollAreaOptions->outlineState = ScrollAreaOutlineState::NONE;
				}
			}
			if (ImGui::BeginTabItem(langString(LanguageCategory::Option_UI, LanguageKey::Menu_Bar_Profession_Colors))) {
				paintProfessionColors();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(langString(LanguageCategory::Option_UI, LanguageKey::Menu_Bar_Filtered_Skills))) {
				paintSkillFilters();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(langString(LanguageCategory::Option_UI, LanguageKey::Menu_Bar_Skill_Icons))) {
				paintSkillIcons();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}
}

std::string btos(bool b) {
	return b ? "1" : "0";
}

bool stob(std::string const& s) {
	return s != "0";
}

void GW2_SCT::Options::save() {
	std::ofstream out((getSCTPath() + "sct.json").c_str());
	nlohmann::json j = opt;
#if _DEBUG
	out << j.dump(4);
#else
	out << j;
#endif
	out.close();
}

void GW2_SCT::Options::load() {
	fontSelectionString = "";
	for (std::map<int, std::pair<std::string, FontType*>>::iterator it = fontMap.begin(); it != fontMap.end(); ++it) {
		fontSelectionString += '\0' + it->second.first;
	}
	fontSelectionString += '\0';
	fontSelectionStringWithMaster = std::string("Master Font" + fontSelectionString);
	fontSelectionString.erase(fontSelectionString.begin());

	fontSizeTypeSelectionString = std::string(langStringG(LanguageKey::Default_Font_Size)) + '\0' + std::string(langStringG(LanguageKey::Default_Crit_Font_Size)) + '\0' + std::string(langStringG(LanguageKey::Custom_Font_Size))  + '\0';
	skillFilterTypeSelectionString = std::string(langStringG(LanguageKey::Skill_Filter_Type_ID)) + '\0' + std::string(langStringG(LanguageKey::Skill_Filter_Type_Name)) + '\0';

	std::string iniFilename = getSCTPath() + "sct.ini";
	std::string jsonFilename = getSCTPath() + "sct.json";
	if (file_exist(iniFilename)) {
		LOG("Loading sct.ini");
		CSimpleIniA loadedIni(true, false, false);
		SI_Error rc = loadedIni.LoadFile(iniFilename.c_str());
		if (rc < 0) {
			setDefault();
			return;
		}
		else {
			opt.revision = loadedIni.GetValue("General", "Revision", "1.0");

			if (opt.revision.compare(SCT_VERSION_STRING) != 0) {
				backupFileName = getSCTPath() + "sct_" + opt.revision + ".ini";
				if (!MoveFile(iniFilename.c_str(), backupFileName.c_str())) {
					backupFileName = "";
				}
			}

			if (std::regex_match(opt.revision, std::regex("1\\.[0-2][a-z]?"))) {
				LOG("Updating config for versions before 1.3.");
				for (auto categoryIterator = receiverInformationPerCategoryAndType.begin(); categoryIterator != receiverInformationPerCategoryAndType.end(); categoryIterator++) {
					const char* section = messageCategorySections.at(categoryIterator->first).c_str();
					for (auto typeIterator = categoryIterator->second.begin(); typeIterator != categoryIterator->second.end(); typeIterator++) {
						float font_size = std::stof(loadedIni.GetValue(section, (typeIterator->second.iniPrefix + "_Font_Size").c_str(), std::to_string(typeIterator->second.defaultReceiver.fontSize).c_str()));
						if (typeIterator->first == MessageType::CRIT) {
							if (floatEqual(font_size, opt.defaultCritFontSize)) {
								loadedIni.SetValue(section, (typeIterator->second.iniPrefix + "_Font_Size").c_str(), "-2");
							}
						} else {
							if (floatEqual(font_size, opt.defaultFontSize)) {
								loadedIni.SetValue(section, (typeIterator->second.iniPrefix + "_Font_Size").c_str(), "-1");
							}
						}
					}
				}
				opt.revision = "1.3";
			}
		}

		opt.sctEnabled = stob(loadedIni.GetValue("General", "SCT_enabled", btos(opt.sctEnabled).c_str()));
		opt.scrollSpeed = std::stof(loadedIni.GetValue("General", "Scrolling_Speed", std::to_string(opt.scrollSpeed).c_str()));
		opt.dropShadow = stob(loadedIni.GetValue("General", "Drop_Shadow", btos(opt.dropShadow).c_str()));
		opt.messagesInStack = std::stoi(loadedIni.GetValue("General", "Maximal_Messages_in_Stack", std::to_string(opt.messagesInStack).c_str()));
		opt.combineAllMessages = stob(loadedIni.GetValue("General", "Combine_all_Messages", btos(opt.combineAllMessages).c_str()));
		opt.masterFont = stofo(loadedIni.GetValue("General", "Master_Font", fotos(opt.masterFont, false).c_str()), false);
		opt.defaultFontSize = std::stof(loadedIni.GetValue("General", "Default_Font_Size", std::to_string(opt.defaultFontSize).c_str()));
		opt.defaultCritFontSize = std::stof(loadedIni.GetValue("General", "Default_Crit_Font_Size", std::to_string(opt.defaultCritFontSize).c_str()));
		defaultFont = getFontType(opt.masterFont, false);
		opt.selfMessageOnlyIncoming = stob(loadedIni.GetValue("General", "Self_Message_Only_Incoming", btos(opt.selfMessageOnlyIncoming).c_str()));
		opt.outgoingOnlyToTarget = stob(loadedIni.GetValue("General", "Outgoing_Only_To_Target", btos(opt.outgoingOnlyToTarget).c_str()));
		int numScrollAreas = std::stoi(loadedIni.GetValue("General", "Num_Scroll_Areas", "0"));

		for (int i = 1; i <= numScrollAreas; i++) {
			std::string section = "Scroll_Area_" + std::to_string(i);
			auto messageManagerOptions = std::make_shared<scroll_area_options_struct>();
			messageManagerOptions->name = std::string(loadedIni.GetValue(section.c_str(), "Name", "defaultName"));
			messageManagerOptions->offsetX = std::stof(loadedIni.GetValue(section.c_str(), "Horrizontal_Offset", "0"));
			messageManagerOptions->offsetY = std::stof(loadedIni.GetValue(section.c_str(), "Vertical_Offset", "0"));
			messageManagerOptions->width = std::stof(loadedIni.GetValue(section.c_str(), "Width", "0"));
			messageManagerOptions->height = std::stof(loadedIni.GetValue(section.c_str(), "Height", "0"));
			messageManagerOptions->textAlign = (TextAlign)std::stoi(loadedIni.GetValue(section.c_str(), "Text_Align", "0"));
			messageManagerOptions->textCurve = (TextCurve)std::stoi(loadedIni.GetValue(section.c_str(), "Text_Flow", "0"));
			messageManagerOptions->receivers = {};
			opt.scrollAreaOptions.push_back(messageManagerOptions);
		}

		for (auto categoryIterator = receiverInformationPerCategoryAndType.begin(); categoryIterator != receiverInformationPerCategoryAndType.end(); categoryIterator++) {
			const char* section = messageCategorySections.at(categoryIterator->first).c_str();
			for (auto typeIterator = categoryIterator->second.begin(); typeIterator != categoryIterator->second.end(); typeIterator++) {
				int outputArea = std::stoi(loadedIni.GetValue(section, (typeIterator->second.iniPrefix + "_Output_Manager").c_str(), std::to_string(-1).c_str()));
				if (outputArea >= 0 && outputArea <= opt.scrollAreaOptions.size()) {
					message_receiver_options_struct newReceiver{
						typeIterator->second.defaultReceiver.name,
						categoryIterator->first,
						typeIterator->first,
						stob(loadedIni.GetValue(section, (typeIterator->second.iniPrefix + "_Enabled").c_str(), btos(true).c_str())),
						std::string(loadedIni.GetValue(section, (typeIterator->second.iniPrefix + "_Output_Template").c_str(), "")),
						std::string(loadedIni.GetValue(section, (typeIterator->second.iniPrefix + "_Color").c_str(), "#FFFFFF")),
						stofo(loadedIni.GetValue(section, (typeIterator->second.iniPrefix + "_Font").c_str(), fotos(0).c_str())),
						std::stof(loadedIni.GetValue(section, (typeIterator->second.iniPrefix + "_Font_Size").c_str(), std::to_string(22.f).c_str()))
					};
					opt.scrollAreaOptions.at(outputArea - 1)->receivers.push_back(std::make_shared<message_receiver_options_struct>(newReceiver));
				}
			}
		}

		opt.professionColorGuardian = loadedIni.GetValue("Profession_Colors", "Profession_Color_Guardian", opt.professionColorGuardian.c_str());
		opt.professionColorWarrior = loadedIni.GetValue("Profession_Colors", "Profession_Color_Warrior", opt.professionColorWarrior.c_str());
		opt.professionColorEngineer = loadedIni.GetValue("Profession_Colors", "Profession_Color_Engineer", opt.professionColorEngineer.c_str());
		opt.professionColorRanger = loadedIni.GetValue("Profession_Colors", "Profession_Color_Ranger", opt.professionColorRanger.c_str());
		opt.professionColorThief = loadedIni.GetValue("Profession_Colors", "Profession_Color_Thief", opt.professionColorThief.c_str());
		opt.professionColorElementalist = loadedIni.GetValue("Profession_Colors", "Profession_Color_Elementalist", opt.professionColorElementalist.c_str());
		opt.professionColorMesmer = loadedIni.GetValue("Profession_Colors", "Profession_Color_Mesmer", opt.professionColorMesmer.c_str());
		opt.professionColorNecromancer = loadedIni.GetValue("Profession_Colors", "Profession_Color_Necromancer", opt.professionColorNecromancer.c_str());
		opt.professionColorRevenant = loadedIni.GetValue("Profession_Colors", "Profession_Color_Revenant", opt.professionColorRevenant.c_str());
		opt.professionColorDefault = loadedIni.GetValue("Profession_Colors", "Profession_Color_Default", opt.professionColorDefault.c_str());

		int numFilteredIDs = std::stoi(loadedIni.GetValue("Filters", "Num_Filtered_IDs", "0"));
		for (int i = 1; i <= numFilteredIDs; i++) {
			std::string key = "Filter_" + std::to_string(i) + "_";
			filter_options_struct filter;
			filter.type = intToFilterType(stoi(std::string(loadedIni.GetValue("Filters", (key + "type").c_str(), "0"))));
			filter.skillId = stoi(std::string(loadedIni.GetValue("Filters", (key + "id").c_str(), "0")));
			filter.skillName = std::string(loadedIni.GetValue("Filters", (key + "name").c_str(), ""));
			opt.skillFilters.push_back(filter);
		}

		opt.skillIconsEnabled = stob(loadedIni.GetValue("Skill_Icons", "Skill_Icons_Enabled", btos(opt.skillIconsEnabled).c_str()));
		opt.preloadAllSkillIcons = stob(loadedIni.GetValue("Skill_Icons", "Skill_Icons_Preload", btos(opt.preloadAllSkillIcons).c_str()));
	} else if (file_exist(jsonFilename)) {
		LOG("Loading sct.json");
		std::string line, text;
		std::ifstream in(jsonFilename);
		while (std::getline(in, line)) {
			text += line + "\n";
		}
		try {
			nlohmann::json j = nlohmann::json::parse(text);
			opt = j;
			defaultFont = getFontType(opt.masterFont, false);
		} catch (std::exception e) {
			LOG("Error loading options: ", e.what());
		}
	} else {
		setDefault();
	}
}

void GW2_SCT::Options::setDefault() {
	opt.revision = SCT_VERSION_STRING;
	auto incomingStruct = std::make_shared<scroll_area_options_struct>(scroll_area_options_struct{
		std::string(langStringG(LanguageKey::Default_Scroll_Area_Incoming)),
		-249.f,
		-25.f,
		40.f,
		260.f,
		TextAlign::RIGHT,
		TextCurve::LEFT,
		ScrollAreaOutlineState::NONE,
		{}
	});
	for (const auto& messageTypePair : receiverInformationPerCategoryAndType.at(MessageCategory::PLAYER_IN)) {
		incomingStruct->receivers.push_back(std::make_shared<message_receiver_options_struct>(messageTypePair.second.defaultReceiver));
	}
	for (const auto& messageTypePair : receiverInformationPerCategoryAndType.at(MessageCategory::PET_IN)) {
		incomingStruct->receivers.push_back(std::make_shared<message_receiver_options_struct>(messageTypePair.second.defaultReceiver));
	}
	opt.scrollAreaOptions.push_back(incomingStruct);
	auto outgoingStruct = std::make_shared<scroll_area_options_struct>(scroll_area_options_struct{
		std::string(langStringG(LanguageKey::Default_Scroll_Area_Outgoing)),
		217.f,
		-25.f,
		40.f,
		260.f,
		TextAlign::LEFT,
		TextCurve::RIGHT,
		ScrollAreaOutlineState::NONE,
		{}
	});
	for (const auto& messageTypePair : receiverInformationPerCategoryAndType.at(MessageCategory::PLAYER_OUT)) {
		outgoingStruct->receivers.push_back(std::make_shared<message_receiver_options_struct>(messageTypePair.second.defaultReceiver));
	}
	for (const auto& messageTypePair : receiverInformationPerCategoryAndType.at(MessageCategory::PET_OUT)) {
		outgoingStruct->receivers.push_back(std::make_shared<message_receiver_options_struct>(messageTypePair.second.defaultReceiver));
	}
	opt.scrollAreaOptions.push_back(outgoingStruct);
}

bool GW2_SCT::Options::isOptionsWindowOpen() {
	return windowIsOpen;
}

void GW2_SCT::Options::paintGeneral() {
	ImGui::Checkbox(langString(LanguageCategory::Option_UI, LanguageKey::General_Enabled), &opt.sctEnabled);
	ImGui::ClampingDragFloat(langString(LanguageCategory::Option_UI, LanguageKey::General_Scrolling_Speed), &opt.scrollSpeed, 1.f, 1.f, 2000.f);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(langString(LanguageCategory::Option_UI, LanguageKey::General_Scrolling_Speed_Toolip));
	ImGui::Checkbox(langString(LanguageCategory::Option_UI, LanguageKey::General_Drop_Shadow), &opt.dropShadow);
	ImGui::ClampingDragInt(langString(LanguageCategory::Option_UI, LanguageKey::General_Max_Messages), &opt.messagesInStack, 1, 1, 8);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(langString(LanguageCategory::Option_UI, LanguageKey::General_Max_Messages_Toolip));
	ImGui::Checkbox(langString(LanguageCategory::Option_UI, LanguageKey::General_Combine_Messages), &opt.combineAllMessages);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(langString(LanguageCategory::Option_UI, LanguageKey::General_Combine_Messages_Toolip));
	if (ImGui::Combo(langStringG(LanguageKey::Font_Master), &opt.masterFont, getFontSelectionString(false).c_str())) {
		defaultFont = getFontType(opt.masterFont, false);
	}
	ImGui::ClampingDragFloat(langStringG(LanguageKey::Default_Font_Size), &opt.defaultFontSize, 1.f, 1.f, 100.f);
	ImGui::ClampingDragFloat(langStringG(LanguageKey::Default_Crit_Font_Size), &opt.defaultCritFontSize, 1.f, 1.f, 100.f);
	ImGui::Checkbox(langString(LanguageCategory::Option_UI, LanguageKey::General_Self_Only_As_Incoming), &opt.selfMessageOnlyIncoming);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(langString(LanguageCategory::Option_UI, LanguageKey::General_Self_Only_As_Incoming_Toolip));
	ImGui::Checkbox(langString(LanguageCategory::Option_UI, LanguageKey::General_Out_Only_For_Target), &opt.outgoingOnlyToTarget);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(langString(LanguageCategory::Option_UI, LanguageKey::General_Out_Only_For_Target_Toolip));
}

void GW2_SCT::Options::paintScrollAreas() {
	const float square_size = ImGui::GetFontSize();
	ImGuiStyle style = ImGui::GetStyle();

	static int selectedScrollArea = -1;
	{
		ImGui::BeginChild("left pane", ImVec2(ImGui::GetWindowWidth() * 0.25f, 0), true);
		int i = 0;
		auto scrollAreaOptions = std::begin(opt.scrollAreaOptions);
		while (scrollAreaOptions != std::end(opt.scrollAreaOptions)) {
			if (ImGui::Selectable(
				ImGui::BuildLabel("scroll-area-selectable", i).c_str(),
				selectedScrollArea == i,
				ImGuiSelectableFlags_AllowItemOverlap,
				ImVec2(0, square_size + style.FramePadding.y * 2))
			)
				selectedScrollArea = i;
			ImGui::SameLine();
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + style.FramePadding.y);
			ImGui::Text(scrollAreaOptions->get()->name.c_str());
			ImGui::SameLineEnd(square_size + style.FramePadding.y * 2, 0);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - style.FramePadding.y);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.67f, 0.40f, 0.40f, 0.60f));
			std::string modalName = ImGui::BuildLabel(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Title), "scroll-area-delete-modal", i);
			if (ImGui::Button(ImGui::BuildLabel("-", "scroll-area-delete-button", i).c_str(), ImVec2(square_size + style.FramePadding.y * 2, square_size + style.FramePadding.y * 2))) {
				ImGui::OpenPopup(modalName.c_str());
			}
			ImGui::PopStyleColor();
			if (ImGui::BeginPopupModal(modalName.c_str())) {
				ImGui::Text(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Content));
				ImGui::Separator();
				if (ImGui::Button(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Confirmation), ImVec2(120, 0))) {
					scrollAreaOptions = opt.scrollAreaOptions.erase(scrollAreaOptions);
					ImGui::CloseCurrentPopup();
				} else {
					scrollAreaOptions++;
					i++;
				}
				ImGui::SameLine();
				if (ImGui::Button(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Delete_Confirmation_Cancel), ImVec2(120, 0))) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			} else {
				scrollAreaOptions++;
				i++;
			}
		}
		ImGui::Text("");
		ImGui::SameLineEnd(square_size + style.FramePadding.y * 2, 0);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.67f, 0.40f, 0.40f, 0.60f));
		if (ImGui::Button("+", ImVec2(square_size + style.FramePadding.y * 2, square_size + style.FramePadding.y * 2))) {
			scroll_area_options_struct newScrollArea{ langString(LanguageCategory::Scroll_Area_Option_UI, LanguageKey::Scroll_Areas_New), 0.f, 0.f, 40.f, 260.f, TextAlign::CENTER, TextCurve::STRAIGHT, ScrollAreaOutlineState::NONE, {} };
			opt.scrollAreaOptions.push_back(std::make_shared<scroll_area_options_struct>(newScrollArea));
			selectedScrollArea = -1;
		}
		ImGui::PopStyleColor();
		ImGui::EndChild();
	}
	ImGui::SameLine();

	// Right
	{
		ImGui::BeginGroup();
		ImGui::BeginChild("scroll area details", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
		if (selectedScrollArea >= 0 && selectedScrollArea < opt.scrollAreaOptions.size()) {
			std::shared_ptr<scroll_area_options_struct> scrollAreaOptions = opt.scrollAreaOptions[selectedScrollArea];

			for (auto otherScrollAreaOptions : opt.scrollAreaOptions) {
				if (scrollAreaOptions == otherScrollAreaOptions) {
					if (otherScrollAreaOptions->outlineState != ScrollAreaOutlineState::FULL) otherScrollAreaOptions->outlineState = ScrollAreaOutlineState::FULL;
				} else {
					if (otherScrollAreaOptions->outlineState != ScrollAreaOutlineState::LIGHT) otherScrollAreaOptions->outlineState = ScrollAreaOutlineState::LIGHT;
				}
			}

			{
				std::vector<char> arr(512);
				std::copy(scrollAreaOptions->name.begin(), scrollAreaOptions->name.size() < 512 ? scrollAreaOptions->name.end() : scrollAreaOptions->name.begin() + 512, arr.begin());
				if (ImGui::InputText((std::string(langString(GW2_SCT::LanguageCategory::Option_UI, GW2_SCT::LanguageKey::Scroll_Areas_Name)) + "##scroll-area-name").c_str(), arr.data(), 512)) {
					scrollAreaOptions->name = arr.data();
				}
			}
			ImGui::Separator();
			ImGui::ClampingDragFloat(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Horizontal_Offset), &scrollAreaOptions->offsetX, 1.f, -windowWidth / 2.f, windowWidth / 2.f);
			ImGui::ClampingDragFloat(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Vertical_Offset), &scrollAreaOptions->offsetY, 1.f, -windowHeight / 2.f, windowHeight / 2.f);
			ImGui::ClampingDragFloat(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Width), &scrollAreaOptions->width, 1.f, 1.f, (float)windowWidth);
			ImGui::ClampingDragFloat(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Height), &scrollAreaOptions->height, 1.f, 1.f, (float)windowHeight);
			ImGui::Combo(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Text_Align), (int*)&scrollAreaOptions->textAlign, TextAlignTexts, 3);
			ImGui::Combo(langString(GW2_SCT::LanguageCategory::Scroll_Area_Option_UI, GW2_SCT::LanguageKey::Text_Flow), (int*)&scrollAreaOptions->textCurve, TextCurveTexts, 3);
			ImGui::Text("Message Receivers:");
			int receiverOptionsCounter = 0;
			auto receiverOptionsIterator = std::begin(scrollAreaOptions->receivers);
			while (receiverOptionsIterator != std::end(scrollAreaOptions->receivers)) {
				int receiverReturnFlags = ImGui::ReceiverCollapsible(receiverOptionsCounter, *receiverOptionsIterator);
				if (receiverReturnFlags & ReceiverCollapsible_Remove) {
					receiverOptionsIterator = scrollAreaOptions->receivers.erase(receiverOptionsIterator);
				} else {
					receiverOptionsIterator++;
					receiverOptionsCounter++;
				}
			}
			if (scrollAreaOptions->receivers.size() == 0) ImGui::Text("    -");
			ImGui::Separator();
			ImGui::Text("New receiver:");
			static MessageCategory newReceiverCategory = MessageCategory::PLAYER_OUT;
			static MessageType newReceiverType = MessageType::PHYSICAL;
			if (ImGui::NewReceiverLine(&newReceiverCategory, &newReceiverType)) {
				auto& defaultReceiver = receiverInformationPerCategoryAndType.at(newReceiverCategory).at(newReceiverType).defaultReceiver;
				scrollAreaOptions->receivers.push_back(std::make_shared<message_receiver_options_struct>(defaultReceiver));
			}
		} else {
			for (auto scrollAreaOptions : opt.scrollAreaOptions) {
				if (scrollAreaOptions->outlineState != ScrollAreaOutlineState::FULL) scrollAreaOptions->outlineState = ScrollAreaOutlineState::FULL;
			}
		}
		ImGui::EndChild();
		ImGui::EndGroup();
	}
}

bool drawColorSelector(const char * name, std::string &color) {
	int num = std::stoi(color, 0, 16);
	int red = num / 0x10000;
	int green = (num / 0x100) % 0x100;
	int blue = num % 0x100;
	float col[3] = { red / 255.f, green / 255.f, blue / 255.f };
	if (ImGui::ColorEdit3(name, col, ImGuiColorEditFlags_NoAlpha)) {
		std::stringstream stm;
		stm << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(col[0] * 255.f);
		stm << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(col[1] * 255.f);
		stm << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(col[2] * 255.f);
		color = stm.str();
		return true;
	}
	return false;
}

void GW2_SCT::Options::paintProfessionColors() {
	drawColorSelector(langString(LanguageCategory::Option_UI, LanguageKey::Profession_Colors_Guardian), opt.professionColorGuardian);
	drawColorSelector(langString(LanguageCategory::Option_UI, LanguageKey::Profession_Colors_Warrior), opt.professionColorWarrior);
	drawColorSelector(langString(LanguageCategory::Option_UI, LanguageKey::Profession_Colors_Engineer), opt.professionColorEngineer);
	drawColorSelector(langString(LanguageCategory::Option_UI, LanguageKey::Profession_Colors_Ranger), opt.professionColorRanger);
	drawColorSelector(langString(LanguageCategory::Option_UI, LanguageKey::Profession_Colors_Thief), opt.professionColorThief);
	drawColorSelector(langString(LanguageCategory::Option_UI, LanguageKey::Profession_Colors_Elementalist), opt.professionColorElementalist);
	drawColorSelector(langString(LanguageCategory::Option_UI, LanguageKey::Profession_Colors_Mesmer), opt.professionColorMesmer);
	drawColorSelector(langString(LanguageCategory::Option_UI, LanguageKey::Profession_Colors_Necromancer), opt.professionColorNecromancer);
	drawColorSelector(langString(LanguageCategory::Option_UI, LanguageKey::Profession_Colors_Revenant), opt.professionColorRevenant);
	drawColorSelector(langString(LanguageCategory::Option_UI, LanguageKey::Profession_Colors_Undetectable), opt.professionColorDefault);
}

void GW2_SCT::Options::paintSkillFilters() {
	ImGui::Text("Filter by");
	uint32_t i = 0;
	for (auto it = opt.skillFilters.begin(); it != opt.skillFilters.end(); ++it) {
		int retFlags = ImGui::FilterOptionLine(i, &(*it));
		if (retFlags != 0) {
			if (retFlags & FilterOptionLine_Remove) {
				if (it == opt.skillFilters.begin()) {
					opt.skillFilters.erase(it);
					break;
				}
				else {
					auto last = it - 1;
					opt.skillFilters.erase(it);
					it = last;
				}
			}
		}
		i++;
	}
	ImGui::Text(" ");
	const float square_size = ImGui::GetFontSize();
	ImVec2 windowSize = ImGui::GetWindowSize();
	bool scrollY = ImGui::GetScrollMaxY() > 0;
	ImGuiStyle style = ImGui::GetStyle();
	ImGui::SameLine(windowSize.x - (square_size + style.FramePadding.y * 2) - (scrollY ? style.ScrollbarSize : 0) - style.WindowPadding.x, style.ItemInnerSpacing.x);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.67f, 0.40f, 0.40f, 0.60f));
	if (ImGui::Button("+", ImVec2(square_size + style.FramePadding.y * 2, square_size + style.FramePadding.y * 2))) {
		opt.skillFilters.push_back({ FilterType::SKILL_ID, 0, "" });
	}
	ImGui::PopStyleColor();
}

void GW2_SCT::Options::paintSkillIcons() {
	std::string language_warning(langString(LanguageCategory::Option_UI, LanguageKey::Skill_Icons_Warning));
	ImGui::TextWrapped((language_warning + " '" + getSCTPath() + "icons\\" + "'").c_str());
	ImGui::Checkbox(langString(LanguageCategory::Option_UI, LanguageKey::Skill_Icons_Enable), &opt.skillIconsEnabled);
	ImGui::TextWrapped(langString(LanguageCategory::Option_UI, LanguageKey::Skill_Icons_Preload_Description));
	ImGui::Checkbox(langString(LanguageCategory::Option_UI, LanguageKey::Skill_Icons_Preload), &opt.preloadAllSkillIcons);
}

std::map<char, std::string> GW2_SCT::mapParameterListToLanguage(const char * section, std::vector<char> list) {
	std::map<char, std::string> result;
	for (auto p : list) {
		result.insert({ p, std::string(langStringImguiSafe(GetLanguageCategoryValue(section), GetLanguageKeyValue((std::string("Parameter_Description_") + p).c_str()))) });
	}
	return result;
}