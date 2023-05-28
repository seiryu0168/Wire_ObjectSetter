#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
private:
	friend class Collider;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="basePos">�����蔻���t����I�u�W�F�N�g�Ƃ̑��ΓI�ȍ��W</param>
	/// <param name="radius">���a</param>
	SphereCollider(XMFLOAT3 basePos, float radius);
	/// <summary>
	/// �Փ˔���
	/// </summary>
	/// <param name="target">���������R���C�_�[�̃|�C���^</param>
	/// <returns></returns>
	bool IsHit(Collider* target) override;
};

