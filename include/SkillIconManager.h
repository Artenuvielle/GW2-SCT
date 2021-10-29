#pragma once
#include "Texture.h"
#include <memory>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <imgui.h>
#include <safe_ptr.h>
#include <atomic>

namespace GW2_SCT {

	class SkillIcon {
	public:
		ImVec2 draw(ImVec2 pos, ImVec2 size, ImU32 color = 0xFFFFFFFF);
		SkillIcon(std::shared_ptr<std::vector<BYTE>> fileData, uint32_t skillID);
		~SkillIcon();
	private:
		void loadTexture();
		ImmutableTexture* texture = nullptr;
		std::shared_ptr<std::vector<BYTE>> fileData;
		uint32_t skillID;
		bool textureCreated = false;
	};

	class SkillIconManager {
	public:
		static void init();
		static void cleanup();
		static SkillIcon* getIcon(uint32_t skillID);
	private:
		static void internalInit();
		static void spawnLoadThread();
		static void loadThreadCycle();
		static int requestsPerMinute;
		static std::unordered_map<uint32_t, std::pair<std::string, std::string>> staticFiles;
		static sf::contfree_safe_ptr<std::unordered_map<uint32_t, bool>> checkedIDs;
		static sf::contfree_safe_ptr<std::list<uint32_t>> requestedIDs;
		static sf::contfree_safe_ptr<std::unordered_map<uint32_t, SkillIcon>> loadedIcons;
		static std::thread loadThread;
		static std::atomic<bool> keepLoadThreadRunning;

		static long skillIconsEnabledCallbackId;
		static long preloadAllSkillIconsId;
	};
}
