#include "FlieExporter.h"
#include"resource.h"
#include<d3d11.h>
#include<fstream>
#include<sstream>
#include"SettingObjectManager.h"
#include"Controller.h"

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
	
	//���̃f�B���N�g���������Ă���
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);
	//�t�@�C���̕ۑ��������(�L�����Z���̏ꍇfalse)
	bool isSelectDir;
	isSelectDir = GetSaveFileName(&ofn);
	if (isSelectDir == FALSE)
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
	
	std::string exptData = CreateExportFile();
	int a = 0;

	BOOL successWrite = WriteFile(hFile,
						exptData.c_str(),
						exptData.length(),
						&byte,
						NULL);
	if (successWrite == FALSE)
	{
		int a = 0;
	}

	BOOL successClose = CloseHandle(hFile);
	if (successClose==FALSE)
	{
		int a = 0;
	}
	//�_�C�A���O�Ńt�@�C���̕ۑ��ꏊ�w�肷��ƃf�B���N�g�������̂�
	//�f�B���N�g������
	SetCurrentDirectory(currentDir);
}

std::string FileExporter::CreateExportFile()
{
	SettingObjectManager* som = ((Controller*)pControlObject_)->GetSettingObjectManager();
	std::vector<SettingObject*>* enemyList = som->GetEnemyList();
	std::string outstr;
	json out;
	int i = 0;
		out["InitialEnemyStatus"] = json::array();
	for (SettingObject* enem : *enemyList)
	{

		out["InitialEnemyStatus"][i] = json::object();
		XMFLOAT3 position = enem->GetPosition();
		XMFLOAT3 rotate = enem->GetRotate();
		XMFLOAT3 scale = enem->GetScale();
		std::string objName = enem->GetSettingObjectName();
		out["InitialEnemyStatus"][i][objName];
		out["InitialEnemyStatus"][i][objName]["EnemyName"] = objName;
		out["InitialEnemyStatus"][i][objName]["Position"] = { position.x,position.y,position.z };
		out["InitialEnemyStatus"][i][objName]["Rotate"] = { rotate.x,rotate.y, rotate.z };
		out["InitialEnemyStatus"][i][objName]["Scale"] = { scale.x,scale.y, scale.z };
		i++;
	}
	outstr = out.dump();
	return outstr;
}
