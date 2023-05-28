#include "BoxCollider.h"

BoxCollider::BoxCollider(XMFLOAT3 basePos, XMFLOAT3 size)
{
	center_ = basePos;
	size_ = size;
	type_ = BOX_COLLIDER;
}

//�Փ˔���
//���������I�u�W�F�N�g�̓����蔻�肪target�ɓ���
bool BoxCollider::IsHit(Collider* target)
{
	switch (target->type_)
	{
	case BOX_COLLIDER:
		return IsHitBox_Box((BoxCollider*)target, this);

	case SPHERE_COLLIDER:
		return IsHitBox_Sphere(this, (SphereCollider*)target);
		
		//�����I�Ƀ|���S���Ƃ̓����蔻����������Ă݂�
	//case 
	}
}