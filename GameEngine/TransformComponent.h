#pragma once
#include"Component.h"
#include"Engine/DirectX_11/Math.h"
class TransformComponent : public Component
{
private:
	XMVECTOR RotationQuaternion_;

	XMMATRIX matTranslate_;	//�ړ��s��
	XMMATRIX matRotate_;	//��]�s��	
	XMMATRIX matScale_;	//�g��s��
public:
	XMFLOAT3 position_;	//�ʒu
	XMFLOAT3 rotate_;	//����
	XMFLOAT3 scale_;	//�g�嗦
	XMVECTOR baseVec_;	//��ƂȂ�x�N�g��
		
	TransformComponent* pParent_;//�e�̏��
	TransformComponent(GameObject* atcObj);
	~TransformComponent();

	//�e�s��̌v�Z
	void Calclation();

	//��]�s��Ɗg�k�̋t�s��̌v�Z
	XMMATRIX GetNormalMatrix();

	//���[���h�s����擾
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetLocalMatrix();
	XMMATRIX GetLocalTranslateMatrix();
	XMMATRIX GetLocalRotateMatrix();
	XMMATRIX GetLocalScaleMatrix();

	XMMATRIX GetWorldTranslateMatrix();
	XMMATRIX GetWorldRotateMatrix();
	XMMATRIX GetWorldScaleMatrix();
	void SetParentTransformComponent(TransformComponent* parent);

	static XMFLOAT3 Float3Add(XMFLOAT3 add1, XMFLOAT3 add2)
	{
		return XMFLOAT3(add1.x + add2.x, add1.y + add2.y, add1.z + add2.z);
	}


};

