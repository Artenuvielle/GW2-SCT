#pragma once
#include <stdint.h>
#include <string>
#include <functional>
#include <memory>
#include "Common.h"
#include "Options.h"

namespace GW2_SCT {
	struct MessageData {
		char* skillName = nullptr;
		char* entityName = nullptr;
		char* otherEntityName = nullptr;
		int32_t value = 0;
		uint32_t overstack_value = 0;
		int32_t buffValue = 0;
		uint32_t skillId = 0;
		uint64_t entityId = 0;
		uint32_t entityProf = 0;
		uint64_t otherEntityId = 0;
		uint32_t otherEntityProf = 0;
		bool hasToBeFiltered = false;
		MessageData(cbtevent* ev, ag* entity, ag * otherEntity, char* skillname);
		MessageData(cbtevent1* ev, ag* entity, ag * otherEntity, char* skillname);
	};

	struct MessageHandler {
		std::vector<std::function<bool(std::vector<std::shared_ptr<MessageData>>, std::vector<std::shared_ptr<MessageData>>)>> tryToCombineWithFunctions;
		std::map<char, std::function<std::string(std::vector<std::shared_ptr<MessageData>>)>> parameterToStringFunctions;
		MessageHandler(
			std::vector<std::function<bool(std::vector<std::shared_ptr<MessageData>>, std::vector<std::shared_ptr<MessageData>>)>> tryToCombineWithFunctions,
			std::map<char, std::function<std::string(std::vector<std::shared_ptr<MessageData>>)>> parameterToStringFunctions
		);
	};

	class EventMessage {
	public:
		EventMessage(MessageCategory category, MessageType type, cbtevent* ev, ag* src, ag* dst, char* skillname);
		EventMessage(MessageCategory category, MessageType type, cbtevent1* ev, ag* src, ag* dst, char* skillname);
		std::string getStringForOptions(std::shared_ptr<message_receiver_options_struct> opt);
		MessageCategory getCategory();
		MessageType getType();
		bool hasToBeFiltered();
		bool tryToCombineWith(std::shared_ptr<EventMessage> m);
	private:
		MessageCategory category;
		MessageType type;
		std::vector<std::shared_ptr<MessageData>> messageDatas;
		static std::map<MessageCategory, std::map<MessageType, MessageHandler>> messageHandlers;
	};
}