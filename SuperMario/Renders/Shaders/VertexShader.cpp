#include "stdafx.h"
#include "VertexShader.h"

void VertexShader::Create(const wstring& path, const string& entryName)
{
	this->path = path;
	this->entryName = entryName;

	CompileShader(path, entryName, "vs_5_0", &blob);

	HRESULT hr = DEVICE->CreateVertexShader
	(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		&shader
	);
	CHECK(hr);
}

void VertexShader::Clear()
{
	shader.Reset();
	blob.Reset();
}

void VertexShader::SetShader()
{
	DC->VSSetShader(shader.Get(), nullptr, 0);
}
