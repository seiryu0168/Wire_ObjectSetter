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
	WCHAR fileName[MAX_PATH]=L"無題.json";//ファイル名
	
	OPENFILENAME ofn;		 //保存ダイアログの設定構造体
	ZeroMemory(&ofn, sizeof(ofn));//初期化
	ofn.lStructSize = sizeof(OPENFILENAME);//構造体のサイズ
	ofn.lpstrFilter = TEXT("json(*.json)\0*.json\0");//ファイルの種類
	ofn.lpstrFile = fileName;//ファイル名設定
	ofn.nMaxFile = MAX_PATH;//パスの最大文字数
	ofn.Flags = OFN_OVERWRITEPROMPT;//同じ名前のファイルがあった時確認する
	ofn.lpstrDefExt = L"json";
	
	//今のディレクトリを持っておく
	WCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);
	//ファイルの保存先を決定(キャンセルの場合false)
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

	//ここに入力データを作る関数置く

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
	//ダイアログでファイルの保存場所指定するとディレクトリずれるので
	//ディレクトリ直す
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
