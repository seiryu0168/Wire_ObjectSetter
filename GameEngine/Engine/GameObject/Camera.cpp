#include "Camera.h"

//変数
XMVECTOR position_;			//カメラの位置（視点）
XMVECTOR target_;			//見る位置（焦点）
XMVECTOR upVector_;			//上ベクトル
XMMATRIX viewMatrix_;		//ビュー行列
XMMATRIX projMatrix_;		//プロジェクション行列
XMMATRIX billBoardMatrix_;	//ビルボード行列
float	 angleOfView;		//視野角
float	 aspectRadio;		//アスペクト比
float	 nearClipping;		//ニアクリッピング
float	 farClipping;		//ファークリッピング

//初期化
void Camera::Initialize(float width,float height)
{
	position_ = XMVectorSet(0, 3, -10, 0);	//カメラの位置
	target_ = XMVectorSet(0, 0, 0, 0);	//カメラの焦点
	upVector_ = XMVectorSet(0, 1, 0, 0);
	angleOfView = XM_PIDIV4;
	aspectRadio = (FLOAT)width / (FLOAT)height;
	nearClipping = 0.1f;
	farClipping = 1000.0f;
	// プロジェクション行列
		projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)width / (FLOAT)height, 0.1f, 1000.0f);
}

//更新
void Camera::Update()
{
	//ビュー行列の作成
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, upVector_);

	billBoardMatrix_ = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), target_ - position_, upVector_);
	billBoardMatrix_ = XMMatrixInverse(nullptr, billBoardMatrix_);
}

//位置を設定
void Camera::SetPosition(XMVECTOR position)
{
	position_ = position;
}

//焦点を設定
void Camera::SetTarget(XMVECTOR target)
{
	target_ = target;
}

void Camera::SetUpVector(XMVECTOR upVector)
{
	upVector_ = upVector;
}

void Camera::SetProjection(float aov, float aspectRadio, float nearZ, float farZ)
{
	projMatrix_ = XMMatrixPerspectiveFovLH(aov, aspectRadio, nearZ, farZ);
}

void Camera::SetAOV(float aov)
{
	projMatrix_ = XMMatrixPerspectiveFovLH(aov, aspectRadio, nearClipping, farClipping);
}

void Camera::SetAspectRadio(float aspect)
{
	projMatrix_ = XMMatrixPerspectiveFovLH(angleOfView, aspect, nearClipping, farClipping);
}

void Camera::SetClipping(float nearZ, float farZ)
{
	projMatrix_ = XMMatrixPerspectiveFovLH(angleOfView, aspectRadio, nearZ, farZ);
}


//視点(カメラの位置)を取得
XMFLOAT3 Camera::GetPosition()
{
	return StoreFloat3(position_);
}
XMVECTOR Camera::GetTarget()
{
	return target_;
}
//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

XMMATRIX Camera::GetBillBoardMatrix()
{
	return billBoardMatrix_;
}
//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}