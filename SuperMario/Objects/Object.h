#pragma once

class Object
{
public:
	Object(const string& name, const Vector2& position, const Vector2& scale, float rotation)
		: name(name), position(position), scale(scale), rotation(XMConvertToRadians(rotation))
	{
		WB = make_unique<WorldBuffer>();
	}
	virtual ~Object() = default;

public:
	void AddComponent(const shared_ptr<Component>& component);

	template <typename T>
	shared_ptr<T> GetComponent(const string& componentName) const
	{
		if (!components.count(componentName))	// 없을 경우
			return nullptr;				// nullptr 반환
			
		return dynamic_pointer_cast<T>(components.at(componentName));
	}

public:
	Vector2 GetPosition() const { return position; }
	Vector2 GetScale() const { return scale; }
	float GetRotation() const { return rotation; }
	float GetRotationDegree() const { return XMConvertToDegrees(rotation); }
	Vector2 GetRightVector() const { return Vector2(R._11, R._12); }	// 방향 벡터
	Vector2 GetUpVector() const { return Vector2(R._21, R._22); }
	Matrix GetS() const { return S; }
	Matrix GetR() const { return R; }
	Matrix GetT() const { return T; }
	string GetName() { return name; }

	void SetPosition(const Vector2& position) { this->position = position; }
	void SetScale(const Vector2& scale) { this->scale = scale; }
	void SetRotation(float rotation) { this->rotation = XMConvertToRadians(rotation); }

	void SetWorld(const Matrix& world)
	{
		this->world = world;
		WB->SetWorld(world);
	}

public:
	void Move(const Vector2& position) { this->position += position * DELTA; }
	void Scale(const Vector2& scale) { this->scale += scale * DELTA; }
	void Rotate(float rotation) { this->rotation += XMConvertToRadians(rotation) * DELTA; }

public:
	virtual void Update();
	virtual void Render();

	virtual void GUI();

protected:
	string name = "";
	map<string, shared_ptr<Component>> components;

	unique_ptr<WorldBuffer> WB;
	Matrix world, S, R, T;

	Vector2 position, scale;
	float rotation = 0.0f;
};
