#pragma once
#include "Collider.h"


class BoxCollider : public Collider
{
	//Colliderクラスにアクセスできるようにする
	friend class Collider;

	XMVECTOR scaleX_;
	XMVECTOR scaleY_;
	XMVECTOR scaleZ_;

public:
	//箱型の判定範囲
	//basePos
	BoxCollider(XMFLOAT3 basePos, XMFLOAT3 size);

	//衝突判定
	bool IsHit(Collider* target) override;
};