#include "Texture.h"
#include "Common.h"

GW2_SCT::Texture::Texture(int width, int height) : _textureWidth(width), _textureHeight(height) {}

void GW2_SCT::Texture::draw(ImVec2 pos, ImVec2 size, ImU32 color) {
    return draw(pos, size, ImVec2(0, 0), ImVec2(1, 1), color);
}

void GW2_SCT::Texture::draw(ImVec2 pos, ImVec2 size, ImVec2 uvStart, ImVec2 uvEnd, ImU32 color) {
    if (_textureCreateSuccessful) {
        internalDraw(pos, size, uvStart, uvEnd, color);
    }
}

GW2_SCT::ImmutableTexture* GW2_SCT::ImmutableTexture::Create(int width, int height, unsigned char* data) {
    if (d3Device9 != nullptr) {
        ImmutableTextureD3D9* res = new ImmutableTextureD3D9(width, height, data);
        if (!res->_textureCreateSuccessful) {
            delete res;
            return nullptr;
        }
        return res;
    } else if (d3Device11 != nullptr) {
        ImmutableTextureD3D11* res = new ImmutableTextureD3D11(width, height, data);
        if (!res->_textureCreateSuccessful) {
            delete res;
            return nullptr;
        }
        return res;
    }
    return nullptr;
}

void GW2_SCT::ImmutableTexture::Release(ImmutableTexture* tex) {
    if (tex == nullptr) return;
    if (d3Device9 != nullptr) {
        ImmutableTextureD3D9* res = (ImmutableTextureD3D9*)tex;
        delete res;
    }
    else if (d3Device11 != nullptr) {
        ImmutableTextureD3D11* res = (ImmutableTextureD3D11*)tex;
        delete res;
    }
}

GW2_SCT::ImmutableTexture::ImmutableTexture(int width, int height) : Texture(width, height) {}

GW2_SCT::MutableTexture* GW2_SCT::MutableTexture::Create(int width, int height)
{
    if (d3Device9 != nullptr) {
        MutableTextureD3D9* res = new MutableTextureD3D9(width, height);
        if (!res->_textureCreateSuccessful) {
            delete res;
            return nullptr;
        }
        return res;
    }
    else if (d3Device11 != nullptr) {
        MutableTextureD3D11* res = new MutableTextureD3D11(width, height);
        if (!res->_textureCreateSuccessful) {
            delete res;
            return nullptr;
        }
        return res;
    }
    return nullptr;
}

void GW2_SCT::MutableTexture::Release(MutableTexture* tex) {
    if (tex == nullptr) return;
    if (d3Device9 != nullptr) {
        MutableTextureD3D9* res = (MutableTextureD3D9*)tex;
        delete res;
    }
    else if (d3Device11 != nullptr) {
        MutableTextureD3D11* res = (MutableTextureD3D11*)tex;
        delete res;
    }
}

GW2_SCT::MutableTexture::MutableTexture(int width, int height) : Texture(width, height) {}

bool GW2_SCT::MutableTexture::startUpdate(ImVec2 pos, ImVec2 size, UpdateData* out) {
    if (!_textureCreateSuccessful || _isCurrentlyUpdating) return false;
    _isCurrentlyUpdating = true;
    if (internalStartUpdate(pos, size, out)) {
        return true;
    } else {
        _isCurrentlyUpdating = false;
        return false;
    }
}

bool GW2_SCT::MutableTexture::endUpdate() {
    if (!_textureCreateSuccessful || !_isCurrentlyUpdating) return false;
    _isCurrentlyUpdating = false;
    if (internalEndUpdate()) {
        return true;
    }
    else {
        _isCurrentlyUpdating = true;
        return false;
    }
}