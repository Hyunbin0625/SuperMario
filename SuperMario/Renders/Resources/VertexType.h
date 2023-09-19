#pragma once

struct Vertex
{
	Vertex() = default;
	Vertex(const Vector2& position) : position(position) {}

	Vector2 position;

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

// 점점의 위치, 색을 담고 있는 구조체
struct VertexColor
{
	VertexColor() = default;
	VertexColor(const Vector2& position, const Color& color)
		: position(position), color(color)
	{}

	Vector2 position;
	Color color;

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexTexture
{
	VertexTexture() = default;
	VertexTexture(const Vector2& position, const Vector2& uv)
		: position(position), uv(uv)
	{}

	Vector2 position;
	Vector2 uv;

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};