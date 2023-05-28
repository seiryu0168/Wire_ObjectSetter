#pragma once
#include "GameObject/GameObject.h"
enum class SCENE_ID
{
	SCENE_ID_MAIN = 0,

};

class SceneManager : public GameObject
{
	SCENE_ID currentSceneID_;
	SCENE_ID nextSceneID_;
	bool isSceneChange_;
	UINT countDown_;
public:
	SceneManager(GameObject* parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void ChangeScene(SCENE_ID sceneID, UINT count = 0);
	int GetCountDown() { return countDown_; }
	SCENE_ID GetCurrentSceneID() { return currentSceneID_; }
	SCENE_ID GetNextSceneID() { return nextSceneID_; }
	bool IsSceneChange() { return isSceneChange_; }
	GameObject* CurrentScene();
};

