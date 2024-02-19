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
	public:
		MessageData(cbtevent* ev, ag* entity, ag * otherEntity, const char* skillname);
		MessageData(cbtevent1* ev, ag* entity, ag * otherEntity, const char* skillname);
#ifdef _DEBUG
		MessageData(int32_t value, int32_t buffValue, uint32_t overstack_value, uint32_t skillId, ag* entity, ag* otherEntity, const char* skillname);
#endif // _DEBUG
		MessageData() {};
		MessageData(const MessageData& toCopy);
	};

	struct MessageHandler {
		std::vector<std::function<bool(std::vector<const MessageData*>&, std::vector<const MessageData*>&)>> tryToCombineWithFunctions;
		std::map<char, std::function<std::string(std::vector<const MessageData*>&)>> parameterToStringFunctions;
		MessageHandler(
			std::vector<std::function<bool(std::vector<const MessageData*>&, std::vector<const MessageData*>&)>> tryToCombineWithFunctions,
			std::map<char, std::function<std::string(std::vector<const MessageData*>&)>> parameterToStringFunctions
		);
	};

	class EventMessage {
	public:
		EventMessage(MessageCategory category, MessageType type, cbtevent* ev, ag* src, ag* dst, char* skillname);
		EventMessage(MessageCategory category, MessageType type, cbtevent1* ev, ag* src, ag* dst, char* skillname);
		EventMessage(MessageCategory category, MessageType type, std::shared_ptr<MessageData>);
		~EventMessage();
		std::string getStringForOptions(std::shared_ptr<message_receiver_options_struct> opt);
		std::shared_ptr<MessageData> getCopyOfFirstData();
		MessageCategory getCategory();
		MessageType getType();
		bool hasToBeFiltered();
		bool tryToCombineWith(std::shared_ptr<EventMessage> m);
		std::chrono::system_clock::time_point getTimepoint();
	private:
		std::chrono::system_clock::time_point timepoint;
		MessageCategory category;
		MessageType type;
		std::vector<const MessageData*> messageDatas;
		static std::map<MessageCategory, std::map<MessageType, MessageHandler>> messageHandlers;
	};
}