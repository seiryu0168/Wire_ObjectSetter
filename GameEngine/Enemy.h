#pragma once
#include"SettingObject.h"
class Enemy : public SettingObject
{
private:
	int enemyNum_;
public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	std::string SetObjectName(const std::string& defaultName);

	int CheckSameNameObjectCount();
	void Release() override;
};

