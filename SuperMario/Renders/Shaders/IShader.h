#pragma once

class IShader	// 온전한 interface가 아닌 class
{
public:
	virtual ~IShader() = default;

	virtual void Create(const wstring& path, const string& entryName) = 0;
	virtual void Clear() = 0;
	virtual void SetShader() = 0;

protected:
	void CompileShader(const wstring& path, const string& entryName, const string& profile, ID3DBlob** blob);

private:
	void CheckShaderError(const HRESULT& hr, const ComPtr<ID3DBlob>& error);

protected:
	wstring path = L"";
	string entryName = "";
};
