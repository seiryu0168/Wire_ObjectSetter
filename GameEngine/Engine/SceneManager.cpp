#include"GameObject/RootJob.h"
#include "SceneManager.h"
#include"../MainScene.h"
#include"ResourceManager/Model.h"
#include"ResourceManager/ImageManager.h"
#include"ResourceManager/Audio.h"


SceneManager::SceneManager(GameObject* parent) 
	: GameObject(parent, "SceneManager"),
	currentSceneID_(SCENE_ID::SCENE_ID_MAIN),
	nextSceneID_(SCENE_ID::SCENE_ID_MAIN),
	isSceneChange_(false)
{
	currentSceneID_ = SCENE_ID::SCENE_ID_MAIN;
	nextSceneID_ = currentSceneID_;

}

void SceneManager::Initialize()
{
	Instantiate<MainScene>(this);
}

void SceneManager::Update()
{
	if (/*currentSceneID_ != nextSceneID_*/isSceneChange_&&countDown_ == 0)
	{
		KillAllChildren();
		ModelManager::AllDeleteModelNum();
		ModelManager::Release();
		ImageManager::AllRelease();
		

		//SceneBase* p = nullptr;
		switch ((SCENE_ID)nextSceneID_)
		{
		case SCENE_ID::SCENE_ID_MAIN:Instantiate<MainScene>(this); break;
		}

		isSceneChange_ = false;
		currentSceneID_ = nextSceneID_;
	}
	countDown_--;
}

void SceneManager::Draw()
{

}

void SceneManager::Release()
{

}
void SceneManager::ChangeScene(SCENE_ID sceneID,UINT count)
{
	nextSceneID_ = sceneID;
	countDown_ = count;
	isSceneChange_ = true;
}
