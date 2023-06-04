#pragma once
#include"Enemy.h"
class SettingObjectManager
{
private:
	GameObject* scene_;
public :
	SettingObjectManager(GameObject* scene);
	~SettingObjectManager();
	void Update();
	std::vector<SettingObject*> settingObjectList_;
	std::vector<SettingObject*>enemyObjectList_;
	std::vector<SettingObject*>* GetEnemyList() { return &enemyObjectList_; }
	void CountSettingObject();
	
	void CreateSettingObject(std::string name);
	void CountEnemy();
	void Release();
};

