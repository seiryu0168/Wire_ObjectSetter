#include "Enemy.h"

Enemy::Enemy(GameObject* parent)
	:SettingObject(parent)
{
	
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	//SetObjectName(GetObjectName() + std::to_string(CheckSameNameObjectCount()));
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	//DrawModel();
}

std::string Enemy::SetObjectName(const std::string& defaultName)
{
	std::string name = defaultName + std::to_string(CheckSameNameObjectCount());
	return name;
}

int Enemy::CheckSameNameObjectCount()
{
	int count = 0;
	for (auto i : *GetParent()->GetChildList())
	{
		if (i->GetObjectName() == "SettingObject")
		{
			if (((SettingObject*)i)->GetSettingObjectName() == "Enemy")
			{
				count++;
			}
		}
	}
	return count;
}

void Enemy::Release()
{
}
