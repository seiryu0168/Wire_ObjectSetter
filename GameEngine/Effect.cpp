#include "Effect.h"
#include"Engine/Fbx.h"
//#include"Engine/SAFE_DELETE_RELEASE.h"
#include<vector>
namespace Effect
{
	struct EffectData
	{
		Fbx* pFbx_;
		Transform transform_;
		std::string fileName_;
	};
	std::vector<EffectData*> effectData_;
}

int Effect::Load(std::string fileName)
{
	EffectData* pEffect = new EffectData;
	pEffect->fileName_ = fileName;
	pEffect->pFbx_ = nullptr;
	//既にあるファイルと名前が同じならそのアドレスを代入
	for (auto itr = effectData_.begin(); itr != effectData_.end(); itr++)
	{

		if (fileName == (*itr)->fileName_)
		{
			pEffect->pFbx_ = (*itr)->pFbx_;
			break;
		}
	}

	if (pEffect->pFbx_ == nullptr)
	{
		pEffect->pFbx_ = new Fbx;
		pEffect->pFbx_->Load(fileName);
	}
	effectData_.push_back(pEffect);
	return effectData_.size() - 1;

}
void Effect::Draw(int effNum)
{
	effectData_[effNum]->pFbx_->Draw(effectData_[effNum]->transform_, SHADER_EFF);
}
void Effect::SetTransform(int effNum, Transform transform)
{
	if (effectData_[effNum] != nullptr)
	{
		effectData_[effNum]->transform_ = transform;
	}
}
void Effect::Release()
{
	for (int i = 0; i < effectData_.size(); i++)
	{
		bool isRef = false;
		//i番目のデータより後に同じファイルを参照してなければfalse
		for (int j = i + 1; j < effectData_.size(); j++)
		{
			if(effectData_[i]->pFbx_ == effectData_[j]->pFbx_)
			{
				isRef = true;
			}
		}

		if (!isRef)
		{
			SAFE_DELETE(effectData_[i]->pFbx_);
		}

		SAFE_DELETE(effectData_[i]);
	}
	effectData_.clear();

}