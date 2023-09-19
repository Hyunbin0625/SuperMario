#pragma once

#include "stdafx.h"

class WorldBuffer : public ConstantBuffer
{
	struct WorldStruct
	{
		Matrix world;	// 16 배수로 해야한다. 바이트 패딩
	};

public:
	WorldBuffer() : ConstantBuffer(&data, sizeof(data)) {}

	void SetWorld(const Matrix& world)
	{
		data.world = XMMatrixTranspose(world);	// 전치 행렬을 통해 열우선 행렬로 변경 후 넘긴다.
	}

private:
	WorldStruct data;
};

class ViewProjectionBuffer : public ConstantBuffer
{
	struct ViewProjectionStruct
	{
		Matrix view;
		Matrix projection;	// 16 배수로 해야한다. 바이트 패딩
	};

public:
	ViewProjectionBuffer() : ConstantBuffer(&data, sizeof(data)) {}

	void SetView(const Matrix& view)
	{
		data.view = XMMatrixTranspose(view);	// 전치 행렬을 통해 열우선 행렬로 변경 후 넘긴다.
	}

	void SetProjection(const Matrix& projection)
	{
		data.projection = XMMatrixTranspose(projection);	// 전치 행렬을 통해 열우선 행렬로 변경 후 넘긴다.
	}

private:
	ViewProjectionStruct data;
};

class ColorBuffer : public ConstantBuffer
{
	struct ColorStruct
	{
		Color color;	// 16 배수로 해야한다. 바이트 패딩
	};

public:
	ColorBuffer() : ConstantBuffer(&data, sizeof(data)) {}

	void SetColor(const Color& color)
	{
		data.color = color;
	}

private:
	ColorStruct data;
};

class SelectionBuffer : public ConstantBuffer
{
	struct SelectionStruct
	{
		UINT selection = 1;	// 16 배수로 해야한다. 바이트 패딩
		Vector3 dummy;
	};

public:
	SelectionBuffer() : ConstantBuffer(&data, sizeof(data)) {}

	void SetSelection(const UINT& selection)
	{
		data.selection = selection;
	}

private:
	SelectionStruct data;
};

class OutlineBuffer : public ConstantBuffer
{
	struct OutlineStruct
	{
		bool bOutline = false;	// 16 배수로 해야한다. 바이트 패딩
		char dumy;
		short dumy1;
		Vector3 dummy2 = Vector3();
	};

public:
	OutlineBuffer() : ConstantBuffer(&data, sizeof(data)) {}

	void SetOutline(const bool& bOutline)
	{
		data.bOutline = bOutline;
	}

private:
	OutlineStruct data;
};