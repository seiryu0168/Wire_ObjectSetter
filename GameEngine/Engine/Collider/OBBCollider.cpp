#include "OBBCollider.h"
#include"../GameObject/GameObject.h"
OBBCollider::OBBCollider(XMFLOAT3 size, bool isRotate, bool isScale)
{
	rotateFlag_ = isRotate;
	scaleFlag_ = isScale;

	center_ = { 0,0,0 };
	size_ = size;
	nX = XMVectorSet(1, 0, 0, 0);
	nY = XMVectorSet(0, 1, 0, 0);
	nZ = XMVectorSet(0, 0, 1, 0);
	OBB_X = nX * size.x;
	OBB_Y = nY * size.y;
	OBB_Z = nZ * size.z;
	type_ = OBB_COLLIDER;
}

bool OBBCollider::IsHit(Collider* target)
{
	switch (target->type_)
	{

	case SPHERE_COLLIDER:
		return IsHitOBB_Sphere(this, (SphereCollider*)target);
	case OBB_COLLIDER:
		return IsHitOBB_OBB(this, (OBBCollider*)target);
	}
	return false;
}

XMVECTOR OBBCollider::calcDistance(XMFLOAT3 posA, XMFLOAT3 posB)
{
	XMVECTOR distance = XMLoadFloat3(&posB) - XMLoadFloat3(&posA);
	return distance;
}

float OBBCollider::prjLine(XMVECTOR* sep, XMVECTOR* e1, XMVECTOR* e2, XMVECTOR* e3)
{
	XMVECTOR sp = XMVectorZero();
	sp = XMVector3Normalize(*sep);
	float r1 = abs(XMVectorGetX(XMVector3Dot(sp, *e1)));
	float r2 = abs(XMVectorGetX(XMVector3Dot(sp, *e2)));
	float r3 = e3 ? abs(XMVectorGetX(XMVector3Dot(sp, *e3))) : 0;

	return r1 + r2 + r3;
}

void OBBCollider::CalcAxisVec()
{
	XMVECTOR rotateQua = XMQuaternionRotationMatrix(GetpColObject()->GetTransform().GetWorldRotateMatrix());
	OBB_X = XMVector3Rotate(OBB_X, rotateQua);
	OBB_Y = XMVector3Rotate(OBB_Y, rotateQua);
	OBB_Z = XMVector3Rotate(OBB_Z, rotateQua);


}