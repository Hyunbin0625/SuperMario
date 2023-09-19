#include "stdafx.h"
#include "Drawable.h"

Drawable::Drawable(const string& name, const Vector2& position, const Vector2& scale, float rotation, const wstring& shaderPath)
	: Object(name, position, scale, rotation), shaderPath(shaderPath)
{
	VB = make_unique<VertexBuffer>();

	VS = make_unique<VertexShader>();
	VS->Create(shaderPath, "VS");

	IL = make_unique<InputLayout>();

	PS = make_unique<PixelShader>();
	PS->Create(shaderPath, "PS");
}

void Drawable::SetShader(const wstring& shaderPath)
{
	this->shaderPath = shaderPath;
	VS->Create(shaderPath, "VS");
	PS->Create(shaderPath, "PS");
}

void Drawable::Update()
{
	SUPER::Update();	// 명시적 의미, Object를 사용할 수 있다. Visual Studio 문법
}

void Drawable::Render()
{
	SUPER::Render();

	VB->SetIA();
	IL->SetIA();

	VS->SetShader();
	PS->SetShader();
}

void Drawable::DrawCall(const D3D11_PRIMITIVE_TOPOLOGY& topology)
{
	DC->IASetPrimitiveTopology(topology);

	DC->Draw(VB->GetCount(), 0);
}

void Drawable::DrawCall(const D3D11_PRIMITIVE_TOPOLOGY& topology, UINT indexCount)
{
	DC->IASetPrimitiveTopology(topology);

	DC->DrawIndexed(indexCount, 0, 0);
}