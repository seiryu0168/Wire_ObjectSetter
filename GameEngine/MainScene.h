#pragma once
#include"Engine/GameObject/GameObject.h"
class MainScene : public GameObject
{
	GameObject* pControlObject_;
public:
	MainScene(GameObject* parent);
	~MainScene();
	void Initialize() override;
	void Update() override;
	void Release() override;

};

