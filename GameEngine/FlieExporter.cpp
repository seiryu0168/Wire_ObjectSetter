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
	WCHAR fileName[MAX_PATH]=L"無題.json";//ファイル名
	
	OPENFILENAME ofn;		 //保存ダイアログの設定構造体
	ZeroMemory(&ofn, sizeof(ofn));//初期化
	ofn.lStructSize = sizeof(OPENFILENAME);//構造体のサイズ
	ofn.lpstrFilter = TEXT("json(*.json)\0*.json\0");//ファイルの種類
	ofn.lpstrFile = fileName;//ファイル名設定
	ofn.nMaxFile = MAX_PATH;//パスの最大文字数
	ofn.Flags = OFN_OVERWRITEPROMPT;//同じ名前のファイルがあった時確認する
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

	//ここに入力データを作る関数置く

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
