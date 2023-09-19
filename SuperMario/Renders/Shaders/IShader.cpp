#include "stdafx.h"
#include "IShader.h"

void IShader::CompileShader(const wstring& path, const string& entryName, const string& profile, ID3DBlob** blob)
{
	ComPtr<ID3DBlob> error;

	HRESULT hr = D3DCompileFromFile
	(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryName.c_str(),
		profile.c_str(),
		D3DCOMPILE_ENABLE_STRICTNESS,	// STRICTNESS : 엄격함, 레거시 구문을 허용하지 않음
		0,
		blob,
		&error
	);

	CheckShaderError(hr, error);
}

void IShader::CheckShaderError(const HRESULT& hr, const ComPtr<ID3DBlob>& error)
{
	// hlsl에 문제가 있는지, 컴파일러에 문제가 있는지 check
	if (FAILED(hr))	// 실패 check
	{
		if (error.Get())	// 에러를 가지고 있다면
		{
			const string& str = (const char*)error->GetBufferPointer();	// 에러 위치가 이진형태로 저장되어 있는 시작점의 위치를 const char*로 형변환해 저장
			MessageBoxA(nullptr, str.c_str(), "Shader Error", MB_OK);
		}
		assert(false);
	}
}
