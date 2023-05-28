#pragma once
#include "Collider.h"


class BoxCollider : public Collider
{
	//Collider�N���X�ɃA�N�Z�X�ł���悤�ɂ���
	friend class Collider;

	XMVECTOR scaleX_;
	XMVECTOR scaleY_;
	XMVECTOR scaleZ_;

public:
	//���^�̔���͈�
	//basePos
	BoxCollider(XMFLOAT3 basePos, XMFLOAT3 size);

	//�Փ˔���
	bool IsHit(Collider* target) override;
};