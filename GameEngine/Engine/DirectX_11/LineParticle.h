#pragma once
#include"Direct3D.h"
#include"Texture.h"
#include"DirectXMath.h"
#include"../GameObject/Transform.h"
#include<list>

class LineParticle
{
	enum class LineMode
	{
		LINE_DEFAULT=0,
		LINE_CROSS,
	};

private:
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMFLOAT4	color;
	};

	//���_���
	struct VERTEX
	{
		XMFLOAT3 position;
		XMFLOAT3 uv;
	};

	float WIDTH_;//���C���p�[�e�B�N���̕�
	UINT LENGTH_;//���C���p�[�e�B�N���̃|�W�V�������L�������
	float tipWidth_;
	XMFLOAT4 color_;
	//int* index_;

	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;

	Texture* pTexture_;
	std::vector<int> indexList;
	std::list<XMFLOAT3> positionList_;
public:
	LineParticle();
	LineParticle(float width,int length,float tipWidth);
	/// <summary>
	/// ���C���̈ʒu�ۑ�
	/// �ʒu���|�W�V�������X�g�ɒǉ�
	/// </summary>
	/// <param name="pos">���̊֐����Ă΂ꂽ���̈ʒu</param>
	void AddPosition(XMFLOAT3 pos);
	/// <summary>
	/// �p�C�v��̃��C���p�[�e�B�N������
	/// </summary>
	/// <param name="pList"></param>
	/// <returns></returns>
	HRESULT CreateMeshPype(std::list<XMFLOAT3>* pList);
	HRESULT CreateMeshPlate(std::list<XMFLOAT3>* pList);
	/// <summary>
	/// ���C���p�[�e�B�N���Ɏg���摜�̃��[�h
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	HRESULT Load(std::string fileName);
	/// <summary>
	/// ���C���p�[�e�B�N���̐F�ݒ�
	/// </summary>
	/// <param name="col">�F</param>
	void SetColor(XMFLOAT4 col);
	/// <summary>
	/// �C���f�b�N�X����
	/// </summary>
	void SetIndex();
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="transform">�Ăяo������transform</param>
	void Draw(Transform* transform);
	/// <summary>
	/// ���C���p�[�e�B�N���̊e�p�����[�^�ݒ�
	/// </summary>
	/// <param name="width">����</param>
	/// <param name="length">�|�W�V�����̕ۑ���</param>
	/// <param name="tipWidth">�I�[�̕�</param>
	void SetLineParameter(float width, int length,float tipWidth=0);
	/// <summary>
	/// �|�W�V�������X�g�̍폜
	/// </summary>
	void DeleteLine();
	void Release();



};

