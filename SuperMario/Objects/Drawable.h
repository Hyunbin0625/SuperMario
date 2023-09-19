#pragma once

class Drawable : public Object
{
public:
	Drawable(const string& name, const Vector2& position, const Vector2& scale, float rotation, const wstring& shaderPath = L"");
	virtual ~Drawable() = default;

public:
	void SetShader(const wstring& shaderPath);

public:
	virtual void Update() override;		// virtual 명시적 의미
	virtual void Render() override;

	void DrawCall(const D3D11_PRIMITIVE_TOPOLOGY& topology);
	void DrawCall(const D3D11_PRIMITIVE_TOPOLOGY& topology, UINT indexCount);

protected:
	unique_ptr<VertexBuffer> VB;

	unique_ptr<VertexShader> VS;

	unique_ptr<InputLayout> IL;

	unique_ptr<PixelShader> PS;

	wstring shaderPath = L"";
};
