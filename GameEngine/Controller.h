#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/DirectX_11/Direct3D.h"
#include"FlieExporter.h"
class SettingObjectManager;
class Controller : public GameObject
{
	SettingObjectManager* SOM_;
	FileExporter fileExporter_;
	bool isInputToForm_;
	bool isSelecting_;
	bool isSelectSaveFile_;
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
	void SelectSaveFile();
	SettingObjectManager* GetSettingObjectManager() { return SOM_; }
	int	 CountFile();
	//int  CountObjectName();
	void ControlObjectData(GameObject* parentObject);
	void Reset();
	void Release() override;
};

