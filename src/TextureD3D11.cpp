#include "Texture.h"
#include "Common.h"

GW2_SCT::TextureD3D11::TextureD3D11(int width, int height, unsigned char* data) {
    if (d3Device11 == nullptr) return;
    HRESULT res;
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

    if (data != nullptr) {
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = data;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;

        if (FAILED(res = d3Device11->CreateTexture2D(&desc, &subResource, &_texture11))) {
            _textureCreateSuccessful = false;
            LOG("d3Device11->CreateTexture2D failed: " + std::to_string(res));
            return;
        }
    } else {
        if (FAILED(res = d3Device11->CreateTexture2D(&desc, NULL, &_texture11))) {
            _textureCreateSuccessful = false;
            LOG("d3Device11->CreateTexture2D failed: " + std::to_string(res));
            return;
        }
    }

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	if (FAILED(res = d3Device11->CreateShaderResourceView(_texture11, &srvDesc, &_texture11View))) {
		_textureCreateSuccessful = false;
		LOG("d3Device11->CreateShaderResourceView failed: " + std::to_string(res));
        _texture11->Release();
		return;
	}
    _textureCreateSuccessful = true;
}

GW2_SCT::TextureD3D11::~TextureD3D11() {
    if (_texture11View != nullptr) _texture11View->Release();
    if (_texture11 != nullptr) _texture11->Release();
}

GW2_SCT::ImmutableTextureD3D11::ImmutableTextureD3D11(int width, int height, unsigned char* data)
    : TextureD3D11(width, height, data), ImmutableTexture(width, height) {}

void GW2_SCT::ImmutableTextureD3D11::internalDraw(ImVec2 pos, ImVec2 size, ImVec2 uvStart, ImVec2 uvEnd, ImU32 color) {
    ImGui::GetWindowDrawList()->AddImage(_texture11View, pos, ImVec2(pos.x + size.x, pos.y + size.y), uvStart, uvEnd, color);
}

GW2_SCT::MutableTextureD3D11::MutableTextureD3D11(int width, int height)
    : TextureD3D11(width, height, nullptr), MutableTexture(width, height) {
    if (d3Device11 == nullptr) return;
    HRESULT res;
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

    if (FAILED(res = d3Device11->CreateTexture2D(&desc, NULL, &_texture11Staging))) {
        _textureCreateSuccessful = false;
        LOG("d3Device11->CreateTexture2D failed: " + std::to_string(res));
        return;
    }
}

GW2_SCT::MutableTextureD3D11::~MutableTextureD3D11() {
    if (_texture11Staging != nullptr) _texture11Staging->Release();
}

void GW2_SCT::MutableTextureD3D11::internalDraw(ImVec2 pos, ImVec2 size, ImVec2 uvStart, ImVec2 uvEnd, ImU32 color) {
    if (_stagingChanged && d3D11Context != nullptr) {
        d3D11Context->CopyResource(_texture11, _texture11Staging);
        _stagingChanged = false;
    }
    ImGui::GetWindowDrawList()->AddImage(_texture11View, pos, ImVec2(pos.x + size.x, pos.y + size.y), uvStart, uvEnd, color);
}

bool GW2_SCT::MutableTextureD3D11::internalStartUpdate(ImVec2 pos, ImVec2 size, UpdateData* out) {
    if (d3D11Context == nullptr) return false;

    HRESULT res;
    D3D11_MAPPED_SUBRESOURCE mapped;
    if (FAILED(res = d3D11Context->Map(_texture11Staging, 0, D3D11_MAP_READ_WRITE, 0, &mapped))) {
        LOG("Could not map staging texture.", std::to_string(res));;
        return false;
    }
    out->data = (unsigned char*)mapped.pData;
    out->rowPitch = mapped.RowPitch;
    out->bytePerPixel = 4;
    return true;
}

bool GW2_SCT::MutableTextureD3D11::internalEndUpdate() {
    d3D11Context->Unmap(_texture11Staging, 0);
    _stagingChanged = true;
    return true;
}
