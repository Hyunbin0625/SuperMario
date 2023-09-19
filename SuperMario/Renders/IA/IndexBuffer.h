#pragma once

class IndexBuffer
{
public:
	void Create(const vector<UINT>& indices, const D3D11_USAGE& usage = D3D11_USAGE_DEFAULT);

	ID3D11Buffer* GetResource() const { return buffer.Get(); }
	UINT GetStride() const { return stride; }
	UINT GetOffset() const { return offset; }
	UINT GetCount() const { return count; }

	void SetIA();	// Render에서 하던 Set

private:
	ComPtr<ID3D11Buffer> buffer;
	UINT stride = 0;
	UINT offset = 0;
	UINT count = 0;
};
