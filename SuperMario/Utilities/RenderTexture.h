#pragma once

class RenderTexture
{
public:
	void Initalize();	// √ ±‚»≠

	void Update();

	void RenderToTexture();

public:
	ComPtr<ID3D11ShaderResourceView> GetSRV() const { return srv; }

private:
	ComPtr<ID3D11Texture2D> renderTargetTexture;
	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11ShaderResourceView> srv;
};
