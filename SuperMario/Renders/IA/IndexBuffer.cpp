#include "stdafx.h"
#include "IndexBuffer.h"

void IndexBuffer::Create(const vector<UINT>& indices, const D3D11_USAGE& usage)
{
	stride = sizeof(UINT);	// stride : 보폭
	count = (UINT)indices.size();

	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = stride * count;			// 내가 담을 총 바이트 수, 전체 크기
	desc.Usage = usage;							// 어떤 용도로 활용할 것인지
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;	// 어디에 어떤 용도로 바인딩할 것인지

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
		break;
	case D3D11_USAGE_IMMUTABLE:
		desc.CPUAccessFlags = 0;
		break;
	case D3D11_USAGE_DYNAMIC:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
		break;
	}

	D3D11_SUBRESOURCE_DATA subData = { 0 };
	subData.pSysMem = indices.data();

	HRESULT hr = DEVICE->CreateBuffer(&desc, &subData, &buffer);
	CHECK(hr);
}

void IndexBuffer::SetIA()
{
	DC->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, offset);
}
