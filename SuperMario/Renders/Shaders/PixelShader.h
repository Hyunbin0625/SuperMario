#pragma once

class PixelShader : public IShader
{
public:
	void Create(const wstring& path, const string& entryName) override;
	void Clear() override;
	void SetShader() override;

public:
	ID3DBlob* GetBlob() const { return blob.Get(); }
	ID3D11PixelShader* GetResource() const { return shader.Get(); }

private:
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3D11PixelShader> shader;
};
