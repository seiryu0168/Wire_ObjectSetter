#include "Camera.h"

//�ϐ�
XMVECTOR position_;			//�J�����̈ʒu�i���_�j
XMVECTOR target_;			//����ʒu�i�œ_�j
XMVECTOR upVector_;			//��x�N�g��
XMMATRIX viewMatrix_;		//�r���[�s��
XMMATRIX projMatrix_;		//�v���W�F�N�V�����s��
XMMATRIX billBoardMatrix_;	//�r���{�[�h�s��
float	 angleOfView;		//����p
float	 aspectRadio;		//�A�X�y�N�g��
float	 nearClipping;		//�j�A�N���b�s���O
float	 farClipping;		//�t�@�[�N���b�s���O

//������
void Camera::Initialize(float width,float height)
{
	position_ = XMVectorSet(0, 3, -10, 0);	//�J�����̈ʒu
	target_ = XMVectorSet(0, 0, 0, 0);	//�J�����̏œ_
	upVector_ = XMVectorSet(0, 1, 0, 0);
	angleOfView = XM_PIDIV4;
	aspectRadio = (FLOAT)width / (FLOAT)height;
	nearClipping = 0.1f;
	farClipping = 1000.0f;
	// �v���W�F�N�V�����s��
		projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)width / (FLOAT)height, 0.1f, 1000.0f);
}

//�X�V
void Camera::Update()
{
	//�r���[�s��̍쐬
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, upVector_);

	billBoardMatrix_ = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), target_ - position_, upVector_);
	billBoardMatrix_ = XMMatrixInverse(nullptr, billBoardMatrix_);
}

//�ʒu��ݒ�
void Camera::SetPosition(XMVECTOR position)
{
	position_ = position;
}

//�œ_��ݒ�
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


//���_(�J�����̈ʒu)���擾
XMFLOAT3 Camera::GetPosition()
{
	return StoreFloat3(position_);
}
XMVECTOR Camera::GetTarget()
{
	return target_;
}
//�r���[�s����擾
XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

XMMATRIX Camera::GetBillBoardMatrix()
{
	return billBoardMatrix_;
}
//�v���W�F�N�V�����s����擾
XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}