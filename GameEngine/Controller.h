#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/DirectX_11/Direct3D.h"
class SettingObjectManager;

class Controller : public GameObject
{
	SettingObjectManager* SOM_;
	bool isInputToForm_;
public:
	Controller(GameObject* parent);
	~Controller();
	void Initialize() override;
	void Update() override;
	void InitUI(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void StartUI();
	void CleanUpUI();
	void PathForm(GameObject* obj);
	void ControlObjectData(GameObject* parentObject);
	void Release() override;
};

