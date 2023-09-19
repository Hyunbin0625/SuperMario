#include "stdafx.h"
#include "ColorRect.h"

ColorRect::ColorRect(const Vector2& position, const Vector2& scale, float rotation, const Color& color)
	: Drawable("ColorRect", position, scale, rotation, L"_Shaders/Vertex.hlsl")
{
	vertices.assign(4, Vertex());
	vertices[0].position = Vector2(-0.5f, -0.5f);
	vertices[1].position = Vector2(-0.5f, 0.5f);
	vertices[2].position = Vector2(0.5f, -0.5f);
	vertices[3].position = Vector2(0.5f, 0.5f);

	VB->Create(vertices, D3D11_USAGE_IMMUTABLE);

	indices = { 0, 1, 2, 2, 1, 3 };

	IB = make_unique<IndexBuffer>();
	IB->Create(indices, D3D11_USAGE_IMMUTABLE);

	IL->Create(Vertex::descs, VS->GetBlob());

	AddComponent(make_shared<ColorComponent>(color, 0));
}

void ColorRect::Update()
{
	SUPER::Update();
}

void ColorRect::Render()
{
	SUPER::Render();

	IB->SetIA();

	DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IB->GetCount());
}
