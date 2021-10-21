#pragma once
#include <deque>
#include <list>
#include <chrono>
#include <memory>
#include <mutex>
#include "OptionsStructures.h"
#include "Message.h"
#include "TemplateInterpreter.h"

namespace GW2_SCT {
	class ScrollArea {
	public:
		ScrollArea(std::shared_ptr<scroll_area_options_struct> options);
		void receiveMessage(std::shared_ptr<EventMessage> m);
		void transferMessagesTo(std::shared_ptr<ScrollArea> otherScrollArea);
		void paint();
	private:
		struct MessagePrerender {
			std::shared_ptr<EventMessage> message;
			std::string str;
			FontType* font;
			float fontSize;
			MessageCategory category;
			MessageType type;
			std::shared_ptr<message_receiver_options_struct> options;
			std::vector<TemplateInterpreter::InterpretedText> interpretedText;
			float interpretedTextWidth;
			bool prerenderNeeded = true;
			long templateObserverId, colorObserverId, fontObserverId, fontSizeObserverId;
			MessagePrerender(std::shared_ptr<EventMessage> message, std::shared_ptr<message_receiver_options_struct> options);
			~MessagePrerender();
			void update();
			void prerenderText();
		};

		std::mutex messageQueueMutex;
		std::deque<std::shared_ptr<MessagePrerender>> messageQueue = std::deque<std::shared_ptr<MessagePrerender>>();

		void paintOutline();
		bool paintMessage(std::shared_ptr<MessagePrerender> m, __int64 time);

		std::list<std::pair<std::shared_ptr<MessagePrerender>, std::chrono::time_point<std::chrono::system_clock>>> paintedMessages;

		std::shared_ptr<scroll_area_options_struct> options;
	};
}