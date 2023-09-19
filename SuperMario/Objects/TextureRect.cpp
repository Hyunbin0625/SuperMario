#include "stdafx.h"
#include "TextureRect.h"

TextureRect::TextureRect(const Vector2& position, const Vector2& scale, float rotation, const wstring& texturePath, UINT slot)
	: Drawable("TextureRect", position, scale, rotation, L"_Shaders/VertexTexture.hlsl"), texturePath(texturePath), slot(slot)
{
	vertices.assign(4, VertexTexture());
	vertices[0].position = Vector2(-0.5f, -0.5f);
	vertices[1].position = Vector2(-0.5f, 0.5f);
	vertices[2].position = Vector2(0.5f, -0.5f);
	vertices[3].position = Vector2(0.5f, 0.5f);

	vertices[0].uv = Vector2(0.0f,  1.0f);
	vertices[1].uv = Vector2(0.0f, 0.0f);
	vertices[2].uv = Vector2(1.0f, 1.0f);
	vertices[3].uv = Vector2(1.0f, 0.0f);

	VB->Create(vertices, D3D11_USAGE_IMMUTABLE);

	indices = { 0, 1, 2, 2, 1, 3 };

	IB = make_unique<IndexBuffer>();
	IB->Create(indices, D3D11_USAGE_IMMUTABLE);

	IL->Create(VertexTexture::descs, VS->GetBlob());

	if (texturePath != L"")
		SetSRV(texturePath);

	OB = make_unique<OutlineBuffer>();

	// CreateBlendState
	{
		D3D11_BLEND_DESC desc;
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = DEVICE->CreateBlendState(&desc, &blendState);
		CHECK(hr);
	}
}

void TextureRect::SetSRV(const wstring& texturePath)
{
	this->texturePath = texturePath;

	HRESULT hr = LoadFromWICFile(texturePath.c_str(), WIC_FLAGS_IGNORE_SRGB, nullptr, image);
	CHECK(hr);

	hr = CreateShaderResourceView(DEVICE.Get(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &srv);
	CHECK(hr);
}

void TextureRect::Update()
{
	SUPER::Update();
}

void TextureRect::Render()
{
	SUPER::Render();

	IB->SetIA();

	DC->PSSetShaderResources(slot, 1, srv.GetAddressOf());

	if (Path::GetExtension(texturePath) == L"png")
		DC->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);

	OB->SetPSBuffer(0);

	DrawCall(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, IB->GetCount());

	DC->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void TextureRect::GUI(int ordinal)
{
	string objName = name + to_string(ordinal);
	string imgName = "Image : " + String::ToString(Path::GetFileName(texturePath));
	string shaderName = "Shader : " + String::ToString(Path::GetFileName(shaderPath));

	if (ImGui::BeginMenu(objName.c_str()))
	{
		SetOutline(true);

		ImGui::Text(objName.c_str());
		ImGui::Text(imgName.c_str());
		ImGui::Text(shaderName.c_str());
		
		SUPER::GUI();

		if (ImGui::Button("ChangImage", ImVec2(100, 30)))
			ChangeImageFunc();
		if (ImGui::Button("ChangShader", ImVec2(100, 30)))
			ChangeShaderFunc();

		if (ImGui::Button("Delete", ImVec2(50, 30)))
			bDelete = true;

		ImGui::EndMenu();
	}
	else
	{
		SetOutline(false);
	}
}

void TextureRect::ChangeImageFunc(const wstring& path)
{
	if (path.empty())
	{
		function<void(wstring)> func = bind(&TextureRect::ChangeImageFunc, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::ImageFilter, L"_Textures/", func, gHandle);
	}
	else
	{
		wstring tempPath = path.substr(path.find(L"_Textures/"));
		SetSRV(tempPath);
	}
}

void TextureRect::ChangeShaderFunc(const wstring& path)
{
	if (path.empty())
	{
		function<void(wstring)> func = bind(&TextureRect::ChangeShaderFunc, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::ShaderFilter, L"_Shaders/", func, gHandle);
	}
	else
	{
		wstring tempPath = path.substr(path.find(L"_Shaders/"));
		SetShader(tempPath);
	}
}
