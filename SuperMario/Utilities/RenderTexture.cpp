#include "stdafx.h"
#include "RenderTexture.h"

void RenderTexture::Initalize()
{
	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	ZeroMemory(&rtvDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	textureDesc.Width = (UINT)gWinWidth;
	textureDesc.Height = (UINT)gWinHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT hr = DEVICE->CreateTexture2D(&textureDesc, nullptr, &renderTargetTexture);
	CHECK(hr);

	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	hr = DEVICE->CreateRenderTargetView(renderTargetTexture.Get(), &rtvDesc, &rtv);
	CHECK(hr);

	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	hr = DEVICE->CreateShaderResourceView(renderTargetTexture.Get(), &srvDesc, &srv);
	CHECK(hr);
}

void RenderTexture::Update()
{
}

void RenderTexture::RenderToTexture()
{
	DC->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
	DC->ClearRenderTargetView(rtv.Get(), GRAPHICS->GetClearColor());
}
