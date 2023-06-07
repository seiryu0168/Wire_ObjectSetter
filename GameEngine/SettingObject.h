#pragma once
#include"Engine/GameObject/GameObject.h"

enum class OBJECT_TYPE
{
	TYPE_ENEMY = 0,
	TYPE_ITEM,
	MAX,
};
class SettingObject : public GameObject
{
private:
	int hModel_;
	std::string objectName_;
	int objectNum_;
	OBJECT_TYPE type_;
	bool isLoaded;
public:
	SettingObject(GameObject* parent);
	SettingObject(GameObject* parent,std::string name);
	~SettingObject();
	void Initialize() override;
	std::string GetSettingObjectName() { return objectName_; }
	void SetSettingObjectName(const std::string& name);
	void SetObjectNum(int num) { objectNum_ = num; }
	void SetType(OBJECT_TYPE type) { type_ = type; }
	OBJECT_TYPE GetType() { return type_; }
	int GetObjectNum() { return objectNum_; }
	void Draw() override;
	void LoadModel(std::string modelName);
	void Release() override;
};

