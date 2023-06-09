#include "TransformComponent.h"

TransformComponent::TransformComponent(GameObject* atcObj)
	:Component(atcObj)
{
	matTranslate_ = XMMatrixIdentity();
	matRotate_ = XMMatrixIdentity();
	matScale_ = XMMatrixIdentity();
	position_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotate_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
	baseVec_ = XMVectorSet(0, 0, 0, 0);
	pParent_ = nullptr;
	pParent_ = atcObj->GetComponent<TransformComponent>();
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Calclation()
{
	//ˆÚ“®s—ñ
	matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);

	//‰ñ“]s—ñ
	matRotate_ = XMMatrixRotationX(XMConvertToRadians(rotate_.x))
		* XMMatrixRotationY(XMConvertToRadians(rotate_.y))
		* XMMatrixRotationZ(XMConvertToRadians(rotate_.z));

	//Šg‘ås—ñ
	matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

XMMATRIX TransformComponent::GetNormalMatrix()
{
	return matRotate_ * XMMatrixInverse(nullptr, matScale_);
}

XMMATRIX TransformComponent::GetWorldMatrix()
{
	Calclation();
	if (pParent_)
	{
		return matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
	}
	return matScale_ * matRotate_ * matTranslate_;
}

XMMATRIX TransformComponent::GetLocalMatrix()
{
	return matScale_ * matRotate_ * matTranslate_;
}

XMMATRIX TransformComponent::GetLocalTranslateMatrix()
{
	return matTranslate_;
}

XMMATRIX TransformComponent::GetLocalRotateMatrix()
{
	return matRotate_;
}

XMMATRIX TransformComponent::GetLocalScaleMatrix()
{
	return matScale_;
}

XMMATRIX TransformComponent::GetWorldTranslateMatrix()
{
	Calclation();
	if (pParent_)
	{
		return matTranslate_ * pParent_->GetLocalTranslateMatrix();
	}
	return matTranslate_;	
}

XMMATRIX TransformComponent::GetWorldRotateMatrix()
{
	Calclation();
	if (pParent_)
	{
		return matRotate_ * pParent_->GetLocalRotateMatrix();
	}
	return matRotate_;
}

XMMATRIX TransformComponent::GetWorldScaleMatrix()
{
	Calclation();
	if (pParent_)
	{
		return matScale_ * pParent_->GetLocalScaleMatrix();
	}
	return matScale_;
}

void TransformComponent::SetParentTransformComponent(TransformComponent* parent)
{
	pParent_ = parent;
}
