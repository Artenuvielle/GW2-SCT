#include "SCTMain.h"
#include <string>
#include <vector>
#include <stdint.h>
#include <fstream>
#include "imgui.h"
#include "Options.h"
#include "SkillIconManager.h"
#include "FontManager.h"
#include "Language.h"
#include "ExampleMessageOptions.h"
#include <chrono>

#if _DEBUG
long uiFrames = 0;
float uiTime = 0;
#endif

float windowWidth;
float windowHeight;

GW2_SCT::SCTMain::SCTMain() {}

GW2_SCT::SCTMain::~SCTMain() {
	this->Release();
	Options::save();
	FontManager::cleanup();
}

arcdps_exports* GW2_SCT::SCTMain::Init(char* arcvers, void* mod_wnd, void* mod_combat, void* mod_imgui, void* mod_options, void* mod_combat_local) {
	logFile = std::ofstream(getSCTPath() + "sct.log");
	LOG("Running arcvers: ", arcvers);
	LOG("Running sct version: ", SCT_VERSION_STRING, " / ", __DATE__, " / ", __TIME__);

	Options::profile.onAssign([=](std::shared_ptr<profile_options_struct> oldProfile, std::shared_ptr<profile_options_struct> newProfile) {
		if (currentScrollAreaPushBackCallbackId >= 0) {
			oldProfile->scrollAreaOptions.removeOnPushBackCallback(currentScrollAreaPushBackCallbackId);
		}
		if (currentScrollAreaEraseCallbackId >= 0) {
			oldProfile->scrollAreaOptions.removeOnEraseCallback(currentScrollAreaEraseCallbackId);
		}
		currentScrollAreaPushBackCallbackId = newProfile->scrollAreaOptions.addOnPushBackCallback([=](const std::shared_ptr<scroll_area_options_struct>& newVal) {
			scrollAreas.push_back(std::make_shared<ScrollArea>(newVal));
		});
		currentScrollAreaEraseCallbackId = newProfile->scrollAreaOptions.addOnEraseCallback([=](int pos) {
			scrollAreas.erase(std::begin(scrollAreas) + pos);
		});
	});
	LOG("Set up options changing hook");
	SkillIconManager::init();
	LOG("Started skill icon manager");
	FontManager::init();
	LOG("Started font manager");
	Options::load();
	LOG("Loaded options");
	for (const auto& scrollAreaOptions : Options::get()->scrollAreaOptions)
		scrollAreas.push_back(std::make_shared<ScrollArea>(scrollAreaOptions));
	LOG("Created ", Options::get()->scrollAreaOptions.size(), " scroll areas");

	if (d3Device11 != nullptr) {
		if (d3d11SwapChain != nullptr) {
			DXGI_SWAP_CHAIN_DESC desc;
			if (SUCCEEDED(d3d11SwapChain->GetDesc(&desc))) {
				RECT rect;
				if (GetWindowRect(desc.OutputWindow, &rect)) {
					windowWidth = rect.right - rect.left;
					windowHeight = rect.bottom - rect.top;
				}
			}
		}
		LOG("Found d3d11 device.");
		if (d3D11Context != nullptr) {
			LOG("Found d3d11 context aswell.");
		} else {
			LOG("But found no d3d11 context.");
		}
	} else {
		LOG("Found no d3 device (version: ", d3dversion, ")!");
	}

	std::string remapJsonFilename = getSCTPath() + "remap.json";
	if (file_exist(remapJsonFilename)) {
		LOG("Loading remap.json");
		std::string line, text;
		std::ifstream in(remapJsonFilename);
		while (std::getline(in, line)) {
			text += line + "\n";
		}
		in.close();
		try {
			std::map<std::string, std::string> remapJsonValues = nlohmann::json::parse(text);
			for (const auto& entry : remapJsonValues) {
				uint32_t from = stoi(std::string(entry.first));
				uint32_t to = stoi(std::string(entry.second));
				while (skillRemaps.find(to) != skillRemaps.end()) {
					if (from == to) break;
					to = skillRemaps[to];
				}
				if (from == to) break;
				LOG("Remapping id ", from, " to id ", to);
				for (auto currentRemap : skillRemaps) {
					if (currentRemap.second == from) {
						skillRemaps[currentRemap.first] = to;
						LOG("Touched remap from id ", currentRemap.first, " to id ", to);
					}
				}
				skillRemaps[from] = to;
			}
			LOG("Loaded remap.json successfully: ", skillRemaps.size(), " remapped skills")
		}
		catch (std::exception& e) {
			LOG("Error parsing remap.json");
		}
	} else {
		LOG("No remap.json file loaded");
	}

	ExampleMessageOptions::setMain(this);
	
	/* for arcdps */
	memset(&arc_exports, 0, sizeof(arcdps_exports));
	arc_exports.size = sizeof(arcdps_exports);
	arc_exports.sig = 0x79167910;
	arc_exports.imguivers = IMGUI_VERSION_NUM;
	arc_exports.out_name = "GW2 SCT";
	arc_exports.out_build = SCT_VERSION_STRING;
	arc_exports.wnd_nofilter = mod_wnd;
	arc_exports.combat = mod_combat;
	arc_exports.imgui = mod_imgui;
	arc_exports.options_end = mod_options;
	arc_exports.combat_local = mod_combat_local;
	return &arc_exports;
}

uintptr_t GW2_SCT::SCTMain::Release() {
	SkillIconManager::cleanup();
	logFile.flush();
	logFile.close();
	return 0;
}

uintptr_t GW2_SCT::SCTMain::WindowUpdate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_SIZE:
		windowWidth = LOWORD(lParam);
		windowHeight = HIWORD(lParam);
		break;
	}
	return uMsg;
}

uintptr_t GW2_SCT::SCTMain::CombatEventArea(cbtevent * ev, ag * src, ag * dst, char * skillname, uint64_t id, uint64_t revision) {
	if (ev != nullptr) {
		if (revision == 1) {
			cbtevent1* ev1 = reinterpret_cast<cbtevent1*>(ev);
			if (src && src->self && selfInstID != ev1->src_instid) {
				selfInstID = ev1->src_instid;
				Options::loadProfile(std::string(src->name));
			}
		}
		else {
			if (src && src->self && selfInstID != ev->src_instid) {
				selfInstID = ev->src_instid;
				Options::loadProfile(std::string(src->name));
			}
		}
	}
	
	return 0;
}

uintptr_t GW2_SCT::SCTMain::CombatEventLocal(cbtevent * ev, ag * src, ag * dst, char * skillname, uint64_t id, uint64_t revision) {
	/* combat event. skillname may be null. non-null skillname will remain static until module is unloaded. refer to evtc notes for complete detail */
	if (ev) {
		if (revision == 1) {
			cbtevent1* ev1 = reinterpret_cast<cbtevent1*>(ev);
			/* default names */
			if (!src->name || !strlen(src->name)) src->name = _strdup(langStringG(LanguageKey::Unknown_Skill_Source));
			if (!dst->name || !strlen(dst->name)) dst->name = _strdup(langStringG(LanguageKey::Unknown_Skill_Target));
			if (!skillname || !strlen(skillname)) skillname = _strdup(langStringG(LanguageKey::Unknown_Skill_Name));

			if (src->self) {
				selfInstID = ev1->src_instid;
			}
			if (dst->self) {
				selfInstID = ev1->dst_instid;
			}

			/* statechange */
			if (ev1->is_statechange) {
				return 0;
			}

			/* activation */
			else if (ev1->is_activation) {
				return 0;
			}

			/* buff remove */
			else if (ev1->is_buffremove) {
				return 0;
			}

			else {
				std::vector<MessageType> types;
				if (ev1->buff) {
					if (ev1->buff_dmg > 0) {
						if (ev1->overstack_value != 0) {
							ev1->buff_dmg -= ev1->overstack_value;
							types.push_back(MessageType::SHIELD_RECEIVE);
						}
						if (ev1->buff_dmg > 0) {
							types.push_back(MessageType::HOT);
						}
					}
					else if (ev1->buff_dmg < 0) {
						if (ev1->overstack_value > 0) {
							ev1->buff_dmg += ev1->overstack_value;
							types.push_back(MessageType::SHIELD_REMOVE);
						}
						if (ev1->buff_dmg < 0) {
							switch (ev1->skillid)
							{
							case 736: types.push_back(MessageType::BLEEDING); break;
							case 737: types.push_back(MessageType::BURNING); break;
							case 723: types.push_back(MessageType::POISON); break;
							case 861: types.push_back(MessageType::CONFUSION); break;
							case 873: types.push_back(MessageType::RETALIATION); break;
							case 19426: types.push_back(MessageType::TORMENT); break;
							default: types.push_back(MessageType::DOT); break;
							}
						}
					}
				}
				else {
					if (ev1->value > 0) {
						if (ev1->overstack_value != 0) {
							ev1->value += ev1->overstack_value;
							types.push_back(MessageType::SHIELD_RECEIVE);
						}
						if (ev1->value > 0) {
							types.push_back(MessageType::HEAL);
						}
					}
					else {
						if (ev1->overstack_value > 0) {
							ev1->value += ev1->overstack_value;
							types.push_back(MessageType::SHIELD_REMOVE);
						}
						if (ev1->overstack_value <= 0 || ev1->value < 0) {
							switch (ev1->result) {
							case CBTR_GLANCE:
							case CBTR_INTERRUPT:
							case CBTR_NORMAL: types.push_back(MessageType::PHYSICAL); break;
							case CBTR_CRIT: types.push_back(MessageType::CRIT); break;
							case CBTR_BLOCK: types.push_back(MessageType::BLOCK);  break;
							case CBTR_EVADE: types.push_back(MessageType::EVADE); break;
							case CBTR_ABSORB: types.push_back(MessageType::INVULNERABLE); break;
							case CBTR_BLIND: types.push_back(MessageType::MISS); break;
							default:
								break;
							}
						}
					}
				}
				if (types.size() > 0) {
					ev1->skillid = remapSkillID(ev1->skillid);
				}
				for (auto type : types) {
					if (src->self == 1 && (!Options::get()->outgoingOnlyToTarget || dst->id == targetAgentId)) {
						if (!Options::get()->selfMessageOnlyIncoming || dst->self != 1) {
							std::shared_ptr<GW2_SCT::EventMessage> m = std::make_shared<GW2_SCT::EventMessage>(MessageCategory::PLAYER_OUT, type, ev1, src, dst, skillname);
							emitMessageToScrollAreas(m);
						}
					}
					else if (ev1->src_master_instid == selfInstID && (!Options::get()->outgoingOnlyToTarget || dst->id == targetAgentId)) {
						std::shared_ptr<GW2_SCT::EventMessage> m = std::make_shared<GW2_SCT::EventMessage>(MessageCategory::PET_OUT, type, ev1, src, dst, skillname);
						emitMessageToScrollAreas(m);
					}
					if (dst->self == 1) {
						std::shared_ptr<GW2_SCT::EventMessage> m = std::make_shared<GW2_SCT::EventMessage>(MessageCategory::PLAYER_IN, type, ev1, src, dst, skillname);
						emitMessageToScrollAreas(m);
					}
					else if (ev1->dst_master_instid == selfInstID) {
						std::shared_ptr<GW2_SCT::EventMessage> m = std::make_shared<GW2_SCT::EventMessage>(MessageCategory::PET_IN, type, ev1, src, dst, skillname);
						emitMessageToScrollAreas(m);
					}
				}
			}
		}
		else {
			/* default names */
			if (!src->name || !strlen(src->name)) src->name = _strdup(langStringG(LanguageKey::Unknown_Skill_Source));
			if (!dst->name || !strlen(dst->name)) dst->name = _strdup(langStringG(LanguageKey::Unknown_Skill_Target));
			if (!skillname || !strlen(skillname)) skillname = _strdup(langStringG(LanguageKey::Unknown_Skill_Name));

			if (src->self) {
				selfInstID = ev->src_instid;
			}
			if (dst->self) {
				selfInstID = ev->dst_instid;
			}

			/* statechange */
			if (ev->is_statechange) {
				return 0;
			}

			/* activation */
			else if (ev->is_activation) {
				return 0;
			}

			/* buff remove */
			else if (ev->is_buffremove) {
				return 0;
			}

			else {
				MessageType type = MessageType::NONE;
				if (ev->buff) {
					if (ev->buff_dmg < 0) {
						type = MessageType::HOT;
					}
					else if (ev->buff_dmg > 0) {
						switch (ev->skillid)
						{
						case 736: type = MessageType::BLEEDING; break;
						case 737: type = MessageType::BURNING; break;
						case 723: type = MessageType::POISON; break;
						case 861: type = MessageType::CONFUSION; break;
						case 873: type = MessageType::RETALIATION; break;
						case 19426: type = MessageType::TORMENT; break;
						default: type = MessageType::DOT; break;
						}
					}
				}
				else {
					if (ev->value < 0) {
						type = MessageType::HEAL;
					}
					else switch (ev->result) {
					case CBTR_GLANCE:
					case CBTR_NORMAL: type = MessageType::PHYSICAL; break;
					case CBTR_CRIT: type = MessageType::CRIT; break;
					case CBTR_BLOCK: type = MessageType::BLOCK;  break;
					case CBTR_EVADE: type = MessageType::EVADE; break;
					case CBTR_ABSORB: type = MessageType::INVULNERABLE; break;
					case CBTR_BLIND: type = MessageType::MISS; break;
					default:
						break;
					}
				}
				if (type != MessageType::NONE) {
					if (src->self) {
						if (!Options::get()->selfMessageOnlyIncoming || !dst->self) {
							std::shared_ptr<GW2_SCT::EventMessage> m = std::make_shared<GW2_SCT::EventMessage>(MessageCategory::PLAYER_OUT, type, ev, src, dst, skillname);
							emitMessageToScrollAreas(m);
						}
					}
					else if (ev->src_master_instid == selfInstID) {
						std::shared_ptr<GW2_SCT::EventMessage> m = std::make_shared<GW2_SCT::EventMessage>(MessageCategory::PET_OUT, type, ev, src, dst, skillname);
						emitMessageToScrollAreas(m);
					}
					if (dst->self) {
						std::shared_ptr<GW2_SCT::EventMessage> m = std::make_shared<GW2_SCT::EventMessage>(MessageCategory::PLAYER_IN, type, ev, src, dst, skillname);
						emitMessageToScrollAreas(m);
					}
					/*else if (ev->dst_master_instid == selfInstID) {
						std::shared_ptr<GW2_SCT::Message> m = std::make_shared<GW2_SCT::Message>(MessageCategory::PET_IN, type, ev, src, dst, skillname);
						ScrollArea::emitMessage(m);
					}*/
				}
			}
		}
	}
	else {
		if (src != nullptr) {
			targetAgentId = src->id;
		}
	}
	return 0;
}

uintptr_t GW2_SCT::SCTMain::UIUpdate() {
#if _DEBUG
	auto start_time = std::chrono::high_resolution_clock::now();
#endif
	FontType::ensureAtlasCreation();
	Options::paint();
	ExampleMessageOptions::paint();
	if (Options::get()->sctEnabled) {
		ImVec2 windowSize((float)windowWidth, (float)windowHeight);
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(windowSize);
		ImGui::Begin("SCT", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

		for (std::shared_ptr<ScrollArea> scrollArea : scrollAreas) {
			scrollArea->paint();
		}

		ImGui::End();
	}
#if _DEBUG
	auto time = std::chrono::high_resolution_clock::now() - start_time;
	uiFrames++;
	uiTime += time / std::chrono::microseconds(1);
	if (uiFrames >= 1000) {
		LOG("time per ui update: ", uiTime / uiFrames, "ns");
		uiFrames = 0;
		uiTime = 0;
	}
#endif
	return 0;
}

uintptr_t GW2_SCT::SCTMain::UIOptions() {
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 4, 3 });
	if (ImGui::Button(langString(LanguageCategory::Option_UI, LanguageKey::Title)))
		Options::open();
	if (ImGui::Button(langString(LanguageCategory::Example_Message_UI, LanguageKey::Title)))
		ExampleMessageOptions::open();
	ImGui::PopStyleVar();
	return 0;
}

uint32_t GW2_SCT::SCTMain::remapSkillID(uint32_t originalID) {
	if (skillRemaps.count(originalID) == 0) return originalID;
	else return skillRemaps[originalID];
}

void GW2_SCT::SCTMain::emitMessageToScrollAreas(std::shared_ptr<EventMessage> m) {
	if (m->hasToBeFiltered()) return;
	for (auto scrollArea : scrollAreas) {
		scrollArea->receiveMessage(m);
	}
	ExampleMessageOptions::receiveMessage(m);
}