#pragma once
#include"Engine/GameObject/GameObject.h"
class Component
{
private:
	Component* parent_;
	GameObject* attacheObject_;
public:
	Component(GameObject* atcObj, Component* parent = nullptr);
	virtual ~Component();
	void SetParent(Component* parent);
	Component* GetParent();
	virtual void Draw() {};
	virtual void Start() {};
	virtual void Update() {};
};

