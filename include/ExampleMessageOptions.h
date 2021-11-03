#pragma once
#include <chrono>
#include <map>
#include <memory>
#include <thread>
#include "Common.h"
#include "Message.h"
#include "SCTMain.h"

namespace GW2_SCT {
	class ExampleMessageOptions {
	public:
		static void paint();
		static void open();
		static void setMain(SCTMain* m) { main = m; };
		static void receiveMessage(std::shared_ptr<EventMessage>);
	private:
		enum State {
			READY_TO_RECORD,
			RECORDING,
			EMITTING
		};
		struct MessageInformation {
			MessageCategory category;
			MessageType type;
			std::shared_ptr<MessageData> data;
		};
		static void emitMessages();
		static bool windowIsOpen;
		static State state;
		static std::chrono::system_clock::time_point recordingStart;
		static SCTMain* main;
		static std::multimap<std::chrono::system_clock::duration, MessageInformation> messagesToEmmit;
		static std::thread* emitterThread;
	};
}
