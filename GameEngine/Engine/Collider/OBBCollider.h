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

	//オブジェクトA,Bの距離を計算
	XMVECTOR calcDistance(XMFLOAT3 posA, XMFLOAT3 posB);
	
	//当たっているかどうかの計算
	//sep : 分離軸
	//e1  : 軸1
	//e2  : 軸2
	//e3  : 軸3(軸3は割愛できる場合がある)
	float prjLine(XMVECTOR* sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3 = nullptr);

	void CalcAxisVec();
};

