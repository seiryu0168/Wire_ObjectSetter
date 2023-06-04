#pragma once
#include"SettingObject.h"
class Enemy : public SettingObject
{
private:
	int enemyNum_;
	std::string enemyType_;
public:
	Enemy(GameObject* parent);
	~Enemy();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	std::string SetObjectName(const std::string& defaultName);
	void SetEnemyType(std::string type) { enemyType_ = type; }
	std::string GetEnemyType() { return enemyType_; }
	int CheckSameNameObjectCount();
	void Release() override;
};

