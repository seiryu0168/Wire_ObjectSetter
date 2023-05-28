#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
private:
	friend class Collider;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="basePos">当たり判定を付けるオブジェクトとの相対的な座標</param>
	/// <param name="radius">半径</param>
	SphereCollider(XMFLOAT3 basePos, float radius);
	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="target">当たったコライダーのポインタ</param>
	/// <returns></returns>
	bool IsHit(Collider* target) override;
};

