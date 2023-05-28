#include "GameObject.h"
#include"../SAFE_DELETE_RELEASE.h"
int objectcount = 0;
GameObject::GameObject() : GameObject(nullptr,"")
{

}
GameObject::GameObject(GameObject* parent, const std::string& name)
	: pParent_(parent),
	objectName_(name),
	objectTag_(""),
	killFlag_(0),
	activeFlag_(true),
	isUpdate_(true),
	startFlag_(false),
	drawFlag_(true),
	objectID_(-1)
{
	if(parent)
	transform_.pParent_ = &parent->transform_;
}
GameObject::~GameObject()
{

}

void GameObject::UpdateSub()
{
	/////////�A�b�v�f�[�g/////////
	if (startFlag_ == false&&activeFlag_)
	{
		GameObject* p = GetRootJob();
		this->Initialize();
		this->startFlag_ = true;
	}
	else if(startFlag_&&
			activeFlag_&&
			isUpdate_)
	Update();



	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->UpdateSub();
	}
	////////�����蔻��///////////
	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		if (activeFlag_)
		{

			if ((*itr)->killFlag_ == true)
			{
				(*itr)->BeforeDeath();
				(*itr)->ReleaseSub();
				delete* itr;
				itr = childList_.erase(itr);
			}
			else
			{
				(*itr)->Collision(GetParent());
				itr++;
			}
		}
	}
}

void GameObject::FixedUpdateSub()
{
	FixedUpdate();
	for(auto itr = childList_.begin();itr!=childList_.end();itr++)
	{
		(*itr)->FixedUpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		if ((*itr)->killFlag_ == true)
		{
			(*itr)->ReleaseSub();
			delete* itr;
			itr = childList_.erase(itr);
		}
		else
		{
			(*itr)->Collision(GetParent());
			itr++;
		}
	}
}

void GameObject::DrawSub()
{
	if (drawFlag_ && startFlag_)
	{
		/*for (int i = 0; i < componentList_.size(); i++)
		{
			componentList_[i]->Update(true);
		}*/
		Draw();
	}
	for (auto i = childList_.begin(); i != childList_.end(); i++)
	{
		(*i)->DrawSub();
	}

}

void GameObject::SecondDrawSub()
{
	if (drawFlag_ && startFlag_)
	{
		SecondDraw();
	}

	for (auto i = childList_.begin(); i != childList_.end(); i++)
	{
		(*i)->SecondDrawSub();
	}
}

void GameObject::ThirdDrawSub()
{
	if (drawFlag_&&startFlag_)
	{
		ThirdDraw();
	}

	for (auto i = childList_.begin(); i != childList_.end(); i++)
	{
		(*i)->ThirdDrawSub();
	}
}

void GameObject::ReleaseSub()
{
	for (auto i = colliderList_.begin(); i != colliderList_.end(); i++)
	{
		SAFE_DELETE(*i);
	}
	for (auto i = childList_.begin(); i != childList_.end(); i++)
	{
		(*i)->ReleaseSub();
		SAFE_DELETE(*i);
	}

	Release();
}

//void GameObject::AddComponent(Component* comp)
//{
//	componentList_.push_back(comp);
//}

//void GameObject::DeleteComponent(Component* comp)
//{
//	comp->Release();
//}

//Component* GameObject::GetComponent(int compNum)
//{
//	return componentList_[compNum];
//}

//////////////////////////�����蔻��֌W////////////////////

//�Փ˔���
void GameObject::Collision(GameObject* pTarget)
{
	//pTarget��������nullptr��������Ȃ�����Ȃ�
	if (pTarget == nullptr||pTarget==this)
	{
		return;
	}
	//�����R���C�_�[�����Ă邩������Ȃ������d���[�v
	for (auto i = this->colliderList_.begin(); i != this->colliderList_.end(); i++)
	{

		for (auto j = pTarget->colliderList_.begin(); j != pTarget->colliderList_.end();j++)
		{
			if ((*i)->IsHit(*j))
			{
				this->OnCollision(pTarget);
				if (this->colliderList_.empty())
					break;
			}
		}

		if (this->colliderList_.empty())
			break;
	}

	if (pTarget->childList_.empty())
	{
		return;
	}

	for (auto itr = pTarget->childList_.begin(); itr != pTarget->childList_.end(); itr++)
	{
		Collision(*itr);
	}
}

//�R���C�_�[��ǉ�
//collider->pColObject�ɃZ�b�g����z
void GameObject::AddCollider(Collider* collider)
{
	collider->SetGemaObject(this);
	colliderList_.push_back(collider);
}

void GameObject::DelCollider(const GameObject& obj)
{
	for (auto itr = colliderList_.begin(); itr != colliderList_.end();)
	{
		if ((*itr)->GetpColObject() == &obj)
		{
			itr = colliderList_.erase(itr);
		}
		if (itr == colliderList_.end())
		{
			break;
		}
		itr ++;
	}
}

void GameObject::KillAllChildren()
{
	//���X�g�ɉ���������ΏI���
	if (childList_.empty())
	{
		return;
	}
	//���X�g�ɂ���q�ǂ���S������
	for (auto itr = this->childList_.begin(); itr != this->childList_.end();)
	{
		KillObjectSub(*itr);
		delete* itr;
		itr = this->childList_.erase(itr);
	}
}

void GameObject::KillObjectSub(GameObject* pTarget)
{
	if (!pTarget->childList_.empty())
	{
		for (auto itr = pTarget->childList_.begin(); itr != pTarget->childList_.end();)
		{
			KillObjectSub(*itr);
			delete* itr;
			itr = pTarget->childList_.erase(itr);
		}
		pTarget->childList_.clear();
	}
	pTarget->Release();
}
void GameObject::PushBackChild(GameObject* pTarget)
{
	assert(pTarget != nullptr);
	pTarget->pParent_ = this;
	pTarget->objectID_ = objectcount++;
	childList_.push_back(pTarget);
}

//�e���擾
GameObject* GameObject::GetParent()
{
	return pParent_;
}

GameObject* GameObject::GetRootJob()
{
	if (this->GetParent() == nullptr)
	{
		return this;
	}
	else return GetParent()->GetRootJob();
}

GameObject* GameObject::FindObject(std::string name)
{
	GameObject* obj= GetRootJob()->FindChild(name);
	return obj;
}

GameObject* GameObject::FindObjectAtTag(std::string tagName)
{
	GameObject* obj = GetRootJob()->FindChildAtTag(tagName);
	return obj;
}

GameObject* GameObject::FindChild(std::string name)
{
	//�q�����Ȃ�������nullptr�Ԃ�
	if (this->childList_.empty())
	{
		return nullptr;
	}

	//�q�̒�����T��
	for (auto itr = this->childList_.begin(); itr != this->childList_.end(); itr++)
	{
		//���O����v������Ԃ�
		if (name == (*itr)->objectName_)
		{
			return *itr;
		}

		//���̃I�u�W�F�N�g���T��
		GameObject* obj = (*itr)->FindChild(name);
		if (obj != nullptr)
		{
			return obj;
		}
	}	
	//������Ȃ�������
	return nullptr;
}

GameObject* GameObject::FindChildAtTag(std::string tagName)
{
	if (this->childList_.empty())
	{
		return nullptr;
	}
	//�����^�O����������Ԃ�
	for (auto i : this->childList_)
	{
		if (i->GetTag() == tagName)
		{
			return i;
		}
		//�������ׂ�
		GameObject* obj = i->FindChildAtTag(tagName);
		if (obj != nullptr)
			return obj;
	}

	//������Ȃ�������
	return nullptr;
}

GameObject* GameObject::GetScene()
{
	auto itr = GetRootJob()->GetChildList()->begin();
	return (*(*itr)->GetChildList()->begin());
}

void GameObject::SetActive(bool status)
{
	this->activeFlag_ = status;
}

void GameObject::SetParent(GameObject* parent)
{
	pParent_ = parent;
	GetParent()->PushBackChild(pParent_);
}

Transform GameObject::GetTransform()
{
	return this->transform_;
}
XMFLOAT3  GameObject::GetPosition()
{
	return this->transform_.position_;
}
XMFLOAT3  GameObject::GetRotate()
{
	return this->transform_.rotate_;
}
XMFLOAT3  GameObject::GetScale()
{
	return this->transform_.scale_;
}

XMMATRIX GameObject::LookAtMatrix(XMFLOAT3 target, XMVECTOR frontVec, XMVECTOR upVector)
{
	frontVec = XMVector3Normalize(frontVec);

	XMVECTOR Z = XMLoadFloat3(&target) - XMLoadFloat3(&this->transform_.position_); //��������ڕW�ւ̃x�N�g���@=�@Z��
	Z = XMVector3Normalize(Z);

	float angleX=0;
	XMVECTOR AAA = XMVector3Cross(Z, frontVec);					//Z���ƌ����Ă�����̃x�N�g���̊O�όv�Z(���̊O�ς���]�̎��ɂȂ�)
	AAA = XMVector3Normalize(AAA);								//���K��

	angleX= XMVectorGetX(XMVector3Dot(frontVec,Z));				//Z���ƌ����Ă�����̃x�N�g���̊O�όv�Z
	angleX = -acosf(angleX);									//���W�A���p�ɕϊ�

	XMVECTOR quo = XMQuaternionRotationNormal(AAA, angleX);    //AAA�����ɉ�]�l�������쐬
															   //�������K������Ă�x�N�g���̏ꍇXMQuaternionRotationNormal�̕����ǂ�(�����炵��)
															   //XMQuaternionRotationAxis���ƃG���[�f����

	XMMATRIX rotateMatrix = XMMatrixRotationQuaternion(quo);

	return rotateMatrix;
}

//���[���h�s��擾
//�e�̉e�����݂̍ŏI�I�ȍs��
XMMATRIX GameObject::GetWorldMatrix()
{
	return transform_.GetWorldMatrix();
}