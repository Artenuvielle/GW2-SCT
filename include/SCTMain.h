#pragma once
#include <memory>
#include <vector>
#include "Common.h"
#include "Options.h"
#include "ScrollArea.h"

/* arcdps export table */
struct arcdps_exports {
	uintptr_t size; /* size of exports table */
	uint32_t sig; /* pick a number between 0 and uint32_t max that isn't used by other modules */
	uint32_t imguivers; /* set this to IMGUI_VERSION_NUM. if you don't use imgui, 18000 (as of 2021-02-02) */
	const char* out_name; /* name string */
	const char* out_build; /* build string */
	void* wnd_nofilter; /* wndproc callback, fn(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam), return assigned to umsg */
	void* combat; /* combat event callback, fn(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision) */
	void* imgui; /* id3dd9::present callback, before imgui::render, fn(uint32_t not_charsel_or_loading) */
	void* options_end; /* id3dd9::present callback, appending to the end of options window in arcdps, fn() */
	void* combat_local;  /* combat event callback like area but from chat log, fn(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision) */
	void* wnd_filter; /* wndproc callback like above, input filered using modifiers */
	void* options_windows; /* called once per 'window' option checkbox, with null at the end, non-zero return disables drawing that option, fn(char* windowname) */
};

namespace GW2_SCT {
	class SCTMain {
	public:
		SCTMain();
		~SCTMain();

		arcdps_exports* Init(char* arcvers, void* mod_wnd, void* mod_combat, void* mod_imgui, void* mod_options, void* mod_combat_local);
		uintptr_t Release();
		uintptr_t WindowUpdate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		uintptr_t CombatEventArea(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision);
		uintptr_t CombatEventLocal(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision);
		uintptr_t UIUpdate();
		uintptr_t UIOptions();
	private:
		uint32_t remapSkillID(uint32_t originalID);
		void emitMessageToScrollAreas(std::shared_ptr<EventMessage> m);
		arcdps_exports arc_exports;

		uintptr_t selfInstID = -1;
		uintptr_t targetAgentId = -1;
		std::map<uint32_t, uint32_t> skillRemaps;
		std::vector<std::shared_ptr<GW2_SCT::ScrollArea>> scrollAreas;
	};
}
