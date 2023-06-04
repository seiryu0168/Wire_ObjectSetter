#pragma once

#include"Engine/GameObject/GameObject.h"
class Stage : public GameObject
{
private:
	int hModel_;
public:
	Stage(GameObject* parent);
	~Stage();
	void Initialize() override;
	void Draw() override;
	void LoadModel(std::string modelName);
	void Release() override;
};

