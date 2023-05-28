#pragma once
#include"ImGui/imgui_impl_dx11.h"
#include"ImGui/imgui_impl_win32.h"
#include<string>
#include"Engine/DirectX_11/Direct3D.h"

class GameObject;
namespace DebugUI
{
	/// <summary>
	/// 初期化 Mainのループが始まる前に呼ぶ
	/// </summary>
	/// <param name="hWnd">ウィンドウのハンドル</param>
	/// <param name="pDevice">デバイス</param>
	/// <param name="pContext">コンテキスト</param>
	void Initialize(HWND hWnd,ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	
	/// <summary>
	/// フレームの最初に
	/// </summary>
	void StartImGui();
	void Debug(GameObject* object);
	void DebugLog(GameObject* object, std::string message);
	void Log();
	void CleanUp();
	void PrintProcessMemory();
	void GetProcess(DWORD processID);
	void ObjectCount(GameObject* object);
	void CountSub(GameObject* object);
};

