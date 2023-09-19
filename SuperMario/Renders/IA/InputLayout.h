#pragma once

class InputLayout
{
public:
	void Create(const vector<D3D11_INPUT_ELEMENT_DESC> desc, const ComPtr<ID3DBlob>& blob);
	void SetIA();

public:
	ID3D11InputLayout* GetResource() const { return inputLayout.Get(); }

private:
	ComPtr<ID3D11InputLayout> inputLayout;
};
