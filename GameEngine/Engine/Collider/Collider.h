#pragma once
#include<d3d11.h>
#include<DirectXMath.h>
#include"../GameObject/Transform.h"
using namespace DirectX;

//前方宣言
class GameObject;
class BoxCollider;
class SphereCollider;
class OBBCollider;
class PolygonCollider;

enum ColliderType
{
	BOX_COLLIDER,
	SPHERE_COLLIDER,
	OBB_COLLIDER,

};

//当たり判定
class Collider
{

	friend class BoxCollider;
	friend class SphereCollider;
	friend class OBBCollider;
	friend class PolygonCollider;

	GameObject*  pColObject_;	//当たり判定を付けるオブジェクト
	ColliderType type_;		//コライダータイプ
	XMFLOAT3     center_;		//原点
	XMFLOAT3     size_;			//大きさ
	Transform    transform_;
	bool		 isKill_;


public:
	Collider();
	virtual ~Collider();
	//衝突判定
	virtual bool IsHit(Collider* target) = 0;

	//衝突範囲
	
	/// <summary>
	/// AABB同士の当たり判定
	/// </summary>
	/// <param name="box1">BoxCollider型</param>
	/// <param name="box2">BoxCollider型</param>
	/// <returns></returns>
	bool IsHitBox_Box(BoxCollider* box1, BoxCollider* box2);

	/// <summary>
	/// AABBと球の当たり判定
	/// </summary>
	/// <param name="box">BoxCollider型</param>
	/// <param name="sphere">SphereCollider型</param>
	/// <returns></returns>
	bool IsHitBox_Sphere(BoxCollider* box, SphereCollider* sphere);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphereA">SphereCollider型</param>
	/// <param name="sphereB">SphereCollider型</param>
	/// <returns></returns>
	bool IsHitSphere_Sphere(SphereCollider* sphereA, SphereCollider* sphereB);

	//bool IsHitSphere_Polygon(SphereCollider* sphereA,std::vector<XMVECTOR> polyList);
	/// <summary>
	/// OBB同士の当たり判定
	/// </summary>
	/// <param name="obbA">OBBCollider型</param>
	/// <param name="obbB">OBBCollider型</param>
	/// <returns></returns>
	bool IsHitOBB_OBB(OBBCollider* obbA,OBBCollider* obbB);

	/// <summary>
	/// OBBと球の当たり判定
	/// </summary>
	/// <param name="obb">OBBCollider型</param>
	/// <param name="sphere">SphereCollider型</param>
	/// <returns></returns>
	bool IsHitOBB_Sphere(OBBCollider* obb, SphereCollider* sphere);


	/// <summary>
	/// 当たり判定を付けたオブジェクトを取得
	/// </summary>
	/// <returns>当たり判定を付けたオブジェクト</returns>
	GameObject* GetpColObject() { return pColObject_; }
	
	/// <summary>
	/// 当たり判定をつけるオブジェクトを設定
	/// </summary>
	/// <param name="gameObject">任意のオブジェクト(GameObject型)</param>
	void SetGemaObject(GameObject* gameObject) { pColObject_ = gameObject; }


	void SetCollider(XMFLOAT3 center,XMFLOAT3 size)
	{
		center_ = center;
		size_ = size;
	}
	void KillCollider() { isKill_ = true; }
	bool IsKill() { return isKill_; }
};

