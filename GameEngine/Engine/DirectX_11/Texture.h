#pragma once
#include <d3d11.h>
#include "string"

class Texture
{

	ID3D11SamplerState* pSampler_;		//�T���v���[
	ID3D11ShaderResourceView* pSRV_;	//�V�F�[�_�[���\�[�X�r���[


	UINT imgWidth_;
	UINT imgHeight_;
public:
	Texture();
	~Texture();
	/// <summary>
	/// �摜���[�h(�t�@�C�����̂�)
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <returns></returns>
	HRESULT Load(LPCWSTR fileName);
	/// <summary>
	/// �摜���[�h(�p�X����)
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <returns></returns>
	HRESULT Load(std::string fileName);
	ID3D11SamplerState* GetSampler(){ return pSampler_; }
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }


	UINT GetWidth();
	UINT GetHeight();
	void Release();
};