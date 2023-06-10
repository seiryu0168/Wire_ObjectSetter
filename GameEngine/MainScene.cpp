#include "MainScene.h"
#include"Controller.h"
#include"Enemy.h"
#include"Stage.h"
#include"CameraController.h"
MainScene::MainScene(GameObject* parent)
	:GameObject(parent,"MainScene")
{
}

MainScene::~MainScene()
{
}

void MainScene::Initialize()
{
	pControlObject_ = Instantiate<Controller>(this);
	((Controller*)pControlObject_)->InitUI(Direct3D::GetWindowHandle(), Direct3D::GetDevice(), Direct3D::GetContext());
	Instantiate<CameraController>(this);
	Instantiate<Stage>(this);

}

void MainScene::Update()
{
	
}

void MainScene::Release()
{
}
