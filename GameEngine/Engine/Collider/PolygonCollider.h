#pragma once
#include"Collider.h"

class PolygonCollider : public Collider
{
	friend class Collider;
	int hModel_;
public:
	PolygonCollider(XMFLOAT3 pos,int hModel);
	bool IsHit(Collider* target) override;
};