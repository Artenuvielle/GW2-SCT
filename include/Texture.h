#pragma once
#include "imgui.h"
#include <D3dx9tex.h>
#include <D3dx11tex.h>

namespace GW2_SCT {
	class CreatedTexture {
	protected:
		bool _textureCreateSuccessful = false;
	};

	class Texture : protected virtual CreatedTexture {
	public:
		Texture(int width, int height);
		void draw(ImVec2 pos, ImVec2 size, ImU32 color);
		void draw(ImVec2 pos, ImVec2 size, ImVec2 uvStart, ImVec2 uvEnd, ImU32 color);
		virtual void internalDraw(ImVec2 pos, ImVec2 size, ImVec2 uvStart, ImVec2 uvEnd, ImU32 color) = 0;
	protected:
		int _textureWidth;
		int _textureHeight;
	};

	class ImmutableTexture : public Texture {
	public:
		static ImmutableTexture* Create(int width, int height, unsigned char* data);
		static void Release(ImmutableTexture*);
		ImmutableTexture(int width, int height);
	};

	class MutableTexture : public Texture {
	public:
		typedef struct {
			int id;
			unsigned char* data;
			int rowPitch;
			int bytePerPixel;
		} UpdateData;
		static MutableTexture* Create(int width, int height);
		static void Release(MutableTexture*);
		MutableTexture(int width, int height);
		bool startUpdate(ImVec2 pos, ImVec2 size, UpdateData* out);
		bool endUpdate();
		virtual bool internalStartUpdate(ImVec2 pos, ImVec2 size, UpdateData* out) = 0;
		virtual bool internalEndUpdate() = 0;
	private:
		bool _isCurrentlyUpdating = false;
	};

	class TextureD3D9 : protected virtual CreatedTexture {
	public:
		TextureD3D9(int width, int height, unsigned char* data);
		~TextureD3D9();
	protected:
		LPDIRECT3DTEXTURE9 _texture9 = nullptr;
	};
	class ImmutableTextureD3D9 : public ImmutableTexture, public TextureD3D9 {
	public:
		ImmutableTextureD3D9(int width, int height, unsigned char* data);
	protected:
		void internalDraw(ImVec2 pos, ImVec2 size, ImVec2 uvStart, ImVec2 uvEnd, ImU32 color) override;
	};

	class MutableTextureD3D9 : public MutableTexture, public TextureD3D9 {
	public:
		MutableTextureD3D9(int width, int height);
	protected:
		void internalDraw(ImVec2 pos, ImVec2 size, ImVec2 uvStart, ImVec2 uvEnd, ImU32 color) override;
		bool internalStartUpdate(ImVec2 pos, ImVec2 size, UpdateData* out) override;
		bool internalEndUpdate() override;
	};

	class TextureD3D11 : protected virtual CreatedTexture {
	public:
		TextureD3D11(int width, int height, unsigned char* data);
		~TextureD3D11();
	protected:
		ID3D11Texture2D* _texture11 = nullptr;
		ID3D11ShaderResourceView* _texture11View = nullptr;
	};
	class ImmutableTextureD3D11 : public ImmutableTexture, public TextureD3D11 {
	public:
		ImmutableTextureD3D11(int width, int height, unsigned char* data);
	protected:
		void internalDraw(ImVec2 pos, ImVec2 size, ImVec2 uvStart, ImVec2 uvEnd, ImU32 color) override;
	};

	class MutableTextureD3D11 : public MutableTexture, public TextureD3D11 {
	public:
		MutableTextureD3D11(int width, int height);
		~MutableTextureD3D11();
	protected:
		void internalDraw(ImVec2 pos, ImVec2 size, ImVec2 uvStart, ImVec2 uvEnd, ImU32 color) override;
		bool internalStartUpdate(ImVec2 pos, ImVec2 size, UpdateData* out) override;
		bool internalEndUpdate() override;
	private:
		ID3D11Texture2D* _texture11Staging = nullptr;
		bool _stagingChanged = false;
	};
}
