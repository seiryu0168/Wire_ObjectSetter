#include "SphereCollider.h"

SphereCollider::SphereCollider(XMFLOAT3 basePos, float radius)
{
	center_ = basePos;
	size_ = XMFLOAT3(radius, radius, radius);
	type_ = SPHERE_COLLIDER;
}


bool SphereCollider::IsHit(Collider* target)
{
	switch (target->type_)
	{
	case BOX_COLLIDER:
		return IsHitBox_Sphere((BoxCollider*)target, this);
	case SPHERE_COLLIDER:
		return IsHitSphere_Sphere((SphereCollider*)target, this);
	case OBB_COLLIDER:
		return IsHitOBB_Sphere((OBBCollider*)target, this);
	default:
		assert(false);
	}
}