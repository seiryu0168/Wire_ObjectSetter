#pragma once
#include<string>

//音
namespace Audio
{
	//初期化
	void Initialize();
	
	/// <summary>
	/// オーディオファイルのロード
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="svNum">最大同時再生数</param>
	/// <returns>ロードしたデータの番号</returns>
	int Load(std::string fileName,int svNum = 1);
	
	/// <summary>
	/// 再生
	/// </summary>
	/// <param name="ID">鳴らしたい音の番号</param>
	void Play(int ID);
	void ReleaseAudio();
	void Releace();

};

