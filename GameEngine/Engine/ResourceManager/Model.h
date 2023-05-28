#pragma once
#include<string>
#include"../DirectX_11/Fbx.h"
#include"../DirectX_11/Texture.h"
#include"../GameObject/Transform.h"
//#include"../../ModelComponent.h"
#include<vector>
namespace ModelManager
{
	void Initialize();
	/// <summary>
	/// ���f���̃��[�h
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <returns></returns>
	int Load(std::string fileName);
	Fbx* LoadModel(std::string fileName);

	void AddFbxModel(Fbx* pfbxModel);
	/// <summary>
	/// transform��ݒ�
	/// </summary>
	/// <param name="modelNum">���f���ԍ�</param>
	/// <param name="transform">�Ăяo������transform</param>
	void SetTransform(int modelNum, Transform transform);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="modelNum">���f���ԍ�</param>
	void Draw(int modelNum);
	void DrawOutLine(int modelNum, XMFLOAT4 lineColor = {0,0,0,1});
	void DrawToon(int modelNum);
	/// <summary>
	/// ���C�L���X�g
	/// </summary>
	/// <param name="modelNum">���C���΂����f���ԍ�</param>
	/// <param name="ray">���C�̃f�[�^</param>
	void RayCast(int modelNum, RayCastData& ray);
	/// <summary>
	/// ������3D���f���ɑ΂��ă��C�L���X�g
	/// </summary>
	/// <param name="ray">���C�̃f�[�^</param>
	void RayCast(RayCastData& ray);
	/// <summary>
	/// �������C�L���X�g�Ɏg�����f���ԍ��̒ǉ�
	/// </summary>
	/// <param name="modelNum"></param>
	//void RayCast(ModelComponent* mComp, RayCastData& rayData);
	void RayCastComponent(RayCastData& rayData);
	void SetModelNum(int modelNum);
	/// <summary>
	/// �������C�L���X�g�Ɏg�����f���ԍ��̍폜
	/// </summary>
	/// <param name="modelNum"></param>
	void DeleteModelNum(int modelNum);
	void AllDeleteModelNum();
	Texture* GetNormalMap();
	//int AddMComponentList(ModelComponent* mComp);
	//void DeleteComponentList(ModelComponent* mComp);
	XMFLOAT3 GetBonePosition(int modelNum,std::string boneName);
	std::string GetModelName(int modelNum);
	void Release();
};

