#include "CameraController.h"
#include"Engine/DirectX_11/Input.h"
namespace
{
	static const XMFLOAT2 CAMERA_ROTATE_LIMIT = { 89.0f,-89.0f };
	static const XMFLOAT2 CAMERA_ZOOM_LIMIT = { 10.0f,1000.0f };
	static const XMFLOAT2 CAMERA_MOVE_RATIO = { 0.5f,0.5f };
}
CameraController::CameraController(GameObject* parent)
	:GameObject(parent,"CameraController"),
	wheelMove_(10.0f),
	wheelMoveVec_(XMVectorSet(0,0,wheelMove_,0)),
	camVec_(XMVectorSet(0,0,1,0))
{

}

CameraController::~CameraController()
{
}

void CameraController::Initialize()
{
}

void CameraController::Update()
{
	Move();
	Rotate();
}

void CameraController::Move()
{

	if (Input::IsKey(DIK_LSHIFT) && Input::IsMouseButton(2))
	{
		moveVec_ = Input::GetMouseMove();
		XMVECTOR v = XMVector3Rotate(moveVec_, 
					 XMQuaternionRotationRollPitchYaw(XMConvertToRadians(camRotate_.x),
													  XMConvertToRadians(camRotate_.y),
													  XMConvertToRadians(camRotate_.z)));
		transform_.position_=StoreFloat3(XMLoadFloat3(&transform_.position_) + v);
		XMFLOAT3 camPos = Camera::GetPosition();
		Camera::SetPosition(XMLoadFloat3(&camPos) + v);
		Camera::SetTarget(XMLoadFloat3(&transform_.position_));
	
	}
	//XMFLOAT3 mouseMove = StoreFloat3(XMVector3Normalize(Input::GetMouseMove()));
	//if (abs(mouseMove.z) > 0)
	//{
	//	wheelMove_ += mouseMove.z;
	//	wheelMove_ = Clamp<float>(wheelMove_, 10.0f,1000.0f);
	//	wheelMoveVec_ = XMVectorSet(0, 0, wheelMove_, 0);//*CalcTargetDistanceRatio();
	//	if(XMVector3Equal(wheelMoveVec_, XMVectorZero()) == false)
	//	Camera::SetPosition(wheelMoveVec_);
	//}
	//if (Input::IsMouseButton(2))
	//{
	//	camRotate_ += mouseMove;
	//	moveVec_ += XMVectorSet(-mouseMove.x, mouseMove.y, 0, 0);
	//	//StoreFloat3(move_);
	//	if (XMVector3Equal(moveVec_, XMVectorZero()) == false)
	//	{
	//		XMFLOAT3 camPos = Camera::GetPosition();
	//		XMVECTOR camtarget = Camera::GetTarget() - XMLoadFloat3(&camPos);
	//		Camera::SetPosition(moveVec_ + wheelMoveVec_);
	//		Camera::SetTarget(XMLoadFloat3(&transform_.position_));
	//	}
	//}
}

void CameraController::Rotate()
{
	XMFLOAT3 musMove = StoreFloat3(Input::GetMouseMove());
	XMVECTOR camVec = XMVectorZero();
	XMMATRIX matRotate;
	if (Input::IsMouseButton(1))
	{
		camRotate_.x += musMove.y *CAMERA_MOVE_RATIO.x;
		camRotate_.y += musMove.x *CAMERA_MOVE_RATIO.x;
		camRotate_.z = 0.0f;
		camRotate_.x = Clamp<float>(camRotate_.x, CAMERA_ROTATE_LIMIT.y, CAMERA_ROTATE_LIMIT.x);

		matRotate = XMMatrixRotationX(XMConvertToRadians(-camRotate_.x)) *
									  XMMatrixRotationY(XMConvertToRadians(camRotate_.y));
		camVec= camVec_ * matRotate;
		upVec_= XMVector3TransformCoord(camVec_, matRotate);
		wheelMove_ = Clamp<float>(wheelMove_, CAMERA_ZOOM_LIMIT.x, CAMERA_ZOOM_LIMIT.y);
		Camera::SetPosition(XMLoadFloat3(&transform_.position_) + camVec*wheelMove_);	
	}
	if (abs(musMove.z) > 0.0f)
	{
		matRotate = XMMatrixRotationX(XMConvertToRadians(-camRotate_.x)) *
					XMMatrixRotationY(XMConvertToRadians(camRotate_.y));
		camVec= camVec_ * matRotate;
		wheelMove_ += -musMove.z * CAMERA_MOVE_RATIO.y;
		wheelMove_ = Clamp<float>(wheelMove_, CAMERA_ZOOM_LIMIT.x, CAMERA_ZOOM_LIMIT.y);
		Camera::SetPosition(XMLoadFloat3(&transform_.position_) + camVec * wheelMove_);
	}
	

}

void CameraController::Release()
{
}

float CameraController::CalcTargetDistanceRatio()
{
	float ratio = 0;
	XMFLOAT3 pos = Camera::GetPosition();
	XMVECTOR distVec = Camera::GetTarget() - XMLoadFloat3(&pos);
	ratio = VectorLength(distVec)/10.0f;
	return ratio;
}