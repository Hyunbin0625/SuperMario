#include "stdafx.h"
#include "AnimationRect.h"

AnimationRect::AnimationRect(const Vector2& position, const Vector2& scale, float rotation, const wstring& texturePath)
	: TextureRect(position, scale, rotation, texturePath)
{
	VB->Create(vertices, D3D11_USAGE_DYNAMIC);
}

void AnimationRect::Update()
{
	SUPER::Update();

	animator->Update();

	D3D11_MAPPED_SUBRESOURCE subResource;
	HRESULT hr = DC->Map(VB->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	CHECK(hr);
	{
		vertices[0].uv = Vector2(animator->GetCurrentFrame().x, animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y);
		vertices[1].uv = animator->GetCurrentFrame();
		vertices[2].uv = animator->GetCurrentFrame() + animator->GetTexelFrameSize();
		vertices[3].uv = Vector2(animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x, animator->GetCurrentFrame().y);

		memcpy(subResource.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());
	}
	DC->Unmap(VB->GetResource(), 0);
}

void AnimationRect::Render()
{
	SUPER::Render();
}
