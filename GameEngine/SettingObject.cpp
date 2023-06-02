#include "SettingObject.h"
#include"Engine/ResourceManager/Model.h"
SettingObject::SettingObject(GameObject* parent,std::string name)
	:GameObject(parent,name)
{
}
SettingObject::SettingObject(GameObject* parent)
	:GameObject(parent, "SettingObject"),
	hModel_(-1),
	isLoaded(false)
{
}
SettingObject::~SettingObject()
{
}

void SettingObject::SetSettingObjectName(const std::string& name)
{
	objectName_ = name;
}

void SettingObject::DrawModel()
{
	if (hModel_ >= 0)
	{
		ModelManager::SetTransform(hModel_, transform_);
		ModelManager::Draw(hModel_);
	}
}

void SettingObject::LoadModel(std::string modelName)
{
	if (isLoaded == false)
	{
		hModel_ = ModelManager::Load(modelName);
		isLoaded = true;
	}
}