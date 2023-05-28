#include "SettingObjectManager.h"

SettingObjectManager::SettingObjectManager(GameObject* scene)
	:scene_(scene)
{
}

SettingObjectManager::~SettingObjectManager()
{
	enemyObjectList_.clear();
	settingObjectList_.clear();
}

void SettingObjectManager::Update()
{
	//CountSettingObject();
	//CountEnemy();
}

void SettingObjectManager::CountSettingObject()
{
	for (auto i : *scene_->GetChildList())
	{
		if (i->GetObjectName() == "SettingObject")
		{
			settingObjectList_.push_back((SettingObject*)i);
		}
	}
}

void SettingObjectManager::CreateSettingObject(std::string name)
{
	if (name == "Enemy")
	{
		//オブジェクト生成+名前設定
		Enemy* pEnemy = scene_->Instantiate<Enemy>(scene_);
		std::string settingObjectName = "Enemy" + std::to_string(enemyObjectList_.size());
		pEnemy->SetSettingObjectName(settingObjectName);
		enemyObjectList_.push_back(pEnemy);
	}
}

void SettingObjectManager::CountEnemy()
{
	for (auto i : settingObjectList_)
	{
		if (i->GetSettingObjectName() == "Enemy")
		{
			enemyObjectList_.push_back(i);
		}
	}
}
