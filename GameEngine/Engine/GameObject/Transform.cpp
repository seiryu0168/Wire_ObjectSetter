#include "Transform.h"

//コンストラクタ
Transform::Transform()
{
	matTranslate_=XMMatrixIdentity();
	matRotate_=XMMatrixIdentity();
	matScale_=XMMatrixIdentity();
	position_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotate_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	baseVec_ = XMVectorSet(0, 0, 0, 0);
	pParent_ = nullptr;
}

//デストラクタ
Transform::~Transform()
{

}

//各行列の計算
void Transform::Calclation()
{
	//移動行列
	matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);
	
	//回転行列
	matRotate_ = XMMatrixRotationX(XMConvertToRadians(rotate_.x))
		* XMMatrixRotationY(XMConvertToRadians(rotate_.y))
		* XMMatrixRotationZ(XMConvertToRadians(rotate_.z));
	
	//拡大行列
	matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

//逆行列の計算
XMMATRIX Transform::GetNormalMatrix()
{
	return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}

//ワールド行列を取得
XMMATRIX Transform::GetWorldMatrix()
{
	Calclation();
	if (pParent_)
	{
		return matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
	}
	return matScale_ * matRotate_ * matTranslate_;

}
XMMATRIX Transform::GetLocalMatrix()
{
	return matScale_ * matRotate_ * matTranslate_;
}
XMMATRIX Transform::GetLocalTranslateMatrix()
{
	return matTranslate_;
}

XMMATRIX Transform::GetLocalRotateMatrix()
{
	return matRotate_;
}
XMMATRIX Transform::GetLocalScaleMatrix()
{
	return matScale_;
}


XMMATRIX Transform::GetWorldTranslateMatrix()
{
	Calclation();
	if (pParent_)
	{
		return matTranslate_ * pParent_->GetLocalTranslateMatrix();
	}
	return matTranslate_;
}

XMMATRIX Transform::GetWorldRotateMatrix()
{
	Calclation();
	if (pParent_)
	{
		return matRotate_ * pParent_->GetLocalRotateMatrix();
	}
	return matRotate_;
}
XMMATRIX Transform::GetWorldScaleMatrix()
{
	Calclation();
	if (pParent_)
	{
		return matScale_ * pParent_->GetLocalScaleMatrix();
	}
	return matScale_;
}