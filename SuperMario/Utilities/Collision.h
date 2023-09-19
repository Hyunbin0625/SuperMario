#pragma once

class Object;

namespace Collision
{
	struct RECT
	{
		RECT(const Vector2& position, const Vector2& scale);

		Vector2 min;
		Vector2 max;
	};

	struct CIRCLE
	{
		CIRCLE(const Vector2& position, const Vector2& scale);

		Vector2 pivot;
		float radius;
	};

	// 충돌 처리 함수
	bool IntersectRectCoord(const RECT& rect, const Vector2& coord);
	bool IntersectCircleCoord(const CIRCLE& circle, const Vector2& coord);
	
	bool IntersectRectRect(const RECT& rect1, const RECT& rect2);	// AABB, 회전하지 않은 바운딩 박스
	bool IntersectRectRect(const Object& object1, const Object& object2);	// OBB, 횐전된 바운딩 박스

	bool IntersectCircleCircle(const CIRCLE& circle1, const CIRCLE& circle2);
	bool IntersectRectCircle(const RECT& rect, const CIRCLE& circle);
};
