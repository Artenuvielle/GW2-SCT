#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <imgui.h>
#include "stb_truetype.h"
#include "Texture.h"

namespace GW2_SCT {
	class Glyph {
	public:
		static Glyph* GetGlyph(const stbtt_fontinfo* font, float scale, int codepoint, int ascent);
		static void cleanup();
		int getX1();
		int getX2();
		int getY1();
		int getY2();
		size_t getWidth();
		size_t getHeight();
		int getCodepoint();
		int getOffsetTop();
		float getLeftSideBearing();
		unsigned char* getBitmap();
		float getAdvanceAndKerning(int nextCodepoint);
	private:
		static std::unordered_map<const stbtt_fontinfo*, std::unordered_map<float, std::unordered_map<int,Glyph*>>> _knownGlyphs;
		Glyph(const stbtt_fontinfo* font, float scale, int codepoint, int ascent);
		~Glyph();
		const stbtt_fontinfo* _font = nullptr;
		float _scale = 0;
		int _codepoint = 0, _x1 = 0, _x2 = 0, _y1 = 0, _y2 = 0, _advance = 0, _lsb = 0, _offsetTop = 0;
		size_t _width = 0, _height = 0;
		unsigned char* _bitmap = nullptr;
		std::unordered_map<int, float> _advanceAndKerningCache;
		float getRealAdvanceAndKerning(int nextCodepoint);
	};
	class FontType {
	public:
		FontType(unsigned char* data, size_t size);
		static void ensureAtlasCreation();
		static void cleanup();
		ImVec2 calcRequiredSpaceForTextAtSize(std::string text, float fontSize);
		void bakeGlyphsAtSize(std::string text, float fontSize);
		void drawAtSize(std::string text, float fontSize, ImVec2 position, ImU32 color);
#if _DEBUG
		void drawAtlas();
#endif
	private:
		stbtt_fontinfo _info;
		int _ascent, _descent, _lineGap;
		std::unordered_map<float, float> _cachedScales;
		std::unordered_map<float, bool> _isCachedScaleExact;
		std::unordered_map<float, float> _cachedRealScales;
		float getCachedScale(float fontSize);
		bool isCachedScaleExactForSize(float fontSize);
		float getRealScale(float fontSize);

		struct GlyphAtlas {
			struct GlyphAtlasLineDefinition {
				float lineHeight;
				ImVec2 nextGlyphPosition;
			};
			MutableTexture* texture = nullptr;
			std::vector<GlyphAtlasLineDefinition> linesWithSpaces;
			GlyphAtlas();
			~GlyphAtlas();
		};
		struct GlyphPositionDefinition {
			size_t atlasID;
			int x, y;
			ImVec2 uvStart, uvEnd;
			Glyph* glyph;
			GlyphPositionDefinition(size_t atlasID, int x, int y, Glyph* glyph);
			GlyphPositionDefinition() {};
			ImVec2 getPos();
			ImVec2 getSize();
			ImVec2 getSize(float scale);
			ImVec2 offsetFrom(ImVec2 origin);
			ImVec2 offsetFrom(ImVec2 origin, float scale);
		};
		std::unordered_map<float, std::unordered_map<int, GlyphPositionDefinition>> _glyphPositionsAtSizes;

		static std::vector<GlyphAtlas*> _allocatedAtlases;
	};

	class FontManager {
	public:
		static void init();
		static void cleanup();
		static FontType* getDefaultFont();
	private:
		static FontType* loadFontTypeFromFile(std::string path);
		static FontType* DEFAULT_FONT;
	};
}
