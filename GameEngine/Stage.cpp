#include "Stage.h"
#include"Engine/ResourceManager/Model.h"
Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage")
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	hModel_ = ModelManager::Load("Assets\\Model\\AAA.fbx");
	assert(hModel_ >= 0);
}

void Stage::Draw()
{
	ModelManager::SetTransform(hModel_, transform_);
	ModelManager::Draw(hModel_);
}

void Stage::Release()
{
}
