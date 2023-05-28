#pragma once
#include<d3d11.h>
#include<DirectXMath.h>
#include"../GameObject/Transform.h"
using namespace DirectX;

//�O���錾
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

//�����蔻��
class Collider
{

	friend class BoxCollider;
	friend class SphereCollider;
	friend class OBBCollider;
	friend class PolygonCollider;

	GameObject*  pColObject_;	//�����蔻���t����I�u�W�F�N�g
	ColliderType type_;		//�R���C�_�[�^�C�v
	XMFLOAT3     center_;		//���_
	XMFLOAT3     size_;			//�傫��
	Transform    transform_;
	bool		 isKill_;


public:
	Collider();
	virtual ~Collider();
	//�Փ˔���
	virtual bool IsHit(Collider* target) = 0;

	//�Փ˔͈�
	
	/// <summary>
	/// AABB���m�̓����蔻��
	/// </summary>
	/// <param name="box1">BoxCollider�^</param>
	/// <param name="box2">BoxCollider�^</param>
	/// <returns></returns>
	bool IsHitBox_Box(BoxCollider* box1, BoxCollider* box2);

	/// <summary>
	/// AABB�Ƌ��̓����蔻��
	/// </summary>
	/// <param name="box">BoxCollider�^</param>
	/// <param name="sphere">SphereCollider�^</param>
	/// <returns></returns>
	bool IsHitBox_Sphere(BoxCollider* box, SphereCollider* sphere);

	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="sphereA">SphereCollider�^</param>
	/// <param name="sphereB">SphereCollider�^</param>
	/// <returns></returns>
	bool IsHitSphere_Sphere(SphereCollider* sphereA, SphereCollider* sphereB);

	//bool IsHitSphere_Polygon(SphereCollider* sphereA,std::vector<XMVECTOR> polyList);
	/// <summary>
	/// OBB���m�̓����蔻��
	/// </summary>
	/// <param name="obbA">OBBCollider�^</param>
	/// <param name="obbB">OBBCollider�^</param>
	/// <returns></returns>
	bool IsHitOBB_OBB(OBBCollider* obbA,OBBCollider* obbB);

	/// <summary>
	/// OBB�Ƌ��̓����蔻��
	/// </summary>
	/// <param name="obb">OBBCollider�^</param>
	/// <param name="sphere">SphereCollider�^</param>
	/// <returns></returns>
	bool IsHitOBB_Sphere(OBBCollider* obb, SphereCollider* sphere);


	/// <summary>
	/// �����蔻���t�����I�u�W�F�N�g���擾
	/// </summary>
	/// <returns>�����蔻���t�����I�u�W�F�N�g</returns>
	GameObject* GetpColObject() { return pColObject_; }
	
	/// <summary>
	/// �����蔻�������I�u�W�F�N�g��ݒ�
	/// </summary>
	/// <param name="gameObject">�C�ӂ̃I�u�W�F�N�g(GameObject�^)</param>
	void SetGemaObject(GameObject* gameObject) { pColObject_ = gameObject; }


	void SetCollider(XMFLOAT3 center,XMFLOAT3 size)
	{
		center_ = center;
		size_ = size;
	}
	void KillCollider() { isKill_ = true; }
	bool IsKill() { return isKill_; }
};

