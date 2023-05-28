#pragma once
#include"ImGui/imgui_impl_dx11.h"
#include"ImGui/imgui_impl_win32.h"
#include<string>
#include"Engine/DirectX_11/Direct3D.h"

class GameObject;
namespace DebugUI
{
	/// <summary>
	/// ������ Main�̃��[�v���n�܂�O�ɌĂ�
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�̃n���h��</param>
	/// <param name="pDevice">�f�o�C�X</param>
	/// <param name="pContext">�R���e�L�X�g</param>
	void Initialize(HWND hWnd,ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	
	/// <summary>
	/// �t���[���̍ŏ���
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

