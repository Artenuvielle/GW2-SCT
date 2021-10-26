#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "SkillIconManager.h"
#include <string>
#include <thread>
#include <functional>
#include <regex>
#include "SimpleIni.h"
#include "Common.h"
#include "Options.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#pragma comment(lib, "libssl")
#pragma comment(lib, "libcrypto")

httplib::Client renderingClient("https://render.guildwars2.com");
httplib::Client apiClient("https://api.guildwars2.com");

nlohmann::json getJSON(std::string url, std::function<void(httplib::Headers)> callback = nullptr) {
	if (auto res = apiClient.Get(url.c_str())) {
		if (callback != nullptr)
			callback(res->headers);

		return nlohmann::json::parse(res->body);
	} else {
		throw std::exception(("Error sending GET request to https://api.guildwars2.com" + url + ": " + to_string(res.error())).c_str());
	}
}

std::shared_ptr<std::vector<BYTE>> loadBinaryFileData(std::string filename) {
	std::ifstream in(filename, std::ios::binary); //open file
	in >> std::noskipws;  //we don't want to skip spaces        
	
	//initialize a vector with a pair of istream_iterators
	std::shared_ptr<std::vector<BYTE>> ret = std::make_shared<std::vector<BYTE>>(std::istream_iterator<BYTE>(in), std::istream_iterator<BYTE>());
	return ret;
}

sf::contfree_safe_ptr<std::unordered_map<uint32_t, bool>> GW2_SCT::SkillIconManager::checkedIDs;
sf::contfree_safe_ptr<std::unordered_map<uint32_t, GW2_SCT::SkillIcon>> GW2_SCT::SkillIconManager::loadedIcons;
sf::contfree_safe_ptr<std::list<uint32_t>> GW2_SCT::SkillIconManager::requestedIDs;
std::thread GW2_SCT::SkillIconManager::loadThread;
int GW2_SCT::SkillIconManager::requestsPerMinute = 200;
std::atomic<bool> GW2_SCT::SkillIconManager::keepLoadThreadRunning;

std::unordered_map<uint32_t, std::pair<std::string, std::string>> GW2_SCT::SkillIconManager::staticFiles = {
	{ 736, { "79FF0046A5F9ADA3B4C4EC19ADB4CB124D5F0021", "102848" } }, //Bleeding
	{ 737, { "B47BF5803FED2718D7474EAF9617629AD068EE10", "102849" } }, //Burning
	{ 723, { "559B0AF9FB5E1243D2649FAAE660CCB338AACC19", "102840" } }, //Poison
	{ 861, { "289AA0A4644F0E044DED3D3F39CED958E1DDFF53", "102880" } }, //Confusion
	{ 873, { "27F233F7D4CE4E9EFE040E3D665B7B0643557B6E", "102883" } }, //Retaliation
	{ 19426, { "10BABF2708CA3575730AC662A2E72EC292565B08", "598887" } }, //Torment
	{ 718, { "F69996772B9E18FD18AD0AABAB25D7E3FC42F261", "102835" } }, //Regeneration
	{ 17495, { "F69996772B9E18FD18AD0AABAB25D7E3FC42F261", "102835" } }, //Regeneration
	{ 17674, { "F69996772B9E18FD18AD0AABAB25D7E3FC42F261", "102835" } } //Regeneration
};

void GW2_SCT::SkillIconManager::init() {
	internalInit();
	Options::get()->skillIconsEnabled.onAssign([=](const bool& wasEnabled, const bool& isNowEnabled) {
		if (wasEnabled == isNowEnabled) return;
		if (isNowEnabled) {
			std::thread t(SkillIconManager::internalInit);
			t.detach();
		}
		else {
			if (loadThread.joinable()) {
				keepLoadThreadRunning = false;
				loadThread.detach();
			}
		}
	});
	Options::get()->preloadAllSkillIcons.onAssign([=](const bool& wasEnabled, const bool& isNowEnabled) {
		if (wasEnabled == isNowEnabled) return;
		if (isNowEnabled) {
			std::thread t([]() {
				requestedIDs->clear();
				if (Options::get()->preloadAllSkillIcons) {
					auto s_checkedIDs = sf::slock_safe_ptr(checkedIDs);
					for (
						auto checkableIDIterator = s_checkedIDs->begin();
						checkableIDIterator != s_checkedIDs->end();
						checkableIDIterator++) {
						if (!checkableIDIterator->second) {
							requestedIDs->push_back(checkableIDIterator->first);
						}
					}
				}
			});
			t.detach();
		} else {
			// TODO: stop preload Thread
		}
	});
}

void GW2_SCT::SkillIconManager::cleanup() {
	if (loadThread.joinable()) {
		keepLoadThreadRunning = false;
		loadThread.join();
	}
}

void GW2_SCT::SkillIconManager::internalInit() {
	try {
		if (Options::get()->skillIconsEnabled) {
			std::regex matcher("X-Rate-Limit-Limit: ([0-9]+)");
			std::vector<int> skillIdList = getJSON("/v2/skills", [](httplib::Headers headers) {
				auto foundHeader = headers.find("X-Rate-Limit-Limit");
				if (foundHeader != headers.end()) {
					requestsPerMinute = std::min(std::stoi(foundHeader->second), requestsPerMinute);
				}
			});
			bool preload = Options::get()->preloadAllSkillIcons;
			checkedIDs->clear();
			for (const auto& skillId: skillIdList) {
				checkedIDs->insert({ skillId, false });
				if (preload) {
					requestedIDs->push_back(skillId);
				}
			}
			for (auto it : staticFiles) {
				checkedIDs->insert({ it.first, false });
			}

			spawnLoadThread();
		}
	} catch (std::exception& e) {
		LOG("Skill icon thread error: ", e.what());
	}
}

void GW2_SCT::SkillIconManager::spawnLoadThread() {
	if (loadThread.joinable()) {
		keepLoadThreadRunning = false;
		loadThread.join();
	}
	keepLoadThreadRunning = true;
	loadThread = std::thread(GW2_SCT::SkillIconManager::loadThreadCycle);
}


template <typename Range, typename Value = typename Range::value_type>
std::string join(Range const& elements, const char* const delimiter) {
	std::ostringstream os;
	auto b = begin(elements), e = end(elements);

	if (b != e) {
		std::copy(b, prev(e), std::ostream_iterator<Value>(os, delimiter));
		b = prev(e);
	}
	if (b != e) {
		os << *b;
	}

	return os.str();
}

void GW2_SCT::SkillIconManager::loadThreadCycle() {
	LOG("Skillicon load thread started");
	std::vector<std::string> files;
	std::string iconPath = getSCTPath() + "icons\\";
	CreateDirectory(iconPath.c_str(), NULL);
	if (getFilesInDirectory(iconPath, files)) {
		for (std::string iconFile : files) {
			size_t itDot = iconFile.find_last_of(".");
			if (iconFile.substr(itDot + 1) == "jpg") {
				std::string fileName = iconFile.substr(0, itDot);
				uint32_t skillID = std::strtoul(fileName.c_str(), NULL, 10);
				if (skillID != 0) {
					loadedIcons->insert(std::pair<uint32_t, SkillIcon>(skillID, SkillIcon(loadBinaryFileData(iconPath + iconFile), skillID)));
					{
						std::lock_guard<decltype(requestedIDs)> lock(requestedIDs);
						if (std::find(requestedIDs->begin(), requestedIDs->end(), skillID) == requestedIDs->end()) {
							requestedIDs->push_back(skillID);
						}
					}
				}
			}
		}
	}

	std::string skillIniFilename = getSCTPath() + "skill.ini";

	CSimpleIniA skillIni;
	SI_Error rc = skillIni.LoadFile(skillIniFilename.c_str());
	if (rc < 0) {
		LOG("Warning: could not find a skill.ini");
	}

	std::regex renderAPIURLMatcher("/file/([A-Z0-9]+)/([0-9]+)\\.(png|jpg)");
	while (keepLoadThreadRunning) {
		if (requestedIDs->size() > 0) {
			std::list<std::tuple<uint32_t, std::string, std::string>> loadableIconURLs;
			std::vector<uint32_t> idListToRequestFromApi = {};
			while (requestedIDs->size() > 0 && idListToRequestFromApi.size() <= 10) {
				int frontRequestedSkillId = requestedIDs->front();
				requestedIDs->pop_front();

				auto iteratorToFoundStaticFileInformation = staticFiles.find(frontRequestedSkillId);
				if (iteratorToFoundStaticFileInformation != staticFiles.end()) {
					loadableIconURLs.push_back({ iteratorToFoundStaticFileInformation->first, iteratorToFoundStaticFileInformation->second.first, iteratorToFoundStaticFileInformation->second.second });
				} else {
					idListToRequestFromApi.push_back(frontRequestedSkillId);
				}
			}
			if (idListToRequestFromApi.size() > 0) {
				std::string idStringToRequestFromApi = join(idListToRequestFromApi, ",");
				try {
					std::this_thread::sleep_for(std::chrono::milliseconds(60000 / requestsPerMinute));
					nlohmann::json possibleSkillInformationList = getJSON("/v2/skills?ids=" + idStringToRequestFromApi);
					if (possibleSkillInformationList.is_array()) {
						std::vector<nlohmann::json> skillInformationList = possibleSkillInformationList;
						for (auto& skillInformation : skillInformationList) {
							std::string renderingApiURL = skillInformation["icon"];
							std::string signature = std::regex_replace(renderingApiURL, renderAPIURLMatcher, "$1", std::regex_constants::format_no_copy);
							std::string fileId = std::regex_replace(renderingApiURL, renderAPIURLMatcher, "$2", std::regex_constants::format_no_copy);
							loadableIconURLs.push_back(std::make_tuple(skillInformation["id"], signature, fileId));
						}
					}
				}
				catch (std::exception& e) {
					LOG("Could not receive skill information for IDs: ", idStringToRequestFromApi, "(", e.what(), ")");
					for (auto& unresolvedSkillId : idListToRequestFromApi) {
						requestedIDs->push_back(unresolvedSkillId);
					}
				}
			}

			std::list<std::pair<uint32_t, std::shared_ptr<std::vector<BYTE>>>> binaryLoadedIcons;
			for (std::list<std::tuple<uint32_t, std::string, std::string>>::iterator it = loadableIconURLs.begin(); it != loadableIconURLs.end(); ++it) {
				uint32_t curSkillId = std::get<0>(*it);
				try {
					std::string desc = std::get<1>(*it) + "/" + std::get<2>(*it);
					std::string iniKey = "Signature_" + std::to_string(curSkillId);
					std::string iniVal(skillIni.GetValue("Icon_Signatures", iniKey.c_str(), ""));
					std::string curImagePath = iconPath + std::to_string(curSkillId) + ".jpg";
					if (iniVal.compare(desc) != 0 || !std::filesystem::exists(curImagePath.c_str())) {
						std::this_thread::sleep_for(std::chrono::milliseconds(60000 / requestsPerMinute));
						LOG("Downloading skill icon: ", curSkillId);
						std::ofstream fileStream(curImagePath, std::ofstream::binary);

						renderingClient.Get(("/file/" + desc).c_str(), [&](const char* data, size_t data_length) {
							for (const char* i = data; i < data + data_length; i++) {
								fileStream << (int8_t)*i;
							}
							return true;
						});

						fileStream.close();
						LOG("Finished downloading skill icon.");
						binaryLoadedIcons.push_back(std::pair<uint32_t, std::shared_ptr<std::vector<BYTE>>>(curSkillId, loadBinaryFileData(curImagePath)));
						skillIni.SetValue("Icon_Signatures", iniKey.c_str(), desc.c_str());
					}
				}
				catch (std::exception& e) {
					LOG("Error during downloading skill icon image for skill ", std::to_string(curSkillId), ": ", e.what());
					requestedIDs->push_back(curSkillId);
				}
			}
			while (binaryLoadedIcons.size() > 0) {
				(*checkedIDs)[binaryLoadedIcons.front().first] = true;
				loadedIcons->insert(std::pair<uint32_t, SkillIcon>(binaryLoadedIcons.front().first, SkillIcon(binaryLoadedIcons.front().second, binaryLoadedIcons.front().first)));
				binaryLoadedIcons.pop_front();
			}
		} else {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	skillIni.SaveFile(skillIniFilename.c_str());
	LOG("Skillicon load thread stopping");
}

GW2_SCT::SkillIcon* GW2_SCT::SkillIconManager::getIcon(uint32_t skillID) {
	if (Options::get()->skillIconsEnabled) {
		if (loadedIcons->find(skillID) != loadedIcons->end()) {
			SkillIcon* ret = &(loadedIcons->at(skillID));
			return ret;
		}
		if (!Options::get()->preloadAllSkillIcons) {
			if (!(*checkedIDs)[skillID]) {
				requestedIDs->push_back(skillID);
				return nullptr;
			}
		}
	}
	return nullptr;
}

ImVec2 GW2_SCT::SkillIcon::draw(ImVec2 pos, ImVec2 size, ImU32 color) {
	if (!textureCreated) {
		loadTexture();
	}
	if (texture == nullptr) {
		return ImVec2(0,0);
	}

	ImVec2 end(pos.x + size.x, pos.y + size.y);
	if (texture != nullptr) {
		texture->draw(pos, size, color);
	} else {
		return ImVec2(0, 0);
	}
	return size;
}

GW2_SCT::SkillIcon::SkillIcon(std::shared_ptr<std::vector<BYTE>> fileData, uint32_t skillID)
	: fileData(fileData), skillID(skillID), texture(nullptr) {}

GW2_SCT::SkillIcon::~SkillIcon() {
	if (texture != nullptr) ImmutableTexture::Release(texture);
}

struct HexCharStruct {
	unsigned char c;
	HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs) {
	return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c) {
	return HexCharStruct(_c);
}

void logImageData(std::shared_ptr<std::vector<BYTE>> data) {
	std::stringstream str;
	int i = 0;
	for (std::vector<BYTE>::iterator it = data->begin(); it != data->end(); it++) {
		str << " 0x" << std::setfill('0') << std::setw(2) << hex(*it);
		i++;
		if (i >= 16) {
			std::string out = str.str();
			out.erase(out.begin());
			LOG(out);
			str = std::stringstream();
			i = 0;
		}
	}
}

void logImageData(unsigned char* data, size_t size) {
	std::stringstream str;
	int i = 0;
	while (i < size) {
		str << " 0x" << std::setfill('0') << std::setw(2) << hex(*(data + i));
		i++;
		if (i % 16 == 0) {
			std::string out = str.str();
			out.erase(out.begin());
			LOG(out);
			str = std::stringstream();
		}
	}
}

void convertRGBAToARGB(unsigned char* image_data, int image_width, int image_height) {
	unsigned char* cur = image_data;
	for (int y = 0; y < image_height; y++) {
		for (int x = 0; x < image_width; x++) {
			char red = cur[0];
			cur[0] = cur[2];
			cur[2] = red;
			// TODO: possibile feature
			/*
			if (cur[0] == 0x00 && cur[1] == 0x00 && cur[2] == 0x00) {
				cur[3] = 0x00;
			}
			*/
			cur += 4;
		}
	}
}

void GW2_SCT::SkillIcon::loadTexture() {
	textureCreated = true;

	if (fileData->size() < 100) {
		LOG("Icon: ", std::to_string(skillID));
		LOG("WARNING - loaded file for texture too small: ", fileData->size());
		logImageData(fileData);
	}

	// Load from file
	int image_width = 0;
	int image_height = 0;
	int nBpp = 4; // Bytes per pixel
	unsigned char* image_data = stbi_load_from_memory(fileData->data(), (int)fileData->size(), &image_width, &image_height, NULL, nBpp);
	if (image_data == NULL) {
		texture = nullptr;
		LOG("stbi_load_from_memory image_data == NULL");
		logImageData(fileData);
		return;
	}

	convertRGBAToARGB(image_data, image_width, image_height);
	
	// Upload texture to graphics system
	if (texture != nullptr) ImmutableTexture::Release(texture);
	texture = ImmutableTexture::Create(image_width, image_height, image_data);
	if (texture == nullptr) {
		LOG("Could not upload skill icon data to graphics system.");
	}

	stbi_image_free(image_data);
}
