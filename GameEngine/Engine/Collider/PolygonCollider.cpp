#include "PolygonCollider.h"
#include"../ResourceManager/Model.h"

PolygonCollider::PolygonCollider(XMFLOAT3 pos, int hModel)
{
	hModel_ = hModel;
	center_ = pos;
}

bool PolygonCollider::IsHit(Collider* target)
{
	//switch (target->type_)
	//{
	//case SPHERE_COLLIDER
	//	return IsHitSphere_Polygon()
	//default:
	//	break;
	//}
	return false;
}
