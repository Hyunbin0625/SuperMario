#pragma once

#include "stdafx.h"

class ConstantBuffer
{
protected:
	ConstantBuffer(void* data, UINT dataSize)
		:data(data), dataSize(dataSize)
	{
		desc.ByteWidth = dataSize;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = DEVICE->CreateBuffer(&desc, nullptr, &buffer);
		CHECK(hr);
	}

public:
	void SetVSBuffer(UINT slot)
	{
		MapData(data, dataSize);

		DC->VSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
	}

	void SetPSBuffer(UINT slot)
	{
		MapData(data, dataSize);

		DC->PSSetConstantBuffers(slot, 1, buffer.GetAddressOf());
	}

private:
	void MapData(void* data, UINT dataSize)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;

		HRESULT hr = DC->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		CHECK(hr);
		{
			memcpy(subResource.pData, data, dataSize);
		}
		DC->Unmap(buffer.Get(), 0);
	}

private:
	D3D11_BUFFER_DESC desc = { 0 };
	ComPtr<ID3D11Buffer> buffer;

	void* data;
	UINT dataSize;
};