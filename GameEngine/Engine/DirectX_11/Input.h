#pragma once
#define TILT_MAX 32768.0f
#define TRIGGER_MAX 255.0f
#include <dInput.h>
#include"Direct3D.h"
#include"XInput.h"
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")
#pragma comment(lib,"Xinput.lib")
#include"../SAFE_DELETE_RELEASE.h"

namespace Input
{
	void Initialize(HWND hWnd);
	void Update();
	//�L�[�{�[�h
	bool IsKey(int keyCode);
	bool IsKeyDown(int keyCode);
	bool IsKeyUp(int keyCode);

	//�}�E�X
	bool IsMouseButton(int buttonCode);
	bool IsMouseButtonDown(int buttonCode);
	bool IsMouseButtonUp(int buttonCode);

	//�R���g���[���[
	bool IsPadButton(int buttonCode);
	bool IsPadButtonDown(int buttonCode);
	bool IsPadButtonUp(int buttonCode);

	float GetLStick_X();
	float GetLStick_Y();
	float GetRStick_X();
	float GetRStick_Y();



	float GetLTrigger();
	float GetLTriggerDown();
	float GetLTriggerUp();

	float GetRTrigger();
	float GetRTriggerDown();
	float GetRTriggerUp();

	XMVECTOR GetMousePosition();
	XMVECTOR GetMouseMove();
	void SetMousePosition(int x, int y);
	void Release();

}