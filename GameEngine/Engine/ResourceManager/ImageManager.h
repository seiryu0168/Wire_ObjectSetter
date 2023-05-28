#pragma once
#include"../DirectX_11/Direct3D.h"
#include<vector>
#include<string>

namespace ImageManager
{
	/// <summary>
	/// �摜���[�h
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <returns>�摜�ԍ�</returns>
	int Load(std::string fileName);
	
	/// <summary>
	/// �摜�`��
	/// </summary>
	/// <param name="imageHandle">�摜�ԍ�</param>
	void Draw(int imgHandle);
	void DrawUI();
	
	/// <summary>
	/// �摜�̐؂蔲���͈̓��Z�b�g
	/// </summary>
	/// <param name="imageHandle">�摜�ԍ�</param>
	void ResetRect(int imgHandle);
	
	/// <summary>
	/// �摜�̐؂蔲���͈͐ݒ�
	/// </summary>
	/// <param name="imageHandle">�摜�ԍ�</param>
	/// <param name="top">��</param>
	/// <param name="bottom">��</param>
	/// <param name="left">��</param>
	/// <param name="right">�E</param>
	void SetRect(int imgHandle,int top,int bottom,int left,int right);
	
	/// <summary>
	/// �A���t�@�l�̐ݒ�
	/// </summary>
	/// <param name="imghandle">�摜�ԍ�</param>
	/// <param name="alpha">�A���t�@�l</param>
	void  SetAlpha(int imghandle,float alpha);
	
	/// <summary>
	/// �A���t�@�l�擾
	/// </summary>
	/// <param name="imgHandle">�摜�ԍ�</param>
	/// <returns>�A���t�@�l</returns>
	float GetAlpha(int imgHandle);

	/// <summary>
	/// �摜�̈ʒu�ݒ�
	/// </summary>
	/// <param name="imgHandle">�摜�ԍ�</param>
	/// <param name="pos">�摜�̈ʒu</param>
	void SetImagePos(int imgHandle,XMFLOAT3 pos);
	void ChangeColor(int imgHandle, XMFLOAT4 color);

	/// <summary>
	/// �摜�T�C�Y�ݒ�
	/// </summary>
	/// <param name="imgHandle">�摜�ԍ�</param>
	/// <param name="size">�T�C�Y</param>
	void  SetImageSize(int imgHandle, XMFLOAT3 size);

	void SetUIList(int imgHandle);
	std::string GetImageName(int imgHandle);
	void Release(int imgHandle);
	void AllRelease();
};

