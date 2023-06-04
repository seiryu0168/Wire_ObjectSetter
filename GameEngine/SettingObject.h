#pragma once
#include"Engine/GameObject/GameObject.h"
class SettingObject : public GameObject
{
private:
	int hModel_;
	std::string objectName_;
	int objectNum_;
	bool isLoaded;
public:
	SettingObject(GameObject* parent);
	SettingObject(GameObject* parent,std::string name);
	~SettingObject();
	std::string GetSettingObjectName() { return objectName_; }
	void SetSettingObjectName(const std::string& name);
	void SetObjectNum(int num) { objectNum_ = num; }
	int GetObjectNum() { return objectNum_; }
	void DrawModel();
	void LoadModel(std::string modelName);
};

