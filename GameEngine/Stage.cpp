#include "Stage.h"
#include"Engine/ResourceManager/Model.h"
Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),
	hModel_(-1)
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
}

void Stage::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	ModelManager::Draw(hModel_);
}

void Stage::LoadModel(std::string modelName)
{
	if (hModel_ <= -1)
	{
		hModel_ = ModelManager::Load(modelName);
		assert(hModel_ >= 0);
	}
}

void Stage::Release()
{
}
