#pragma once

class TextureRect : public Drawable
{
public:
	TextureRect(const Vector2& position, const Vector2& scale, float rotation, const wstring& texturePath = L"", UINT slot = 0);
	TextureRect(const TextureRect& other) : TextureRect(other.position, other.scale, other.rotation, other.texturePath, other.slot) {}

public:
	wstring GetPath() const { return texturePath; }
	ScratchImage* GetImage() { return &image; }
	bool GetIsDelete() { return bDelete; }

	void SetOutline(bool bOutline) { OB->SetOutline(bOutline); }

	void SetSRV(ComPtr<ID3D11ShaderResourceView> srv) { this->srv = srv; }
	void SetSRV(const wstring& texturePath);

public:
	virtual void Update() override;
	virtual void Render() override;

	virtual void GUI(int ordinal);

private:
	void ChangeImageFunc(const wstring& path = L"");
	void ChangeShaderFunc(const wstring& path = L"");

protected:
	vector<VertexTexture> vertices;

	vector<UINT> indices;
	unique_ptr<IndexBuffer> IB;

	ComPtr<ID3D11ShaderResourceView> srv;
	wstring texturePath = L"";

	ScratchImage image;

	UINT slot = 0;

	ComPtr<ID3D11BlendState> blendState;

	unique_ptr<OutlineBuffer> OB;

	bool bDelete = false;
};
