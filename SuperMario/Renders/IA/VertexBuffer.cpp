#include "stdafx.h"
#include "VertexBuffer.h"

void VertexBuffer::SetIA()
{
	DC->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &stride, &offset);
}
