#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/DirectX_11/Direct3D.h"
class SettingObjectManager;

class Controller : public GameObject
{
	SettingObjectManager* SOM_;
	bool isInputToForm_;
	bool isSelecting_;
	bool isFinalConfirmation_;
	

	std::vector<std::string> nameList_;
public:
	Controller(GameObject* parent);
	~Controller();
	void Initialize() override;
	void Update() override;
	void InitUI(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void StartUI();
	void CleanUpUI();
	void PathForm(GameObject* obj);
	void AddSettingObject();
	void CreateStage(std::string stageName);
	bool YESorNO(std::string message);
	int	 CountFile();
	void ControlObjectData(GameObject* parentObject);
	void Reset();
	void Release() override;
};

