#pragma once

class ColorRect : public Drawable
{
public:
	ColorRect(const Vector2& position, const Vector2& scale, float rotation, const Color& color = RED);

public:
	void Update() override;
	void Render() override;

private:
	// IA
	vector<Vertex> vertices;

	vector<UINT> indices;
	unique_ptr<IndexBuffer> IB;
};
