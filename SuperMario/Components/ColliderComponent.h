#pragma once

enum class ColliderType
{
	NONE,
	RECT,
	CIRCLE,
};

class ColliderComponent : public Component
{
public:
	ColliderComponent(const ColliderType& type = ColliderType::NONE, const string& name = "Collider") : Component(name), type(type) {}

public:
	bool Intersect(const Vector2& coord);
	bool Intersect(const shared_ptr<ColliderComponent>& other);

	void SetType(ColliderType type) { this->type = type; }

private:
	ColliderType type = ColliderType::NONE;
};
