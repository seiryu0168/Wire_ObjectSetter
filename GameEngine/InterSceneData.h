#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/SAFE_DELETE_RELEASE.h"
#include<vector>

struct Data
{
	std::string name_;
	int dataSize_;
	GameObject* chunkClass_;
};

namespace InterSceneData
{
	//extern std::vector<Data*> dataList_;
	/*template<class T>
	int AddData(T* obj, std::string name, size_t dataSize)
	{
		Data* pData = new Data;
		for (int i = 0; i < dataList_.size(); i++)
		{
			if (name == dataList_[i]->name_)
			{
				SAFE_DELETE(pData);
				return -1;
			}
		}
		pData->chunkClass_ = new ((GameObject*)T(nullptr,""));
		memcpy((&(T*)pData->chunkClass_), obj, dataSize);
		dataList_.push_back(pData);

		return 1;
	}*/
	int AddData(std::string dataName,std::string* name,int* i=nullptr,float* f=nullptr, bool* b = nullptr);
	void DeleteData(std::string dataName);
	void SetData(std::string dataName, std::string* name, int* i = nullptr, float* f = nullptr, bool* b = nullptr);
	int GetintData(std::string dataName);
	bool GetboolData(std::string dataName);
	/*int AddData(bool b, std::string name);
	int AddData(XMFLOAT3 f3, std::string name);
	int AddData(XMVECTOR v, std::string name);
	int AddData(XMMATRIX m, std::string name);*/
};

