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
	SettingObject* pObject = scene_->Instantiate<SettingObject>(scene_);

	if (name == "EnemyNormal")
	{
		//オブジェクト生成+名前設定
		pObject->SetObjectNum(settingObjectList_.size());
		pObject->SetSettingObjectName(name);
		pObject->LoadModel("Assets\\Model\\EnemyBall.fbx");
		pObject->SetType(OBJECT_TYPE::TYPE_ENEMY);
		enemyObjectList_.push_back(pObject);
		settingObjectList_.push_back(pObject);
	}
	else if(name=="EnemyTurret")
	{
		pObject->SetObjectNum(settingObjectList_.size());
		pObject->SetSettingObjectName(name);
		pObject->LoadModel("Assets\\Model\\EnemyTurret.fbx");
		pObject->SetType(OBJECT_TYPE::TYPE_ENEMY);
		enemyObjectList_.push_back(pObject);
		settingObjectList_.push_back(pObject);
	}
	else if (name == "EnemyBoss")
	{
		pObject->SetObjectNum(settingObjectList_.size());
		pObject->SetSettingObjectName(name);
		pObject->LoadModel("Assets\\Model\\EnemyBossShield.fbx");
		pObject->SetType(OBJECT_TYPE::TYPE_ENEMY);
		enemyObjectList_.push_back(pObject);
		settingObjectList_.push_back(pObject);
	}
	else if (name == "SpeedUpItem")
	{
		pObject->SetObjectNum(settingObjectList_.size());
		pObject->SetSettingObjectName(name);
		pObject->LoadModel("Assets\\Model\\SpeedUpItem.fbx");
		pObject->SetType(OBJECT_TYPE::TYPE_ITEM);
		itemObjectList_.push_back(pObject);
		settingObjectList_.push_back(pObject);
	}
	else if (name == "SearchUpItem")
	{
		pObject->SetObjectNum(settingObjectList_.size());
		pObject->SetSettingObjectName(name);
		pObject->LoadModel("Assets\\Model\\SearchUpItem.fbx");
		pObject->SetType(OBJECT_TYPE::TYPE_ITEM);
		itemObjectList_.push_back(pObject);
		settingObjectList_.push_back(pObject);
	}
	else
	{
		pObject->KillMe();
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
