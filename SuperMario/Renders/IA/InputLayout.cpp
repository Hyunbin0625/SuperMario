#include "stdafx.h"
#include "InputLayout.h"

void InputLayout::Create(const vector<D3D11_INPUT_ELEMENT_DESC> desc, const ComPtr<ID3DBlob>& blob)
{
	if (desc.empty() || !blob.Get())	// 비어있는 경우 또는 blob이 null인 경우
		assert(false);

	HRESULT hr = DEVICE->CreateInputLayout
	(
		desc.data(),
		(UINT)desc.size(),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&inputLayout
	);
	CHECK(hr);
}

void InputLayout::SetIA()
{
	DC->IASetInputLayout(inputLayout.Get());
}
