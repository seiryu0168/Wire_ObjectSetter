#pragma once
#include<string>

//��
namespace Audio
{
	//������
	void Initialize();
	
	/// <summary>
	/// �I�[�f�B�I�t�@�C���̃��[�h
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="svNum">�ő哯���Đ���</param>
	/// <returns>���[�h�����f�[�^�̔ԍ�</returns>
	int Load(std::string fileName,int svNum = 1);
	
	/// <summary>
	/// �Đ�
	/// </summary>
	/// <param name="ID">�炵�������̔ԍ�</param>
	void Play(int ID);
	void ReleaseAudio();
	void Releace();

};

