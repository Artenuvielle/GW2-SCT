/*
* arcdps submodule scrolling combat text
*/

#include <stdint.h>
#include <string>
#include <locale>
#include <codecvt>
#include <stdio.h>
#include <d3d9.h>
#include <d3d11.h>
#include "SCTMain.h"
#include <iostream>
#include "Language.h"

#pragma comment(lib, "d3dx9")
#pragma comment(lib, "d3d11")

/* proto/globals */
arcdps_exports arc_exports;
void dll_init(HANDLE hModule);
void dll_exit();
extern "C" __declspec(dllexport) void* get_init_addr(char* arcversion, ImGuiContext* imguictx, void* id3dptr, HANDLE arcdll, void* mallocfn, void* freefn, uint32_t d3dversion);
extern "C" __declspec(dllexport) void* get_release_addr();
arcdps_exports* mod_init();
uintptr_t mod_release();
uintptr_t mod_wnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
uintptr_t mod_combat_area(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision);
uintptr_t mod_combat_local(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision);
uintptr_t mod_imgui();
uintptr_t mod_options();

GW2_SCT::SCTMain* sct;

/* dll main -- winapi */
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ulReasonForCall, LPVOID lpReserved) {
	switch (ulReasonForCall) {
		case DLL_PROCESS_ATTACH: dll_init(hModule); break;
		case DLL_PROCESS_DETACH: dll_exit(); break;

		case DLL_THREAD_ATTACH: break;
		case DLL_THREAD_DETACH: break;
	}
	return 1;
}

/* dll attach -- from winapi */
void dll_init(HANDLE hModule) {
	sct = new GW2_SCT::SCTMain();
	return;
}

/* dll detach -- from winapi */
void dll_exit() {
	delete sct;
	return;
}

/* export -- arcdps looks for this exported function and calls the address it returns on client load */
extern "C" __declspec(dllexport) void* get_init_addr(char* arcversion, ImGuiContext* imguictx, void* id3dptr, HANDLE arcdll, void* mallocfn, void* freefn, uint32_t d3dversion) {
	arcvers = arcversion;
	ImGui::SetCurrentContext((ImGuiContext*)imguictx);
	ImGui::SetAllocatorFunctions((void *(*)(size_t, void*))mallocfn, (void(*)(void*, void*))freefn); // on imgui 1.80+
	GW2_SCT::d3dversion = d3dversion;
	if (d3dversion == 9) {
		GW2_SCT::d3Device9 = (IDirect3DDevice9*)id3dptr;
	} else if (d3dversion == 11) {
		GW2_SCT::d3d11SwapChain = (IDXGISwapChain*)id3dptr;
		ID3D11Device* d3Device11;
		((IDXGISwapChain*)id3dptr)->GetDevice(__uuidof(ID3D11Device), (void**)&d3Device11);
		if (d3Device11 != nullptr) {
			ID3D11DeviceContext* d3D11Context;
			d3Device11->GetImmediateContext(&d3D11Context);
			GW2_SCT::d3Device11 = d3Device11;
			GW2_SCT::d3D11Context = d3D11Context;
		}
	}
	return mod_init;
}

/* export -- arcdps looks for this exported function and calls the address it returns */
extern "C" __declspec(dllexport) void* get_release_addr() {
	arcvers = 0;
	return mod_release;
}

bool isArcdpsOutdated = false;

arcdps_exports* mod_init() {
#ifdef _DEBUG
	//AllocConsole();
	debug_console_hnd = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	std::string arcversString(arcvers);
	arcversString = arcversString.substr(0, arcversString.find_first_of('.'));
	std::string arcreqString(ARC_VERSION_REQUIRED);
	arcreqString = arcreqString.substr(0, arcreqString.find_first_of('.'));
	isArcdpsOutdated = arcversString.compare(arcreqString) < 0;
	arcdps_exports* ret = sct->Init(arcvers, mod_wnd, mod_combat_area, mod_imgui, mod_options, mod_combat_local);
	LOG("Running arcvers: ", arcvers);
	LOG("Running sct version: ", SCT_VERSION_STRING, " / ", __DATE__, " / ", __TIME__);
	return ret;
}

uintptr_t mod_release() {
	return sct->Release();
}

/* window callback -- return is assigned to umsg (return zero to not be processed by arcdps or game) */
uintptr_t mod_wnd(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return sct->WindowUpdate(hWnd, uMsg, wParam, lParam);
}

uintptr_t mod_imgui() {
	if (isArcdpsOutdated) {
		ImGui::OpenPopup(langStringG(GW2_SCT::LanguageKey::Outdated_Popup_Title));
	}
	if (ImGui::BeginPopupModal(langStringG(GW2_SCT::LanguageKey::Outdated_Popup_Title))) {
		std::string s = std::string(langStringG(GW2_SCT::LanguageKey::Outdated_Popup_Content)) + "\n\n" + std::string(langStringG(GW2_SCT::LanguageKey::Outdated_Popup_Found_Version)) + ": " + std::string(arcvers) + "\n" + std::string(langStringG(GW2_SCT::LanguageKey::Outdated_Popup_Required_Version)) + ": " + ARC_VERSION_REQUIRED;
		ImGui::Text(s.c_str());
		ImGui::Separator();
		if (ImGui::Button(langStringG(GW2_SCT::LanguageKey::Outdated_Popup_Confirmation), ImVec2(120, 0))) {
			isArcdpsOutdated = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	return sct->UIUpdate();
}

uintptr_t mod_options() {
	return sct->UIOptions();
}

/* combat callback -- may be called asynchronously. return ignored */
uintptr_t mod_combat_area(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision) {
	return sct->CombatEventArea(ev, src, dst, skillname, id, revision);
}

/* combat callback -- may be called asynchronously. return ignored */
uintptr_t mod_combat_local(cbtevent* ev, ag* src, ag* dst, char* skillname, uint64_t id, uint64_t revision) {
	return sct->CombatEventLocal(ev, src, dst, skillname, id, revision);
}
