#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include"Direct3D.h"
#include<list>
#include<vector>
#include <algorithm>
#include "../GameObject/Transform.h"

#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibXml2-MT.lib")
#pragma comment(lib, "zlib-MT.lib")

class Texture;

struct RayCastData
{
	XMFLOAT3 start;//���C�̔��ˈʒu
	XMFLOAT3 dir;//���C�̔��˕���
	XMVECTOR hitPos;//���������ʒu
	XMVECTOR normal;//���������ʒu�̖@��
	float dist;//������������
	float angle;//���������p�x
	BOOL hit;//�����������ǂ���
	float distLimit;//���C�̗L������
	class hitData
	{
	public:
		int hModel;
		float hitDist;
	};
	std::list<hitData> hitModelList;

	RayCastData() :start(XMFLOAT3(0, 0, 0)),
		dir(XMFLOAT3(0, 0, 0)),
		dist(9999.0f),
		angle(0),
		hit(false),
		hitPos(XMVectorSet(0, 0, 0, 0)),
		normal(XMVectorSet(0, 0, 0, 0)),
		distLimit(9999.0f){}
	void Init(XMFLOAT3 argstart = XMFLOAT3( 0,0,0 ), XMFLOAT3 argdir=XMFLOAT3(0,0,0),float argDistLimit=9999.0f)
	{
		start = argstart;
		dir = argdir;
		dist = 9999.0f;
		hit = false;
		hitPos = XMVectorSet(0, 0, 0, 0);
		normal = XMVectorSet(0, 0, 0, 0);
		distLimit = argDistLimit;
	}
	void Adjust()
	{
		hitModelList.sort([](hitData& v1, hitData& v2) {return v1.hitDist < v2.hitDist; });
	}
};

	 class FbxParts;
class Fbx
{
	std::vector<FbxParts*> parts_;

	FbxManager* pFbxManager_;
	FbxScene* pFbxScene_;
	FbxTime::EMode frameRate_;
	float animSpeed_;
	int startFrame_;
	int endFrame_;
	std::string modelName_;
	HRESULT CheckNode(FbxNode* pNode, std::vector<FbxParts*>* pPartsList);
public:

	Fbx();
	~Fbx();
	HRESULT Load(std::string fileName);
	void RayCast(RayCastData& ray,Transform& transform);
	XMFLOAT3 GetBonePosition(std::string boneName);
	std::string GetModelName();
	void    Draw(Transform& transform, SHADER_TYPE shaderType,int frame);
	void	DrawOutLine(Transform& transform, int frame, XMFLOAT4 lineColor = {0,0,0,1});
	void	DrawToon(Transform& transform, bool isOutLine, int frame);
	void    Release();
};