#pragma once
#include"Engine/GameObject/Camera.h"
#include"Engine/GameObject/GameObject.h"
class CameraController : public GameObject
{
private:
	XMVECTOR upVec_;
	XMVECTOR moveVec_;
	XMVECTOR camVec_;
	float wheelMove_;
	XMVECTOR wheelMoveVec_;
	XMFLOAT3 camRotate_;
	float CalcTargetDistanceRatio();
public:
	CameraController(GameObject* parent);
	~CameraController();
	void Initialize() override;
	void Update() override;
	void Move();
	void Rotate();
	void Release() override;

};

