#pragma once

#include "stdafx.h"

class Object;
class Component
{
public:
	Component(const string& compName) : name(compName) {}
	virtual ~Component() = default;

public:
	Object* GetOwner() const { return owner; }
	string GetName() const { return name; }

	void SetOwner(Object* obj) { owner = obj; }
	
public:
	virtual void Update() {}	// 순수 가상함수를 사용해도 된다.
	virtual void Render() {}

protected:
	string name = "";
	Object* owner = nullptr;	// Object Pointer
};
