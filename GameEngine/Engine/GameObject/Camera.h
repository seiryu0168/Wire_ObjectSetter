#pragma once
#include "../DirectX_11/Direct3D.h"
#include <DirectXMath.h>

using namespace DirectX;

//-----------------------------------------------------------
//�J����
//-----------------------------------------------------------
namespace Camera
{
	//�������i�v���W�F�N�V�����s��쐬�j
	void Initialize(float width, float height);

	//�X�V�i�r���[�s��쐬�j
	void Update();

	//���_�i�J�����̈ʒu�j��ݒ�
	void SetPosition(XMVECTOR position);

	//�œ_�i����ʒu�j��ݒ�
	void SetTarget(XMVECTOR target);

	//�J�����̏�����x�N�g����ݒ�
	void SetUpVector(XMVECTOR upVector);

	/// <summary>
	/// �v���W�F�N�V�����s���ݒ�
	/// </summary>
	/// <param name="aov">��p</param>
	/// <param name="aspectRadio">�A�X�y�N�g��</param>
	/// <param name="nearZ">�j�A�N���b�s���O</param>
	/// <param name="farZ">�t�@�[�N���b�s���O</param>
	void SetProjection(float aov, float aspectRadio, float nearZ, float farZ);

	/// <summary>
	/// ��p�ݒ�
	/// </summary>
	/// <param name="aov">��p</param>
	void SetAOV(float aov);

	/// <summary>
	/// �A�X�y�N�g��ݒ�
	/// </summary>
	/// <param name="aspect">�A�X�y�N�g��</param>
	void SetAspectRadio(float aspect);

	/// <summary>
	/// �N���b�s���O�͈͐ݒ�
	/// </summary>
	/// <param name="nearZ">�j�A�N���b�s���O</param>
	/// <param name="farZ">�t�@�[�N���b�s���O</param>
	void SetClipping(float nearZ, float farZ);

	//���_(������ʒu)���擾
	XMFLOAT3 GetPosition();

	//�œ_�擾
	XMVECTOR GetTarget();

	//�r���[�s����擾
	XMMATRIX GetViewMatrix();

	//�r���{�[�h�p��]�s��
	XMMATRIX GetBillBoardMatrix();

	//�v���W�F�N�V�����s����擾
	XMMATRIX GetProjectionMatrix();

};