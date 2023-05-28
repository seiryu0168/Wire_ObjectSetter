#include "Transform.h"

//�R���X�g���N�^
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

//�f�X�g���N�^
Transform::~Transform()
{

}

//�e�s��̌v�Z
void Transform::Calclation()
{
	//�ړ��s��
	matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);
	
	//��]�s��
	matRotate_ = XMMatrixRotationX(XMConvertToRadians(rotate_.x))
		* XMMatrixRotationY(XMConvertToRadians(rotate_.y))
		* XMMatrixRotationZ(XMConvertToRadians(rotate_.z));
	
	//�g��s��
	matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

//�t�s��̌v�Z
XMMATRIX Transform::GetNormalMatrix()
{
	return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}

//���[���h�s����擾
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