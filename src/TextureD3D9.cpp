#include "Texture.h"
#include "Common.h"

GW2_SCT::TextureD3D9::TextureD3D9(int width, int height, unsigned char* data) {
    if (d3Device9 == nullptr) return;
    HRESULT res;
    if (FAILED(res = d3Device9->CreateTexture(width, height, 1, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &_texture9, NULL))) {
        LOG("d3Device->CreateTexture failed: " + std::to_string(res));
        _textureCreateSuccessful = false;
        return;
    }
    if (data != nullptr) {
        D3DLOCKED_RECT tex_locked_rect;
        if (FAILED(res = _texture9->LockRect(0, &tex_locked_rect, NULL, 0))) {
            _textureCreateSuccessful = false;
            LOG("texture9->LockRect failed: " + std::to_string(res));
            return;
        }
        for (int y = 0; y < height; y++)
            memcpy((unsigned char*)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, data + (width * 4) * y, width * 4);
        if (FAILED(res = _texture9->UnlockRect(0))) {
            _textureCreateSuccessful = false;
            LOG("texture9->UnlockRect failed: " + std::to_string(res));
            return;
        }
    }
    _textureCreateSuccessful = true;
}

GW2_SCT::TextureD3D9::~TextureD3D9() {
    if (_texture9 != nullptr) _texture9->Release();
}

GW2_SCT::ImmutableTextureD3D9::ImmutableTextureD3D9(int width, int height, unsigned char* data)
    : TextureD3D9(width, height, data), ImmutableTexture(width, height) {}

void GW2_SCT::ImmutableTextureD3D9::internalDraw(ImVec2 pos, ImVec2 size, ImVec2 uvStart, ImVec2 uvEnd, ImU32 color) {
    ImGui::GetWindowDrawList()->AddImage(_texture9, pos, ImVec2(pos.x + size.x, pos.y + size.y), uvStart, uvEnd, color);
}

GW2_SCT::MutableTextureD3D9::MutableTextureD3D9(int width, int height)
    : TextureD3D9(width, height, nullptr), MutableTexture(width, height) {}

void GW2_SCT::MutableTextureD3D9::internalDraw(ImVec2 pos, ImVec2 size, ImVec2 uvStart, ImVec2 uvEnd, ImU32 color) {
    ImGui::GetWindowDrawList()->AddImage(_texture9, pos, ImVec2(pos.x + size.x, pos.y + size.y), uvStart, uvEnd, color);
}

bool GW2_SCT::MutableTextureD3D9::internalStartUpdate(ImVec2 pos, ImVec2 size, UpdateData* out) {
    HRESULT res;
    D3DLOCKED_RECT lockedRect;
    RECT rectBounds { pos.x, pos.y, pos.x + size.x, pos.y + size.y };
    if (FAILED(res = _texture9->LockRect(0, &lockedRect, &rectBounds, 0))) {
        LOG("texture9->LockRect failed: " + std::to_string(res));
        return false;
    }
    out->data = (unsigned char*)lockedRect.pBits;
    out->rowPitch = lockedRect.Pitch;
    out->bytePerPixel = 4;
    return true;
}

bool GW2_SCT::MutableTextureD3D9::internalEndUpdate() {
    HRESULT res;
    if (FAILED(res = _texture9->UnlockRect(0))) {
        LOG("texture9->UnlockRect failed: " + std::to_string(res));
        return false;
    }
    return true;
}