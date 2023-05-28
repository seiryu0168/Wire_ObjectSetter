#include "TextureManager.h"
#include"../SAFE_DELETE_RELEASE.h"
namespace TextureManager
{
	struct textureData
	{
		Texture* pTexture_;
		std::string fileName_;
	};
	std::vector<textureData*> textureList_;
}

int TextureManager::Load(std::string fileName)
{
	textureData* pTextureData = new textureData;
	pTextureData->fileName_ = fileName;
	pTextureData->pTexture_ = nullptr;
	
	for (auto itr = textureList_.begin(); itr != textureList_.end(); itr++)
	{
		if (fileName == (*itr)->fileName_)
		{
			pTextureData->pTexture_ = (*itr)->pTexture_;

			break;
		}
	}
	if (pTextureData->pTexture_ == nullptr)
	{
		pTextureData->pTexture_ = new Texture;
		pTextureData->pTexture_->Load(pTextureData->fileName_);
	}
	textureList_.push_back(pTextureData);
	return (int)textureList_.size() - 1;
}

Texture* TextureManager::GetTexture(int textureHandle)
{
	return textureList_[textureHandle]->pTexture_;
}

void TextureManager::Release()
{
	for (int i = 0; i<textureList_.size(); i++)
	{
		bool isRef = false;
		for (int j = i + 1; j < textureList_.size(); j++)
		{
			if (textureList_[i]->pTexture_ == textureList_[j]->pTexture_)
			{
				isRef = true;
			}
		}
		if (isRef==false)
		{
			SAFE_DELETE(textureList_[i]->pTexture_);
		}
		SAFE_DELETE(textureList_[i]);
	}

	textureList_.clear();
}