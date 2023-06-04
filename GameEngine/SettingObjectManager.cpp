#include "SettingObjectManager.h"

SettingObjectManager::SettingObjectManager(GameObject* scene)
	:scene_(scene)
{
}

SettingObjectManager::~SettingObjectManager()
{
	Release();
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
	Enemy* pObject = scene_->Instantiate<Enemy>(scene_);

	std::string settingObjectName = name + std::to_string(enemyObjectList_.size());
	if (name == "EnemyNormal")
	{
		//オブジェクト生成+名前設定
		pObject->SetEnemyType(name);
		pObject->SetSettingObjectName(settingObjectName);
		pObject->LoadModel("Assets\\Model\\EnemyBall.fbx");
		enemyObjectList_.push_back(pObject);
	}
	if(name=="EnemyTurret")
	{
		pObject->SetEnemyType(name);
		pObject->SetSettingObjectName(settingObjectName);
		pObject->LoadModel("Assets\\Model\\EnemyTurret.fbx");
		enemyObjectList_.push_back(pObject);
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

void SettingObjectManager::Release()
{
	settingObjectList_.clear();
	enemyObjectList_.clear();
}
