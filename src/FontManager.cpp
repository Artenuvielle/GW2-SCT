#include "FontManager.h"
#include <iostream>
#include "Common.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define min(a,b) a <= b ? a : b
#define max(a,b) a >= b ? a : b

#define FONT_TEXTURE_SIZE 1024

static unsigned int stb_decompress_length(const unsigned char* input);
static unsigned int stb_decompress(unsigned char* output, const unsigned char* input, unsigned int length);
static const char*  GetDefaultCompressedFontDataTTFBase85();
static unsigned int Decode85Byte(char c) { return c >= '\\' ? c - 36 : c - 35; }
static void         Decode85(const unsigned char* src, unsigned char* dst)
{
	while (*src)
	{
		unsigned int tmp = Decode85Byte(src[0]) + 85 * (Decode85Byte(src[1]) + 85 * (Decode85Byte(src[2]) + 85 * (Decode85Byte(src[3]) + 85 * Decode85Byte(src[4]))));
		dst[0] = ((tmp >> 0) & 0xFF); dst[1] = ((tmp >> 8) & 0xFF); dst[2] = ((tmp >> 16) & 0xFF); dst[3] = ((tmp >> 24) & 0xFF);   // We can't assume little-endianness.
		src += 5;
		dst += 4;
	}
}

std::unordered_map<const stbtt_fontinfo*, std::unordered_map<float, std::unordered_map<int, GW2_SCT::Glyph*>>> GW2_SCT::Glyph::_knownGlyphs = {};

GW2_SCT::Glyph* GW2_SCT::Glyph::GetGlyph(const stbtt_fontinfo* font, float scale, int codepoint, int ascent) {
    auto& fontatScale = _knownGlyphs[font][scale];
    auto it = fontatScale.find(codepoint);
    if (it != fontatScale.end()) {
        return it->second;
    } else {
        return _knownGlyphs[font][scale][codepoint] = new Glyph(font, scale, codepoint, ascent);
    }
}

void GW2_SCT::Glyph::cleanup() {
    for (const auto& fontPair : _knownGlyphs) {
        for (const auto& scalePair : fontPair.second) {
            for (const auto& codepointPair : scalePair.second) {
                if (codepointPair.second != nullptr) delete codepointPair.second;
            }
            scalePair.second.empty();
        }
        fontPair.second.empty();
    }
    _knownGlyphs.empty();
}

int GW2_SCT::Glyph::getX1() { return _x1; }

int GW2_SCT::Glyph::getX2() { return _x2; }

int GW2_SCT::Glyph::getY1() { return _y1; }

int GW2_SCT::Glyph::getY2() { return _y2; }

size_t GW2_SCT::Glyph::getWidth() { return _width; }

size_t GW2_SCT::Glyph::getHeight() { return _height; }

int GW2_SCT::Glyph::getCodepoint() { return _codepoint; }

int GW2_SCT::Glyph::getOffsetTop() { return _offsetTop; }

float GW2_SCT::Glyph::getLeftSideBearing() { return _lsb; }

unsigned char* GW2_SCT::Glyph::getBitmap() {
    if (_bitmap == nullptr) {
        _bitmap = (unsigned char*)calloc(_width * _height, sizeof(unsigned char));
        stbtt_MakeCodepointBitmap(_font, _bitmap, _width, _height, _width, _scale, _scale, _codepoint);
    }
    return _bitmap;
}

float GW2_SCT::Glyph::getAdvanceAndKerning(int nextCodepoint) {
    float realAdvanceAndKerning = getRealAdvanceAndKerning(nextCodepoint);
    if (nextCodepoint == 0) return max(_width + _lsb, realAdvanceAndKerning);
    else return realAdvanceAndKerning;
}

GW2_SCT::Glyph::Glyph(const stbtt_fontinfo* font, float scale, int codepoint, int ascent) : _font(font), _scale(scale), _codepoint(codepoint) {
    stbtt_GetCodepointBitmapBox(font, codepoint, scale, scale, &_x1, &_y1, &_x2, &_y2);
    _width = (size_t)_x2 - _x1;
    _height = (size_t)_y2 - _y1;
    _offsetTop = _scale * ascent + _y1;
    stbtt_GetCodepointHMetrics(font, codepoint, &_advance, &_lsb);
    _lsb *= _scale;
}

GW2_SCT::Glyph::~Glyph() { if (_bitmap == nullptr) free(_bitmap); }

float GW2_SCT::Glyph::getRealAdvanceAndKerning(int nextCodepoint) {
    auto it = _advanceAndKerningCache.find(nextCodepoint);
    if (it != _advanceAndKerningCache.end()) {
        return it->second;
    } else {
        return _advanceAndKerningCache[nextCodepoint] = (float)(_advance + stbtt_GetCodepointKernAdvance(_font, _codepoint, nextCodepoint)) * _scale;
    }
}

std::vector<GW2_SCT::FontType::GlyphAtlas*> GW2_SCT::FontType::_allocatedAtlases;

GW2_SCT::FontType::FontType(unsigned char* data, size_t size) {
    _cachedScales = {};
    _isCachedScaleExact = {};
    _cachedRealScales = {};
    if (!stbtt_InitFont(&_info, data, 0)) {
        LOG("Failed initializing font.");
    }
    stbtt_GetFontVMetrics(&_info, &_ascent, &_descent, &_lineGap);
}

void GW2_SCT::FontType::cleanup() {
    for (auto atlas : _allocatedAtlases) {
        delete atlas;
    }
    _allocatedAtlases.empty();
}

short getCodepointLength(std::string text, size_t i) {
    int cplen = 1;
    if ((text[i] & 0xf8) == 0xf0) cplen = 4;
    else if ((text[i] & 0xf0) == 0xe0) cplen = 3;
    else if ((text[i] & 0xe0) == 0xc0) cplen = 2;
    if ((i + cplen) > text.length()) cplen = 1;

    return cplen;
}

int getCodepointOfLength(std::string text, size_t i, short length) {
    if (length == 1) return text[i];
    int ret = (short)text[i] & (0xff >> length + 1);
    for (int j = 1; j < length; j++) {
        ret = (ret << 6) + (text[i + j] & 0x3f);
    }
    return ret;
}

ImVec2 GW2_SCT::FontType::calcRequiredSpaceForTextAtSize(std::string text, float fontSize) {
    if (text.size() == 0) return ImVec2(0, ceil(fontSize));
    float scale = getRealScale(fontSize);
    float x = 0;
    int firstCodePointLength = getCodepointLength(text, 0);
    int firstCodePoint = getCodepointOfLength(text, 0, firstCodePointLength);
    Glyph* g = Glyph::GetGlyph(&_info, scale, firstCodePoint, _ascent);
    x += ceil(-g->getLeftSideBearing());
    for (size_t i = 0; i < text.size();) {
        int codePointLength = getCodepointLength(text, i);
        int codePoint = getCodepointOfLength(text, i, codePointLength);
        i += codePointLength;
        g = Glyph::GetGlyph(&_info, scale, codePoint, _ascent);
        int nextCodePointLength = getCodepointLength(text, i);
        int nextCodePoint = getCodepointOfLength(text, i, nextCodePointLength);
        x += g->getAdvanceAndKerning(nextCodePoint);
    }
    return ImVec2(ceil(x), ceil(fontSize));
}

std::vector<int> getCodepointsWithoutDuplicates(std::string str) {
    std::vector<int> codePointsWithoutDuplicates;
    int i;
    for (i = 0; i < str.size();) {
        int codePointLength = getCodepointLength(str, i);
        int codePoint = getCodepointOfLength(str, i, codePointLength);
        if (std::find(codePointsWithoutDuplicates.begin(), codePointsWithoutDuplicates.end(), codePoint) == codePointsWithoutDuplicates.end())
            codePointsWithoutDuplicates.push_back(codePoint);
        i += codePointLength;
    }
    return codePointsWithoutDuplicates;
}

void GW2_SCT::FontType::bakeGlyphsAtSize(std::string text, float fontSize) {
    float scale = getCachedScale(fontSize);
    std::vector<int> codepointsWithoutDuplicates = getCodepointsWithoutDuplicates(text);
    for (const auto& codePoint : codepointsWithoutDuplicates) {
        if (_glyphPositionsAtSizes[scale].find(codePoint) == _glyphPositionsAtSizes[scale].end()) {
            Glyph* glyph = Glyph::GetGlyph(&_info, scale, codePoint, _ascent);
            size_t atlasId = 0;
            ImVec2 atlasPosition;
            bool positionFound = false;

            while (!positionFound && atlasId < _allocatedAtlases.size()) {
                for (auto &lineDefinition : _allocatedAtlases[atlasId]->linesWithSpaces) {
                    if (fontSize <= lineDefinition.lineHeight && lineDefinition.nextGlyphPosition.x + glyph->getWidth() < FONT_TEXTURE_SIZE) {
                        positionFound = true;
                        atlasPosition = lineDefinition.nextGlyphPosition;
                        lineDefinition.nextGlyphPosition.x += glyph->getWidth();
                        break;
                    }
                }
                if (!positionFound) {
                    float nextYAfterLastLine = _allocatedAtlases[atlasId]->linesWithSpaces.back().nextGlyphPosition.y + _allocatedAtlases[atlasId]->linesWithSpaces.back().lineHeight;
                    if (nextYAfterLastLine + fontSize < FONT_TEXTURE_SIZE) {
                        positionFound = true;
                        _allocatedAtlases[atlasId]->linesWithSpaces.push_back({ fontSize, ImVec2(glyph->getWidth(), nextYAfterLastLine) });
                        atlasPosition = ImVec2(0, nextYAfterLastLine);
                    } else {
                        atlasId++;
                    }
                }
            }
            if (atlasId == _allocatedAtlases.size()) {
                GlyphAtlas* atlas = new GlyphAtlas();
                atlas->linesWithSpaces.push_back({fontSize, ImVec2(glyph->getWidth(), 0)});
                _allocatedAtlases.push_back(atlas);
            }

        

            _glyphPositionsAtSizes[scale][codePoint] = GlyphPositionDefinition(atlasId, atlasPosition.x, atlasPosition.y, glyph);
            if (codePoint != 32 && _allocatedAtlases[atlasId]->texture != nullptr) {
                MutableTexture::UpdateData area;
                if (!_allocatedAtlases[atlasId]->texture->startUpdate(
                    _glyphPositionsAtSizes[scale][codePoint].getPos(),
                    _glyphPositionsAtSizes[scale][codePoint].getSize(),
                    &area)
                ) {
                    LOG("Could not begin updating font atlas.");
                    _glyphPositionsAtSizes[scale].erase(codePoint);
                }
                else {
                    unsigned char* currentPosition = nullptr;
                    for (size_t y = 0; y < glyph->getHeight(); y++) {
                        currentPosition = area.data + y * area.rowPitch;
                        for (int x = 0; x < glyph->getWidth(); x++) {
                            currentPosition[0] = 0xff;
                            currentPosition[1] = 0xff;
                            currentPosition[2] = 0xff;
                            currentPosition[3] = glyph->getBitmap()[y * glyph->getWidth() + x];
                            currentPosition += 4;
                        }
                    }
                    _allocatedAtlases[atlasId]->texture->endUpdate();
                }
            }
        }
    }
}

void GW2_SCT::FontType::drawAtSize(std::string text, float fontSize, ImVec2 pos, ImU32 color) {
    if (text.size() == 0) return;
    std::vector<GlyphPositionDefinition*> definitions;
    float scale = getCachedScale(fontSize);
    for (size_t i = 0; i < text.size();) {
        int codePointLength = getCodepointLength(text, i);
        int codePoint = getCodepointOfLength(text, i, codePointLength);
        i += codePointLength;
        auto it = _glyphPositionsAtSizes[scale].find(codePoint);
        if (it != _glyphPositionsAtSizes[scale].end()) {
            definitions.push_back(&it->second);
        }
    }
    if (definitions.size() == 0) return;
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 currentPos(pos.x - definitions[0]->glyph->getLeftSideBearing(), pos.y);
    if (isCachedScaleExactForSize(fontSize)) {
        for (size_t i = 0; i < definitions.size(); i++) {
            GlyphPositionDefinition* def = definitions[i];
            if (_allocatedAtlases[def->atlasID]->texture != nullptr) {
                ImVec2 thisCharPos(ceil(currentPos.x + def->glyph->getLeftSideBearing()), ceil(currentPos.y + def->glyph->getOffsetTop()));
                _allocatedAtlases[def->atlasID]->texture->draw(thisCharPos, def->getSize(), def->uvStart, def->uvEnd, color);
                currentPos.x += def->glyph->getAdvanceAndKerning(i + 1 >= definitions.size() ? 0 : definitions[i + 1]->glyph->getCodepoint());
            }
        }
    } else {
        float realScaleFraction = getRealScale(fontSize) / scale;
        for (size_t i = 0; i < definitions.size(); i++) {
            GlyphPositionDefinition* def = definitions[i];
            if (_allocatedAtlases[def->atlasID]->texture != nullptr) {
                ImVec2 thisCharPos(ceil(currentPos.x + realScaleFraction * def->glyph->getLeftSideBearing()), ceil(currentPos.y + realScaleFraction * def->glyph->getOffsetTop()));
                _allocatedAtlases[def->atlasID]->texture->draw(thisCharPos, def->getSize(realScaleFraction), def->uvStart, def->uvEnd, color);
                currentPos.x += realScaleFraction * def->glyph->getAdvanceAndKerning(i + 1 >= definitions.size() ? 0 : definitions[i + 1]->glyph->getCodepoint());
            }
        }
    }
}

#if _DEBUG
void GW2_SCT::FontType::drawAtlas() {
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 pos(0, 0);
    for (int i = 0; i < _allocatedAtlases.size(); i++) {
        ImVec2 size(FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE);
        ImVec2 end(pos.x + FONT_TEXTURE_SIZE, pos.y + FONT_TEXTURE_SIZE);
        dl->AddRectFilled(pos, end, ImGui::GetColorU32(ImVec4(.15f, .15f, .15f, .66f)));
        dl->AddRect(pos, end, ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, .66f)));
        if (_allocatedAtlases[i]->texture != nullptr) {
            _allocatedAtlases[i]->texture->draw(pos, size, ImGui::GetColorU32(ImVec4(1.f, 1.f, 1.f, 1.f)));
        }
        if (end.x + FONT_TEXTURE_SIZE > windowWidth) {
            pos = ImVec2(0, pos.y + FONT_TEXTURE_SIZE);
        }
    }
}
#endif

float GW2_SCT::FontType::getCachedScale(float fontSize) {
    auto it = _cachedScales.find(fontSize);
    if (it != _cachedScales.end()) {
        return it->second;
    } else {
        for (auto& scale : _cachedScales) {
            if (isCachedScaleExactForSize(scale.first) && scale.first * 0.75 <= fontSize && scale.first * 1.05 >= fontSize) {
                _cachedScales[fontSize] = scale.second;
                _isCachedScaleExact[fontSize] = false;
                _cachedRealScales[fontSize] = stbtt_ScaleForPixelHeight(&_info, fontSize);
                return scale.second;
            }
        }
        _cachedScales[fontSize] = stbtt_ScaleForPixelHeight(&_info, fontSize);
        _isCachedScaleExact[fontSize] = true;
        _cachedRealScales[fontSize] = _cachedScales[fontSize];
        return _cachedScales[fontSize];
    }
}

bool GW2_SCT::FontType::isCachedScaleExactForSize(float fontSize) {
    auto it = _isCachedScaleExact.find(fontSize);
    if (it != _isCachedScaleExact.end()) {
        return it->second;
    } else {
        return _isCachedScaleExact[fontSize] = false;
    }
}

float GW2_SCT::FontType::getRealScale(float fontSize) {
    auto it = _cachedRealScales.find(fontSize);
    if (it != _cachedRealScales.end()) {
        return it->second;
    } else {
        _isCachedScaleExact[fontSize] = false;
        return _cachedRealScales[fontSize] = stbtt_ScaleForPixelHeight(&_info, fontSize);
    }
}


D3D11_TEXTURE2D_DESC desc = {
    FONT_TEXTURE_SIZE,
    FONT_TEXTURE_SIZE,
    1,
    1,
    DXGI_FORMAT_B8G8R8A8_UNORM,
    { 1 },
    D3D11_USAGE_DEFAULT,
    D3D11_BIND_SHADER_RESOURCE,
    0,
};
D3D11_TEXTURE2D_DESC desc_staging = {
    FONT_TEXTURE_SIZE,
    FONT_TEXTURE_SIZE,
    1,
    1,
    DXGI_FORMAT_B8G8R8A8_UNORM,
    { 1 },
    D3D11_USAGE_STAGING,
    0,
    D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
};

GW2_SCT::FontType::GlyphAtlas::GlyphAtlas() {
    if (texture != nullptr) MutableTexture::Release(texture);
    texture = MutableTexture::Create(FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE);
}

GW2_SCT::FontType::GlyphAtlas::~GlyphAtlas() {
    if (texture != nullptr) MutableTexture::Release(texture);
}

GW2_SCT::FontType::GlyphPositionDefinition::GlyphPositionDefinition(size_t atlasID, int x, int y, Glyph* glyph) :
    atlasID(atlasID), x(x), y(y), glyph(glyph) {
    uvStart = ImVec2((float)x / (float)FONT_TEXTURE_SIZE, (float)y / (float)FONT_TEXTURE_SIZE);
    uvEnd = ImVec2((float)(x + glyph->getWidth()) / (float)FONT_TEXTURE_SIZE, (float)(y + glyph->getHeight()) / (float)FONT_TEXTURE_SIZE);
}

ImVec2 GW2_SCT::FontType::GlyphPositionDefinition::getPos() {
    return ImVec2(x, y);
}

ImVec2 GW2_SCT::FontType::GlyphPositionDefinition::getSize() {
    return ImVec2(glyph->getWidth(), glyph->getHeight());
}

ImVec2 GW2_SCT::FontType::GlyphPositionDefinition::getSize(float scale) {
    return ImVec2(scale * glyph->getWidth(), scale * glyph->getHeight());
}

ImVec2 GW2_SCT::FontType::GlyphPositionDefinition::offsetFrom(ImVec2 origin) {
    return ImVec2(origin.x + glyph->getWidth(), origin.y + glyph->getHeight());
}

ImVec2 GW2_SCT::FontType::GlyphPositionDefinition::offsetFrom(ImVec2 origin, float scale) {
    return ImVec2(origin.x + scale * glyph->getWidth(), origin.y + scale * glyph->getHeight());
}

void GW2_SCT::FontManager::init() {
	std::vector<std::string> fontFiles = std::vector<std::string>();
	std::string addonPath = getSCTPath();

	std::string fontsDirectory = addonPath + "fonts\\";
	CreateDirectory(fontsDirectory.c_str(), NULL);

    LOG("loading default font");
    defaultFont = FontManager::getDefaultFont();
	fontMap = std::map<int, std::pair<std::string, FontType*>>();
	fontMap.insert(std::pair<int, std::pair<std::string, FontType*>>(0, std::pair<std::string, FontType*>("Default", defaultFont)));
	if (getFilesInDirectory(fontsDirectory, fontFiles)) {
		int numFonts = 1;
		for (std::vector<std::string>::iterator it = fontFiles.begin(); it != fontFiles.end(); ++it) {
			std::string fontFilePath = fontsDirectory + *it;
			LOG("loading: ", fontFilePath);
			FontType* fontType = nullptr;
			try {
				fontType = FontManager::loadFontTypeFromFile(fontFilePath);
			} catch (std::exception& e) {
				LOG("Error loading font: ", e.what())
			}
			if (fontType != nullptr) {
				fontMap.insert(std::pair<int, std::pair<std::string, FontType*>>(numFonts, std::pair<std::string, FontType*>(std::string(*it), fontType)));
			}
			numFonts++;
		}
	}
}

void GW2_SCT::FontManager::cleanup() {
    fontMap.clear();
    Glyph::cleanup();
    FontType::cleanup();
}

GW2_SCT::FontType* GW2_SCT::FontManager::DEFAULT_FONT = nullptr;

GW2_SCT::FontType* GW2_SCT::FontManager::getDefaultFont() {
    if (DEFAULT_FONT == nullptr) {
        const char* compressed_ttf_data_base85 = GetDefaultCompressedFontDataTTFBase85();
        int compressed_ttf_size = (((int)strlen(compressed_ttf_data_base85) + 4) / 5) * 4;
        void* compressed_ttf_data = malloc((size_t)compressed_ttf_size);
        Decode85((const unsigned char*)compressed_ttf_data_base85, (unsigned char*)compressed_ttf_data);

        const unsigned int buf_decompressed_size = stb_decompress_length((const unsigned char*)compressed_ttf_data);
        unsigned char* buf_decompressed_data = (unsigned char*)malloc(buf_decompressed_size);
        stb_decompress(buf_decompressed_data, (const unsigned char*)compressed_ttf_data, (unsigned int)compressed_ttf_size);
        free(compressed_ttf_data);

        DEFAULT_FONT = new FontType(buf_decompressed_data, buf_decompressed_size);
    }
    return DEFAULT_FONT;
}

GW2_SCT::FontType* GW2_SCT::FontManager::loadFontTypeFromFile(std::string path)
{
    std::ifstream ifs(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char>* bytes = new std::vector<char>(fileSize);
    ifs.read(bytes->data(), fileSize);
    FontType* ret = new FontType((unsigned char*)bytes->data(), fileSize);
    return ret;
}

//-----------------------------------------------------------------------------
// [SECTION] Decompression code
//-----------------------------------------------------------------------------
// Compressed with stb_compress() then converted to a C array and encoded as base85.
// Use the program in misc/fonts/binary_to_compressed_c.cpp to create the array from a TTF file.
// The purpose of encoding as base85 instead of "0x00,0x01,..." style is only save on _source code_ size.
// Decompression from stb.h (public domain) by Sean Barrett https://github.com/nothings/stb/blob/master/stb.h
//-----------------------------------------------------------------------------

static unsigned int stb_decompress_length(const unsigned char* input) {
    return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}

static unsigned char* stb__barrier_out_e, * stb__barrier_out_b;
static const unsigned char* stb__barrier_in_b;
static unsigned char* stb__dout;
static void stb__match(const unsigned char* data, unsigned int length) {
    // INVERSE of memmove... write each byte before copying the next...
    IM_ASSERT(stb__dout + length <= stb__barrier_out_e);
    if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
    if (data < stb__barrier_out_b) { stb__dout = stb__barrier_out_e + 1; return; }
    while (length--) *stb__dout++ = *data++;
}

static void stb__lit(const unsigned char* data, unsigned int length) {
    IM_ASSERT(stb__dout + length <= stb__barrier_out_e);
    if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
    if (data < stb__barrier_in_b) { stb__dout = stb__barrier_out_e + 1; return; }
    memcpy(stb__dout, data, length);
    stb__dout += length;
}

#define stb__in2(x)   ((i[x] << 8) + i[(x)+1])
#define stb__in3(x)   ((i[x] << 16) + stb__in2((x)+1))
#define stb__in4(x)   ((i[x] << 24) + stb__in3((x)+1))

static const unsigned char* stb_decompress_token(const unsigned char* i) {
    if (*i >= 0x20) { // use fewer if's for cases that expand small
        if (*i >= 0x80)       stb__match(stb__dout - i[1] - 1, i[0] - 0x80 + 1), i += 2;
        else if (*i >= 0x40)  stb__match(stb__dout - (stb__in2(0) - 0x4000 + 1), i[2] + 1), i += 3;
        else /* *i >= 0x20 */ stb__lit(i + 1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
    }
    else { // more ifs for cases that expand large, since overhead is amortized
        if (*i >= 0x18)       stb__match(stb__dout - (stb__in3(0) - 0x180000 + 1), i[3] + 1), i += 4;
        else if (*i >= 0x10)  stb__match(stb__dout - (stb__in3(0) - 0x100000 + 1), stb__in2(3) + 1), i += 5;
        else if (*i >= 0x08)  stb__lit(i + 2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
        else if (*i == 0x07)  stb__lit(i + 3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
        else if (*i == 0x06)  stb__match(stb__dout - (stb__in3(1) + 1), i[4] + 1), i += 5;
        else if (*i == 0x04)  stb__match(stb__dout - (stb__in3(1) + 1), stb__in2(4) + 1), i += 6;
    }
    return i;
}

static unsigned int stb_adler32(unsigned int adler32, unsigned char* buffer, unsigned int buflen) {
    const unsigned long ADLER_MOD = 65521;
    unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
    unsigned long blocklen = buflen % 5552;

    unsigned long i;
    while (buflen) {
        for (i = 0; i + 7 < blocklen; i += 8) {
            s1 += buffer[0], s2 += s1;
            s1 += buffer[1], s2 += s1;
            s1 += buffer[2], s2 += s1;
            s1 += buffer[3], s2 += s1;
            s1 += buffer[4], s2 += s1;
            s1 += buffer[5], s2 += s1;
            s1 += buffer[6], s2 += s1;
            s1 += buffer[7], s2 += s1;

            buffer += 8;
        }

        for (; i < blocklen; ++i)
            s1 += *buffer++, s2 += s1;

        s1 %= ADLER_MOD, s2 %= ADLER_MOD;
        buflen -= blocklen;
        blocklen = 5552;
    }
    return (unsigned int)(s2 << 16) + (unsigned int)s1;
}

static unsigned int stb_decompress(unsigned char* output, const unsigned char* i, unsigned int /*length*/) {
    if (stb__in4(0) != 0x57bC0000) return 0;
    if (stb__in4(4) != 0)          return 0; // error! stream is > 4GB
    const unsigned int olen = stb_decompress_length(i);
    stb__barrier_in_b = i;
    stb__barrier_out_e = output + olen;
    stb__barrier_out_b = output;
    i += 16;

    stb__dout = output;
    for (;;) {
        const unsigned char* old_i = i;
        i = stb_decompress_token(i);
        if (i == old_i) {
            if (*i == 0x05 && i[1] == 0xfa) {
                IM_ASSERT(stb__dout == output + olen);
                if (stb__dout != output + olen) return 0;
                if (stb_adler32(1, output, olen) != (unsigned int)stb__in4(2))
                    return 0;
                return olen;
            }
            else {
                IM_ASSERT(0); /* NOTREACHED */
                return 0;
            }
        }
        IM_ASSERT(stb__dout <= output + olen);
        if (stb__dout > output + olen)
            return 0;
    }
}

//-----------------------------------------------------------------------------
// [SECTION] Default font data (ProggyClean.ttf)
//-----------------------------------------------------------------------------
// ProggyClean.ttf
// Copyright (c) 2004, 2005 Tristan Grimmer
// MIT license (see License.txt in http://www.upperbounds.net/download/ProggyClean.ttf.zip)
// Download and more information at http://upperbounds.net
//-----------------------------------------------------------------------------
// File: 'ProggyClean.ttf' (41208 bytes)
// Exported using misc/fonts/binary_to_compressed_c.cpp (with compression + base85 string encoding).
// The purpose of encoding as base85 instead of "0x00,0x01,..." style is only save on _source code_ size.
//-----------------------------------------------------------------------------
static const char proggy_clean_ttf_compressed_data_base85[11980 + 1] =
"7])#######hV0qs'/###[),##/l:$#Q6>##5[n42>c-TH`->>#/e>11NNV=Bv(*:.F?uu#(gRU.o0XGH`$vhLG1hxt9?W`#,5LsCp#-i>.r$<$6pD>Lb';9Crc6tgXmKVeU2cD4Eo3R/"
"2*>]b(MC;$jPfY.;h^`IWM9<Lh2TlS+f-s$o6Q<BWH`YiU.xfLq$N;$0iR/GX:U(jcW2p/W*q?-qmnUCI;jHSAiFWM.R*kU@C=GH?a9wp8f$e.-4^Qg1)Q-GL(lf(r/7GrRgwV%MS=C#"
"`8ND>Qo#t'X#(v#Y9w0#1D$CIf;W'#pWUPXOuxXuU(H9M(1<q-UE31#^-V'8IRUo7Qf./L>=Ke$$'5F%)]0^#0X@U.a<r:QLtFsLcL6##lOj)#.Y5<-R&KgLwqJfLgN&;Q?gI^#DY2uL"
"i@^rMl9t=cWq6##weg>$FBjVQTSDgEKnIS7EM9>ZY9w0#L;>>#Mx&4Mvt//L[MkA#W@lK.N'[0#7RL_&#w+F%HtG9M#XL`N&.,GM4Pg;-<nLENhvx>-VsM.M0rJfLH2eTM`*oJMHRC`N"
"kfimM2J,W-jXS:)r0wK#@Fge$U>`w'N7G#$#fB#$E^$#:9:hk+eOe--6x)F7*E%?76%^GMHePW-Z5l'&GiF#$956:rS?dA#fiK:)Yr+`&#0j@'DbG&#^$PG.Ll+DNa<XCMKEV*N)LN/N"
"*b=%Q6pia-Xg8I$<MR&,VdJe$<(7G;Ckl'&hF;;$<_=X(b.RS%%)###MPBuuE1V:v&cX&#2m#(&cV]`k9OhLMbn%s$G2,B$BfD3X*sp5#l,$R#]x_X1xKX%b5U*[r5iMfUo9U`N99hG)"
"tm+/Us9pG)XPu`<0s-)WTt(gCRxIg(%6sfh=ktMKn3j)<6<b5Sk_/0(^]AaN#(p/L>&VZ>1i%h1S9u5o@YaaW$e+b<TWFn/Z:Oh(Cx2$lNEoN^e)#CFY@@I;BOQ*sRwZtZxRcU7uW6CX"
"ow0i(?$Q[cjOd[P4d)]>ROPOpxTO7Stwi1::iB1q)C_=dV26J;2,]7op$]uQr@_V7$q^%lQwtuHY]=DX,n3L#0PHDO4f9>dC@O>HBuKPpP*E,N+b3L#lpR/MrTEH.IAQk.a>D[.e;mc."
"x]Ip.PH^'/aqUO/$1WxLoW0[iLA<QT;5HKD+@qQ'NQ(3_PLhE48R.qAPSwQ0/WK?Z,[x?-J;jQTWA0X@KJ(_Y8N-:/M74:/-ZpKrUss?d#dZq]DAbkU*JqkL+nwX@@47`5>w=4h(9.`G"
"CRUxHPeR`5Mjol(dUWxZa(>STrPkrJiWx`5U7F#.g*jrohGg`cg:lSTvEY/EV_7H4Q9[Z%cnv;JQYZ5q.l7Zeas:HOIZOB?G<Nald$qs]@]L<J7bR*>gv:[7MI2k).'2($5FNP&EQ(,)"
"U]W]+fh18.vsai00);D3@4ku5P?DP8aJt+;qUM]=+b'8@;mViBKx0DE[-auGl8:PJ&Dj+M6OC]O^((##]`0i)drT;-7X`=-H3[igUnPG-NZlo.#k@h#=Ork$m>a>$-?Tm$UV(?#P6YY#"
"'/###xe7q.73rI3*pP/$1>s9)W,JrM7SN]'/4C#v$U`0#V.[0>xQsH$fEmPMgY2u7Kh(G%siIfLSoS+MK2eTM$=5,M8p`A.;_R%#u[K#$x4AG8.kK/HSB==-'Ie/QTtG?-.*^N-4B/ZM"
"_3YlQC7(p7q)&](`6_c)$/*JL(L-^(]$wIM`dPtOdGA,U3:w2M-0<q-]L_?^)1vw'.,MRsqVr.L;aN&#/EgJ)PBc[-f>+WomX2u7lqM2iEumMTcsF?-aT=Z-97UEnXglEn1K-bnEO`gu"
"Ft(c%=;Am_Qs@jLooI&NX;]0#j4#F14;gl8-GQpgwhrq8'=l_f-b49'UOqkLu7-##oDY2L(te+Mch&gLYtJ,MEtJfLh'x'M=$CS-ZZ%P]8bZ>#S?YY#%Q&q'3^Fw&?D)UDNrocM3A76/"
"/oL?#h7gl85[qW/NDOk%16ij;+:1a'iNIdb-ou8.P*w,v5#EI$TWS>Pot-R*H'-SEpA:g)f+O$%%`kA#G=8RMmG1&O`>to8bC]T&$,n.LoO>29sp3dt-52U%VM#q7'DHpg+#Z9%H[K<L"
"%a2E-grWVM3@2=-k22tL]4$##6We'8UJCKE[d_=%wI;'6X-GsLX4j^SgJ$##R*w,vP3wK#iiW&#*h^D&R?jp7+/u&#(AP##XU8c$fSYW-J95_-Dp[g9wcO&#M-h1OcJlc-*vpw0xUX&#"
"OQFKNX@QI'IoPp7nb,QU//MQ&ZDkKP)X<WSVL(68uVl&#c'[0#(s1X&xm$Y%B7*K:eDA323j998GXbA#pwMs-jgD$9QISB-A_(aN4xoFM^@C58D0+Q+q3n0#3U1InDjF682-SjMXJK)("
"h$hxua_K]ul92%'BOU&#BRRh-slg8KDlr:%L71Ka:.A;%YULjDPmL<LYs8i#XwJOYaKPKc1h:'9Ke,g)b),78=I39B;xiY$bgGw-&.Zi9InXDuYa%G*f2Bq7mn9^#p1vv%#(Wi-;/Z5h"
"o;#2:;%d&#x9v68C5g?ntX0X)pT`;%pB3q7mgGN)3%(P8nTd5L7GeA-GL@+%J3u2:(Yf>et`e;)f#Km8&+DC$I46>#Kr]]u-[=99tts1.qb#q72g1WJO81q+eN'03'eM>&1XxY-caEnO"
"j%2n8)),?ILR5^.Ibn<-X-Mq7[a82Lq:F&#ce+S9wsCK*x`569E8ew'He]h:sI[2LM$[guka3ZRd6:t%IG:;$%YiJ:Nq=?eAw;/:nnDq0(CYcMpG)qLN4$##&J<j$UpK<Q4a1]MupW^-"
"sj_$%[HK%'F####QRZJ::Y3EGl4'@%FkiAOg#p[##O`gukTfBHagL<LHw%q&OV0##F=6/:chIm0@eCP8X]:kFI%hl8hgO@RcBhS-@Qb$%+m=hPDLg*%K8ln(wcf3/'DW-$.lR?n[nCH-"
"eXOONTJlh:.RYF%3'p6sq:UIMA945&^HFS87@$EP2iG<-lCO$%c`uKGD3rC$x0BL8aFn--`ke%#HMP'vh1/R&O_J9'um,.<tx[@%wsJk&bUT2`0uMv7gg#qp/ij.L56'hl;.s5CUrxjO"
"M7-##.l+Au'A&O:-T72L]P`&=;ctp'XScX*rU.>-XTt,%OVU4)S1+R-#dg0/Nn?Ku1^0f$B*P:Rowwm-`0PKjYDDM'3]d39VZHEl4,.j']Pk-M.h^&:0FACm$maq-&sgw0t7/6(^xtk%"
"LuH88Fj-ekm>GA#_>568x6(OFRl-IZp`&b,_P'$M<Jnq79VsJW/mWS*PUiq76;]/NM_>hLbxfc$mj`,O;&%W2m`Zh:/)Uetw:aJ%]K9h:TcF]u_-Sj9,VK3M.*'&0D[Ca]J9gp8,kAW]"
"%(?A%R$f<->Zts'^kn=-^@c4%-pY6qI%J%1IGxfLU9CP8cbPlXv);C=b),<2mOvP8up,UVf3839acAWAW-W?#ao/^#%KYo8fRULNd2.>%m]UK:n%r$'sw]J;5pAoO_#2mO3n,'=H5(et"
"Hg*`+RLgv>=4U8guD$I%D:W>-r5V*%j*W:Kvej.Lp$<M-SGZ':+Q_k+uvOSLiEo(<aD/K<CCc`'Lx>'?;++O'>()jLR-^u68PHm8ZFWe+ej8h:9r6L*0//c&iH&R8pRbA#Kjm%upV1g:"
"a_#Ur7FuA#(tRh#.Y5K+@?3<-8m0$PEn;J:rh6?I6uG<-`wMU'ircp0LaE_OtlMb&1#6T.#FDKu#1Lw%u%+GM+X'e?YLfjM[VO0MbuFp7;>Q&#WIo)0@F%q7c#4XAXN-U&VB<HFF*qL("
"$/V,;(kXZejWO`<[5?\?ewY(*9=%wDc;,u<'9t3W-(H1th3+G]ucQ]kLs7df($/*JL]@*t7Bu_G3_7mp7<iaQjO@.kLg;x3B0lqp7Hf,^Ze7-##@/c58Mo(3;knp0%)A7?-W+eI'o8)b<"
"nKnw'Ho8C=Y>pqB>0ie&jhZ[?iLR@@_AvA-iQC(=ksRZRVp7`.=+NpBC%rh&3]R:8XDmE5^V8O(x<<aG/1N$#FX$0V5Y6x'aErI3I$7x%E`v<-BY,)%-?Psf*l?%C3.mM(=/M0:JxG'?"
"7WhH%o'a<-80g0NBxoO(GH<dM]n.+%q@jH?f.UsJ2Ggs&4<-e47&Kl+f//9@`b+?.TeN_&B8Ss?v;^Trk;f#YvJkl&w$]>-+k?'(<S:68tq*WoDfZu';mM?8X[ma8W%*`-=;D.(nc7/;"
")g:T1=^J$&BRV(-lTmNB6xqB[@0*o.erM*<SWF]u2=st-*(6v>^](H.aREZSi,#1:[IXaZFOm<-ui#qUq2$##Ri;u75OK#(RtaW-K-F`S+cF]uN`-KMQ%rP/Xri.LRcB##=YL3BgM/3M"
"D?@f&1'BW-)Ju<L25gl8uhVm1hL$##*8###'A3/LkKW+(^rWX?5W_8g)a(m&K8P>#bmmWCMkk&#TR`C,5d>g)F;t,4:@_l8G/5h4vUd%&%950:VXD'QdWoY-F$BtUwmfe$YqL'8(PWX("
"P?^@Po3$##`MSs?DWBZ/S>+4%>fX,VWv/w'KD`LP5IbH;rTV>n3cEK8U#bX]l-/V+^lj3;vlMb&[5YQ8#pekX9JP3XUC72L,,?+Ni&co7ApnO*5NK,((W-i:$,kp'UDAO(G0Sq7MVjJs"
"bIu)'Z,*[>br5fX^:FPAWr-m2KgL<LUN098kTF&#lvo58=/vjDo;.;)Ka*hLR#/k=rKbxuV`>Q_nN6'8uTG&#1T5g)uLv:873UpTLgH+#FgpH'_o1780Ph8KmxQJ8#H72L4@768@Tm&Q"
"h4CB/5OvmA&,Q&QbUoi$a_%3M01H)4x7I^&KQVgtFnV+;[Pc>[m4k//,]1?#`VY[Jr*3&&slRfLiVZJ:]?=K3Sw=[$=uRB?3xk48@aeg<Z'<$#4H)6,>e0jT6'N#(q%.O=?2S]u*(m<-"
"V8J'(1)G][68hW$5'q[GC&5j`TE?m'esFGNRM)j,ffZ?-qx8;->g4t*:CIP/[Qap7/9'#(1sao7w-.qNUdkJ)tCF&#B^;xGvn2r9FEPFFFcL@.iFNkTve$m%#QvQS8U@)2Z+3K:AKM5i"
"sZ88+dKQ)W6>J%CL<KE>`.d*(B`-n8D9oK<Up]c$X$(,)M8Zt7/[rdkqTgl-0cuGMv'?>-XV1q['-5k'cAZ69e;D_?$ZPP&s^+7])$*$#@QYi9,5P&#9r+$%CE=68>K8r0=dSC%%(@p7"
".m7jilQ02'0-VWAg<a/''3u.=4L$Y)6k/K:_[3=&jvL<L0C/2'v:^;-DIBW,B4E68:kZ;%?8(Q8BH=kO65BW?xSG&#@uU,DS*,?.+(o(#1vCS8#CHF>TlGW'b)Tq7VT9q^*^$$.:&N@@"
"$&)WHtPm*5_rO0&e%K&#-30j(E4#'Zb.o/(Tpm$>K'f@[PvFl,hfINTNU6u'0pao7%XUp9]5.>%h`8_=VYbxuel.NTSsJfLacFu3B'lQSu/m6-Oqem8T+oE--$0a/k]uj9EwsG>%veR*"
"hv^BFpQj:K'#SJ,sB-'#](j.Lg92rTw-*n%@/;39rrJF,l#qV%OrtBeC6/,;qB3ebNW[?,Hqj2L.1NP&GjUR=1D8QaS3Up&@*9wP?+lo7b?@%'k4`p0Z$22%K3+iCZj?XJN4Nm&+YF]u"
"@-W$U%VEQ/,,>>#)D<h#`)h0:<Q6909ua+&VU%n2:cG3FJ-%@Bj-DgLr`Hw&HAKjKjseK</xKT*)B,N9X3]krc12t'pgTV(Lv-tL[xg_%=M_q7a^x?7Ubd>#%8cY#YZ?=,`Wdxu/ae&#"
"w6)R89tI#6@s'(6Bf7a&?S=^ZI_kS&ai`&=tE72L_D,;^R)7[$s<Eh#c&)q.MXI%#v9ROa5FZO%sF7q7Nwb&#ptUJ:aqJe$Sl68%.D###EC><?-aF&#RNQv>o8lKN%5/$(vdfq7+ebA#"
"u1p]ovUKW&Y%q]'>$1@-[xfn$7ZTp7mM,G,Ko7a&Gu%G[RMxJs[0MM%wci.LFDK)(<c`Q8N)jEIF*+?P2a8g%)$q]o2aH8C&<SibC/q,(e:v;-b#6[$NtDZ84Je2KNvB#$P5?tQ3nt(0"
"d=j.LQf./Ll33+(;q3L-w=8dX$#WF&uIJ@-bfI>%:_i2B5CsR8&9Z&#=mPEnm0f`<&c)QL5uJ#%u%lJj+D-r;BoF&#4DoS97h5g)E#o:&S4weDF,9^Hoe`h*L+_a*NrLW-1pG_&2UdB8"
"6e%B/:=>)N4xeW.*wft-;$'58-ESqr<b?UI(_%@[P46>#U`'6AQ]m&6/`Z>#S?YY#Vc;r7U2&326d=w&H####?TZ`*4?&.MK?LP8Vxg>$[QXc%QJv92.(Db*B)gb*BM9dM*hJMAo*c&#"
"b0v=Pjer]$gG&JXDf->'StvU7505l9$AFvgYRI^&<^b68?j#q9QX4SM'RO#&sL1IM.rJfLUAj221]d##DW=m83u5;'bYx,*Sl0hL(W;;$doB&O/TQ:(Z^xBdLjL<Lni;''X.`$#8+1GD"
":k$YUWsbn8ogh6rxZ2Z9]%nd+>V#*8U_72Lh+2Q8Cj0i:6hp&$C/:p(HK>T8Y[gHQ4`4)'$Ab(Nof%V'8hL&#<NEdtg(n'=S1A(Q1/I&4([%dM`,Iu'1:_hL>SfD07&6D<fp8dHM7/g+"
"tlPN9J*rKaPct&?'uBCem^jn%9_K)<,C5K3s=5g&GmJb*[SYq7K;TRLGCsM-$$;S%:Y@r7AK0pprpL<Lrh,q7e/%KWK:50I^+m'vi`3?%Zp+<-d+$L-Sv:@.o19n$s0&39;kn;S%BSq*"
"$3WoJSCLweV[aZ'MQIjO<7;X-X;&+dMLvu#^UsGEC9WEc[X(wI7#2.(F0jV*eZf<-Qv3J-c+J5AlrB#$p(H68LvEA'q3n0#m,[`*8Ft)FcYgEud]CWfm68,(aLA$@EFTgLXoBq/UPlp7"
":d[/;r_ix=:TF`S5H-b<LI&HY(K=h#)]Lk$K14lVfm:x$H<3^Ql<M`$OhapBnkup'D#L$Pb_`N*g]2e;X/Dtg,bsj&K#2[-:iYr'_wgH)NUIR8a1n#S?Yej'h8^58UbZd+^FKD*T@;6A"
"7aQC[K8d-(v6GI$x:T<&'Gp5Uf>@M.*J:;$-rv29'M]8qMv-tLp,'886iaC=Hb*YJoKJ,(j%K=H`K.v9HggqBIiZu'QvBT.#=)0ukruV&.)3=(^1`o*Pj4<-<aN((^7('#Z0wK#5GX@7"
"u][`*S^43933A4rl][`*O4CgLEl]v$1Q3AeF37dbXk,.)vj#x'd`;qgbQR%FW,2(?LO=s%Sc68%NP'##Aotl8x=BE#j1UD([3$M(]UI2LX3RpKN@;/#f'f/&_mt&F)XdF<9t4)Qa.*kT"
"LwQ'(TTB9.xH'>#MJ+gLq9-##@HuZPN0]u:h7.T..G:;$/Usj(T7`Q8tT72LnYl<-qx8;-HV7Q-&Xdx%1a,hC=0u+HlsV>nuIQL-5<N?)NBS)QN*_I,?&)2'IM%L3I)X((e/dl2&8'<M"
":^#M*Q+[T.Xri.LYS3v%fF`68h;b-X[/En'CR.q7E)p'/kle2HM,u;^%OKC-N+Ll%F9CF<Nf'^#t2L,;27W:0O@6##U6W7:$rJfLWHj$#)woqBefIZ.PK<b*t7ed;p*_m;4ExK#h@&]>"
"_>@kXQtMacfD.m-VAb8;IReM3$wf0''hra*so568'Ip&vRs849'MRYSp%:t:h5qSgwpEr$B>Q,;s(C#$)`svQuF$##-D,##,g68@2[T;.XSdN9Qe)rpt._K-#5wF)sP'##p#C0c%-Gb%"
"hd+<-j'Ai*x&&HMkT]C'OSl##5RG[JXaHN;d'uA#x._U;.`PU@(Z3dt4r152@:v,'R.Sj'w#0<-;kPI)FfJ&#AYJ&#//)>-k=m=*XnK$>=)72L]0I%>.G690a:$##<,);?;72#?x9+d;"
"^V'9;jY@;)br#q^YQpx:X#Te$Z^'=-=bGhLf:D6&bNwZ9-ZD#n^9HhLMr5G;']d&6'wYmTFmL<LD)F^%[tC'8;+9E#C$g%#5Y>q9wI>P(9mI[>kC-ekLC/R&CH+s'B;K-M6$EB%is00:"
"+A4[7xks.LrNk0&E)wILYF@2L'0Nb$+pv<(2.768/FrY&h$^3i&@+G%JT'<-,v`3;_)I9M^AE]CN?Cl2AZg+%4iTpT3<n-&%H%b<FDj2M<hH=&Eh<2Len$b*aTX=-8QxN)k11IM1c^j%"
"9s<L<NFSo)B?+<-(GxsF,^-Eh@$4dXhN$+#rxK8'je'D7k`e;)2pYwPA'_p9&@^18ml1^[@g4t*[JOa*[=Qp7(qJ_oOL^('7fB&Hq-:sf,sNj8xq^>$U4O]GKx'm9)b@p7YsvK3w^YR-"
"CdQ*:Ir<($u&)#(&?L9Rg3H)4fiEp^iI9O8KnTj,]H?D*r7'M;PwZ9K0E^k&-cpI;.p/6_vwoFMV<->#%Xi.LxVnrU(4&8/P+:hLSKj$#U%]49t'I:rgMi'FL@a:0Y-uA[39',(vbma*"
"hU%<-SRF`Tt:542R_VV$p@[p8DV[A,?1839FWdF<TddF<9Ah-6&9tWoDlh]&1SpGMq>Ti1O*H&#(AL8[_P%.M>v^-))qOT*F5Cq0`Ye%+$B6i:7@0IX<N+T+0MlMBPQ*Vj>SsD<U4JHY"
"8kD2)2fU/M#$e.)T4,_=8hLim[&);?UkK'-x?'(:siIfL<$pFM`i<?%W(mGDHM%>iWP,##P`%/L<eXi:@Z9C.7o=@(pXdAO/NLQ8lPl+HPOQa8wD8=^GlPa8TKI1CjhsCTSLJM'/Wl>-"
"S(qw%sf/@%#B6;/U7K]uZbi^Oc^2n<bhPmUkMw>%t<)'mEVE''n`WnJra$^TKvX5B>;_aSEK',(hwa0:i4G?.Bci.(X[?b*($,=-n<.Q%`(X=?+@Am*Js0&=3bh8K]mL<LoNs'6,'85`"
"0?t/'_U59@]ddF<#LdF<eWdF<OuN/45rY<-L@&#+fm>69=Lb,OcZV/);TTm8VI;?%OtJ<(b4mq7M6:u?KRdF<gR@2L=FNU-<b[(9c/ML3m;Z[$oF3g)GAWqpARc=<ROu7cL5l;-[A]%/"
"+fsd;l#SafT/f*W]0=O'$(Tb<[)*@e775R-:Yob%g*>l*:xP?Yb.5)%w_I?7uk5JC+FS(m#i'k.'a0i)9<7b'fs'59hq$*5Uhv##pi^8+hIEBF`nvo`;'l0.^S1<-wUK2/Coh58KKhLj"
"M=SO*rfO`+qC`W-On.=AJ56>>i2@2LH6A:&5q`?9I3@@'04&p2/LVa*T-4<-i3;M9UvZd+N7>b*eIwg:CC)c<>nO&#<IGe;__.thjZl<%w(Wk2xmp4Q@I#I9,DF]u7-P=.-_:YJ]aS@V"
"?6*C()dOp7:WL,b&3Rg/.cmM9&r^>$(>.Z-I&J(Q0Hd5Q%7Co-b`-c<N(6r@ip+AurK<m86QIth*#v;-OBqi+L7wDE-Ir8K['m+DDSLwK&/.?-V%U_%3:qKNu$_b*B-kp7NaD'QdWQPK"
"Yq[@>P)hI;*_F]u`Rb[.j8_Q/<&>uu+VsH$sM9TA%?)(vmJ80),P7E>)tjD%2L=-t#fK[%`v=Q8<FfNkgg^oIbah*#8/Qt$F&:K*-(N/'+1vMB,u()-a.VUU*#[e%gAAO(S>WlA2);Sa"
">gXm8YB`1d@K#n]76-a$U,mF<fX]idqd)<3,]J7JmW4`6]uks=4-72L(jEk+:bJ0M^q-8Dm_Z?0olP1C9Sa&H[d&c$ooQUj]Exd*3ZM@-WGW2%s',B-_M%>%Ul:#/'xoFM9QX-$.QN'>"
"[%$Z$uF6pA6Ki2O5:8w*vP1<-1`[G,)-m#>0`P&#eb#.3i)rtB61(o'$?X3B</R90;eZ]%Ncq;-Tl]#F>2Qft^ae_5tKL9MUe9b*sLEQ95C&`=G?@Mj=wh*'3E>=-<)Gt*Iw)'QG:`@I"
"wOf7&]1i'S01B+Ev/Nac#9S;=;YQpg_6U`*kVY39xK,[/6Aj7:'1Bm-_1EYfa1+o&o4hp7KN_Q(OlIo@S%;jVdn0'1<Vc52=u`3^o-n1'g4v58Hj&6_t7$##?M)c<$bgQ_'SY((-xkA#"
"Y(,p'H9rIVY-b,'%bCPF7.J<Up^,(dU1VY*5#WkTU>h19w,WQhLI)3S#f$2(eb,jr*b;3Vw]*7NH%$c4Vs,eD9>XW8?N]o+(*pgC%/72LV-u<Hp,3@e^9UB1J+ak9-TN/mhKPg+AJYd$"
"MlvAF_jCK*.O-^(63adMT->W%iewS8W6m2rtCpo'RS1R84=@paTKt)>=%&1[)*vp'u+x,VrwN;&]kuO9JDbg=pO$J*.jVe;u'm0dr9l,<*wMK*Oe=g8lV_KEBFkO'oU]^=[-792#ok,)"
"i]lR8qQ2oA8wcRCZ^7w/Njh;?.stX?Q1>S1q4Bn$)K1<-rGdO'$Wr.Lc.CG)$/*JL4tNR/,SVO3,aUw'DJN:)Ss;wGn9A32ijw%FL+Z0Fn.U9;reSq)bmI32U==5ALuG&#Vf1398/pVo"
"1*c-(aY168o<`JsSbk-,1N;$>0:OUas(3:8Z972LSfF8eb=c-;>SPw7.6hn3m`9^Xkn(r.qS[0;T%&Qc=+STRxX'q1BNk3&*eu2;&8q$&x>Q#Q7^Tf+6<(d%ZVmj2bDi%.3L2n+4W'$P"
"iDDG)g,r%+?,$@?uou5tSe2aN_AQU*<h`e-GI7)?OK2A.d7_c)?wQ5AS@DL3r#7fSkgl6-++D:'A,uq7SvlB$pcpH'q3n0#_%dY#xCpr-l<F0NR@-##FEV6NTF6##$l84N1w?AO>'IAO"
"URQ##V^Fv-XFbGM7Fl(N<3DhLGF%q.1rC$#:T__&Pi68%0xi_&[qFJ(77j_&JWoF.V735&T,[R*:xFR*K5>>#`bW-?4Ne_&6Ne_&6Ne_&n`kr-#GJcM6X;uM6X;uM(.a..^2TkL%oR(#"
";u.T%fAr%4tJ8&><1=GHZ_+m9/#H1F^R#SC#*N=BA9(D?v[UiFY>>^8p,KKF.W]L29uLkLlu/+4T<XoIB&hx=T1PcDaB&;HH+-AFr?(m9HZV)FKS8JCw;SD=6[^/DZUL`EUDf]GGlG&>"
"w$)F./^n3+rlo+DB;5sIYGNk+i1t-69Jg--0pao7Sm#K)pdHW&;LuDNH@H>#/X-TI(;P>#,Gc>#0Su>#4`1?#8lC?#<xU?#@.i?#D:%@#HF7@#LRI@#P_[@#Tkn@#Xw*A#]-=A#a9OA#"
"d<F&#*;G##.GY##2Sl##6`($#:l:$#>xL$#B.`$#F:r$#JF.%#NR@%#R_R%#Vke%#Zww%#_-4&#3^Rh%Sflr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1-d>G21&v(35>V`39V7A4=onx4"
"A1OY5EI0;6Ibgr6M$HS7Q<)58C5w,;WoA*#[%T*#`1g*#d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#;w[H#iQtA#m^0B#qjBB#uvTB##-hB#'9$C#+E6C#"
"/QHC#3^ZC#7jmC#;v)D#?,<D#C8ND#GDaD#KPsD#O]/E#g1A5#KA*1#gC17#MGd;#8(02#L-d3#rWM4#Hga1#,<w0#T.j<#O#'2#CYN1#qa^:#_4m3#o@/=#eG8=#t8J5#`+78#4uI-#"
"m3B2#SB[8#Q0@8#i[*9#iOn8#1Nm;#^sN9#qh<9#:=x-#P;K2#$%X9#bC+.#Rg;<#mN=.#MTF.#RZO.#2?)4#Y#(/#[)1/#b;L/#dAU/#0Sv;#lY$0#n`-0#sf60#(F24#wrH0#%/e0#"
"TmD<#%JSMFove:CTBEXI:<eh2g)B,3h2^G3i;#d3jD>)4kMYD4lVu`4m`:&5niUA5@(A5BA1]PBB:xlBCC=2CDLXMCEUtiCf&0g2'tN?PGT4CPGT4CPGT4CPGT4CPGT4CPGT4CPGT4CP"
"GT4CPGT4CPGT4CPGT4CPGT4CPGT4CP-qekC`.9kEg^+F$kwViFJTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5o,^<-28ZI'O?;xp"
"O?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xp;7q-#lLYI:xvD=#";

static const char* GetDefaultCompressedFontDataTTFBase85() {
	return proggy_clean_ttf_compressed_data_base85;
}
