#pragma once
#include<DirectXMath.h>
#include<fbxsdk.h>
#include<d3d11.h>
#include"Texture.h"
#include"../GameObject/Transform.h"
#include"Fbx.h"

class FbxParts
{
private:
	//�}�e���A��
	//����
	struct MATERIAL
	{
		Texture* pTexture;    //�e�N�X�`��
		Texture* pNormalMap;  //�m�[�}���}�b�v
		XMFLOAT4 diffuse;	  //�f�B�t���[�Y(�}�e���A���̐F)
		XMFLOAT4 ambient;	  //�A���r�G���g
		XMFLOAT4 speculer;    //�X�y�L�����[(�c���c����\������z)
		float shininess;	  //�n�C���C�g�̋���

	};

	//�V�F�[�_�[�ɑ�������܂Ƃ߂��\����
	struct CONSTANT_BUFFER
	{
		XMMATRIX matWVP;			//���[���h�A�r���[�A�v���W�F�N�V�����s��̍���(���_�ϊ��Ɏg��)
		XMMATRIX matW;				//���[���h�s��
		XMMATRIX matNormal;			//��]�s��Ɗg��s��̍���(�@���̕ό`�Ɏg��)
		XMFLOAT4 diffuseColor;		//�f�B�t���[�Y(�}�e���A���̐F)
		XMFLOAT4 ambient;			//�A���r�G���g
		XMFLOAT4 speculer;			//�X�y�L�����[
		XMFLOAT4 lightDirection;	//���C�g�̌���
		XMFLOAT4 cameraPosition;	//�J�����̈ʒu
		XMFLOAT4 customColor;       //�v���O�������ŐF��ς������ꍇ
		FLOAT	 shininess;			//�n�C���C�g�̋���
		BOOL	 isTexture;			//�e�N�X�`�����\���Ă��邩�ǂ���
		BOOL     isNormal;	//�m�[�}���}�b�v�����邩�ǂ���
	};

	//���_�Ɋi�[�������
	//maya����fbx�o�͂��鎞�ɒ��_�@���̐ݒ�����Ȃ��ƕ\�������������Ȃ�
	//blender�̏ꍇ�͎����X���[�Y��؂�Ɛ������\�������
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;//blender�ł̓X���[�Y�V�F�[�h�ɂ�����Ńm�[�}���̎����X���[�Y��؂�Ȃ��Ɩ@���̃f�[�^�����������Ȃ��Đ������\������Ȃ�
		XMVECTOR tangent;//�ڐ�
	};

	struct BONE
	{
		XMMATRIX bindPose;		//�����|�[�Y���̃{�[���ϊ��s��
		XMMATRIX newPose;		//�A�j���[�V�����ŕω��������̃{�[���ϊ��s��
		XMMATRIX diffPose;		//mBindPose�ɑ΂���mNowPose�̕ω���
	};

	struct WEIGHT
	{
		XMVECTOR originPos;			//���̒��_���W
		XMVECTOR originNormal;		//���̖@���x�N�g��
		int*	 pBoneIndex;		//�֘A����{�[����ID
		float*	 pBoneWeight;		//�{�[���̃E�F�C�g
	};

	int vertexCount_;		//���_��
	int polygonCount_;		//�|���S����
	int materialCount_;		//�}�e���A����
	int boneNum_;			//�{�[���̐�


	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer** ppIndexBuffer_;	//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@

	FbxSkin*	 pSkinInfo_;     //�X�L�����b�V���̏��
	FbxCluster** ppCluster_;	 //�N���X�^���
	BONE*		 pBoneArray_;	 //�e�{�[���̏��
	WEIGHT*		 pWeightArray_;	 //�E�F�C�g�̏��
	
	MATERIAL* pMaterialList_;//�}�e���A�����X�g
	int*	  indexCount_;

	int**	ppIndex_;
	VERTEX* pVertices_;
	Texture* pToonTexture_;

	HRESULT InitVertex(fbxsdk::FbxMesh* mesh);
	HRESULT InitIndex(fbxsdk::FbxMesh* mesh);
	HRESULT CreateConstantBuffer();
	HRESULT InitSkelton(FbxMesh* pMesh);
	void InitMaterial(fbxsdk::FbxNode* pNode);
	void CalcTangent(VERTEX& vertex0, const VERTEX& vertex1, const VERTEX& vertex2);
public:
	FbxParts();
	~FbxParts();

	HRESULT Init(FbxNode* pNode);
	void Draw(Transform& transform, XMFLOAT4 lineColor = { 0,0,0,1 });
	void DrawSkinAnime(Transform& transform, FbxTime time, XMFLOAT4 lineColor = { 0,0,0,0 });
	//void DrawMeshAnime(Transform& transform, FbxTime time, FbxScene* scene);
	FbxSkin* GetSkinInfo() { return pSkinInfo_; }
	bool GetBonePosition(std::string boneName, XMFLOAT3* position);
	void RayCast(RayCastData& rayData, Transform& transform);



};

