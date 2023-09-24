#include "stdafx.h"
#include "LineRect.h"

LineRect::LineRect(const Vector2& position, const Vector2& scale, float rotation, const Color& color)
	: Drawable("LineRect", position, scale, rotation, L"_Shaders/Vertex.hlsl")
{
	vertices.assign(4, Vertex());
	vertices[0].position = Vector2(-0.5f, -0.5f);
	vertices[1].position = Vector2(-0.5f, 0.5f);
	vertices[2].position = Vector2(0.5f, -0.5f);
	vertices[3].position = Vector2(0.5f, 0.5f);

	VB->Create(vertices, D3D11_USAGE_IMMUTABLE);

	indices = { 0, 1, 3, 2, 0, 3 };

	IB = make_unique<IndexBuffer>();
	IB->Create(indices, D3D11_USAGE_IMMUTABLE);

	IL->Create(Vertex::descs, VS->GetBlob());

	AddComponent(make_shared<ColorComponent>(color, 0));
}

void LineRect::Update()
{
	SUPER::Update();
}

void LineRect::Render()
{
	SUPER::Render();

	IB->SetIA();

	DrawCall(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, IB->GetCount());
}
