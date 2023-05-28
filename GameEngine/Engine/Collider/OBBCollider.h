#pragma once
#include"Collider.h"
class OBBCollider : public Collider
{
	friend class Collider;
	bool rotateFlag_;
	bool scaleFlag_;

	XMVECTOR nX;
	XMVECTOR nY;
	XMVECTOR nZ;

	XMVECTOR OBB_X;
	XMVECTOR OBB_Y;
	XMVECTOR OBB_Z;
public:
	OBBCollider(XMFLOAT3 size, bool isRotate = true, bool isScale = true);
	bool IsHit(Collider* target) override;

	//�I�u�W�F�N�gA,B�̋������v�Z
	XMVECTOR calcDistance(XMFLOAT3 posA, XMFLOAT3 posB);
	
	//�������Ă��邩�ǂ����̌v�Z
	//sep : ������
	//e1  : ��1
	//e2  : ��2
	//e3  : ��3(��3�͊����ł���ꍇ������)
	float prjLine(XMVECTOR* sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3 = nullptr);

	void CalcAxisVec();
};

