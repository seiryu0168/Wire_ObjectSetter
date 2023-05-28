#pragma once
#include"Engine/GameObject/GameObject.h"
class SettingObject : public GameObject
{
private:
	int hModel_;
	std::string objectName_;
	std::string enemyType_;
public:
	SettingObject(GameObject* parent);
	SettingObject(GameObject* parent,std::string name);
	~SettingObject();
	std::string GetSettingObjectName() { return objectName_; }
	void SetSettingObjectName(const std::string& name);
	void DrawModel();
	void LoadModel(std::string modelName);
};

