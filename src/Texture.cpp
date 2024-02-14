#include "Texture.h"
#include "Common.h"

GW2_SCT::Texture::Texture(int width, int height) : _textureWidth(width), _textureHeight(height), _nextCreationTry(std::chrono::system_clock::now()) {}

void GW2_SCT::Texture::draw(ImVec2 pos, ImVec2 size, ImU32 color) {
    return draw(pos, size, ImVec2(0, 0), ImVec2(1, 1), color);
}

void GW2_SCT::Texture::draw(ImVec2 pos, ImVec2 size, ImVec2 uvStart, ImVec2 uvEnd, ImU32 color) {
    if (!_created && std::chrono::system_clock::now() > _nextCreationTry) {
        _created = internalCreate();
        if (!_created) {
            _creationTries++;
            _nextCreationTry = std::chrono::system_clock::now() + std::chrono::seconds(_creationTries);
        }
    }
    if (_created) {
        internalDraw(pos, size, uvStart, uvEnd, color);
    }
}

void GW2_SCT::Texture::ensureCreation() {
    if (!_created && std::chrono::system_clock::now() > _nextCreationTry) {
        _created = internalCreate();
        if (!_created) {
            _creationTries++;
            _nextCreationTry = std::chrono::system_clock::now() + std::chrono::seconds(_creationTries);
        }
    }
}

GW2_SCT::ImmutableTexture* GW2_SCT::ImmutableTexture::Create(int width, int height, unsigned char* data) {
    if (d3Device11 != nullptr) {
        return new ImmutableTextureD3D11(width, height, data);
    }
    return nullptr;
}

void GW2_SCT::ImmutableTexture::Release(ImmutableTexture* tex) {
    if (tex == nullptr) return;
    if (d3Device11 != nullptr) {
        ImmutableTextureD3D11* res = (ImmutableTextureD3D11*)tex;
        delete res;
    }
}

GW2_SCT::ImmutableTexture::ImmutableTexture(int width, int height) : Texture(width, height) {}

GW2_SCT::MutableTexture* GW2_SCT::MutableTexture::Create(int width, int height) {
    if (d3Device11 != nullptr) {
        return new MutableTextureD3D11(width, height);
    }
    return nullptr;
}

void GW2_SCT::MutableTexture::Release(MutableTexture* tex) {
    if (tex == nullptr) return;
    if (d3Device11 != nullptr) {
        MutableTextureD3D11* res = (MutableTextureD3D11*)tex;
        delete res;
    }
}

GW2_SCT::MutableTexture::MutableTexture(int width, int height) : Texture(width, height) {}

bool GW2_SCT::MutableTexture::startUpdate(ImVec2 pos, ImVec2 size, UpdateData* out) {
    if (_isCurrentlyUpdating || !_created) return false;
    _isCurrentlyUpdating = true;
    if (internalStartUpdate(pos, size, out)) {
        return true;
    } else {
        _isCurrentlyUpdating = false;
        return false;
    }
}

bool GW2_SCT::MutableTexture::endUpdate() {
    if (!_isCurrentlyUpdating) return false;
    _isCurrentlyUpdating = false;
    if (internalEndUpdate()) {
        return true;
    } else {
        _isCurrentlyUpdating = true;
        return false;
    }
}