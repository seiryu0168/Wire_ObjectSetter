#include "FlieExporter.h"
#include"resource.h"
#include<d3d11.h>
#include<fstream>
#include<sstream>
#include"SettingObjectManager.h"

FileExporter::FileExporter()
{
}

FileExporter::~FileExporter()
{
}

void FileExporter::SaveFile()
{
	WCHAR fileName[MAX_PATH]=L"����.json";//�t�@�C����
	
	OPENFILENAME ofn;		 //�ۑ��_�C�A���O�̐ݒ�\����
	ZeroMemory(&ofn, sizeof(ofn));//������
	ofn.lStructSize = sizeof(OPENFILENAME);//�\���̂̃T�C�Y
	ofn.lpstrFilter = TEXT("json(*.json)\0*.json\0");//�t�@�C���̎��
	ofn.lpstrFile = fileName;//�t�@�C�����ݒ�
	ofn.nMaxFile = MAX_PATH;//�p�X�̍ő啶����
	ofn.Flags = OFN_OVERWRITEPROMPT;//�������O�̃t�@�C�������������m�F����
	ofn.lpstrDefExt = L"json";

	bool isGetFile;
	isGetFile = GetSaveFileName(&ofn);
	if (isGetFile == FALSE)
		return;

	HANDLE hFile;
	hFile = CreateFile(ofn.lpstrFile,
					   GENERIC_WRITE,
					   0,
					   NULL,
					   CREATE_ALWAYS,
					   FILE_ATTRIBUTE_NORMAL,
					   NULL);

	//�����ɓ��̓f�[�^�����֐��u��

	DWORD byte = 0;
	std::ofstream writingFile;
	std::string outstr;
	json out;
	out = { "name" };
	outstr = out.dump();
	
	int a = 0;
	//WriteFile(hFile,
	//	)
	//writingFile.open()
}
