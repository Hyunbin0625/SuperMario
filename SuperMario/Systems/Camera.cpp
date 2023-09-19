#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	VPBuffer = make_unique<ViewProjectionBuffer>();

	eye = { position.x, position.y, 0 };
	at = { position.x, position.y, 1 };
	view = XMMatrixLookAtLH(eye, at, up);
	projection = XMMatrixOrthographicOffCenterLH(LB.x, RT.x, LB.y, RT.y, 0, 1);

	VPBuffer->SetView(view);
	VPBuffer->SetProjection(projection);
}

Camera::~Camera() {}

void Camera::Update()
{
	if (INPUT->Press('W'))
		Move(Vector2(0, speed));
	if (INPUT->Press('S'))
		Move(Vector2(0, -speed));
	if (INPUT->Press('A'))
		Move(Vector2(-speed, 0));
	if (INPUT->Press('D'))
		Move(Vector2(speed, 0));

	if (INPUT->Press(VK_CONTROL) && INPUT->Press('U'))
		ZoomIn();
	if (INPUT->Press(VK_CONTROL) && INPUT->Press('O'))
		ZoomOut();

	if (INPUT->Down(VK_F8))
		InitZoom();
}

void Camera::Render()
{
	VPBuffer->SetVSBuffer(1);	// VP 1(hlsl¿¡¼­)
}

void Camera::UpdateView()
{
	eye = { position.x, position.y, 0 };
	at = { position.x, position.y, 1 };
	view = XMMatrixLookAtLH(eye, at, up);

	VPBuffer->SetView(view);
}

void Camera::UpdateProj()
{
	projection = XMMatrixOrthographicOffCenterLH(LB.x, RT.x, LB.y, RT.y, 0, 1);

	VPBuffer->SetProjection(projection);
}

void Camera::ZoomIn()
{
	if (LB.y + zoomSpeed * DELTA >= RT.y - zoomSpeed * DELTA)
		return;
	LB.x += zoomSpeed * DELTA * widthRatio;
	LB.y += zoomSpeed * DELTA;
	RT.x -= zoomSpeed * DELTA * widthRatio;
	RT.y -= zoomSpeed * DELTA;

	UpdateProj();
}

void Camera::ZoomOut()
{
	LB.x -= zoomSpeed * DELTA * widthRatio;
	LB.y -= zoomSpeed * DELTA;
	RT.x += zoomSpeed * DELTA * widthRatio;
	RT.y += zoomSpeed * DELTA;

	UpdateProj();
}

void Camera::InitZoom()
{
	LB = { 0, 0 };
	RT = { WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT };

	UpdateProj();
}

Vector2 Camera::UnProject(const Vector2& source)
{
	Vector2 output;
	output = (source / Vector2(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT) * 2.0f) - Vector2(1, 1);

	Matrix VPInverse = (view * projection).Invert();

	return XMVector2TransformCoord(output, VPInverse);
}

void Camera::SetPosition(const Vector2& position)
{
	this->position = position;

	if (bEdge)
	{
		if (this->position.x < minEdge.x)
			this->position.x = minEdge.x;
		else if (this->position.x + WIN_DEFAULT_WIDTH > maxEdge.x)
			this->position.x = maxEdge.x - WIN_DEFAULT_WIDTH;

		if (this->position.y < minEdge.y)
			this->position.y = minEdge.y;
		else if (this->position.y + WIN_DEFAULT_HEIGHT > maxEdge.y)
			this->position.y = maxEdge.y - WIN_DEFAULT_HEIGHT;
	}

	UpdateView();
}

void Camera::Move(const Vector2& position)
{
	this->position += position * DELTA;

	if (bEdge)
	{
		if (this->position.x < minEdge.x)
			this->position.x = minEdge.x;
		else if (this->position.x + WIN_DEFAULT_WIDTH > maxEdge.x)
			this->position.x = maxEdge.x - WIN_DEFAULT_WIDTH;

		if (this->position.y < minEdge.y)
			this->position.y = minEdge.y;
		else if (this->position.y + WIN_DEFAULT_HEIGHT > maxEdge.y)
			this->position.y = maxEdge.y - WIN_DEFAULT_HEIGHT;
	}

	UpdateView();
}
