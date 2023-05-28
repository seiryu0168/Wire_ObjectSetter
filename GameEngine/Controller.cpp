#include "Controller.h"
#include"ImGui/imgui_impl_dx11.h"
#include"ImGui/imgui_impl_win32.h"
#include"SettingObject.h"
#include"SettingObjectManager.h"
#include"Engine/DirectX_11/Input.h"
Controller::Controller(GameObject* parent)
	:GameObject(parent,"Controller"),
	isInputToForm_(false)
{
}

Controller::~Controller()
{
}

void Controller::Initialize()
{
	SOM_ = new SettingObjectManager(GetScene());
	//StartUI();
}

void Controller::Update()
{
	//SOM_->Update();
	if (Input::IsKey(DIK_LSHIFT)&&Input::IsKeyDown(DIK_A)&& ImGui::IsAnyItemActive()==false)
	{
		SOM_->CreateSettingObject("Enemy");
	}
	ControlObjectData(GetParent());
}

void Controller::InitUI(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/*IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);*/
}

void Controller::StartUI()
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
}

void Controller::CleanUpUI()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Controller::PathForm(GameObject* obj)
{
	char buf[64] = "";
	size_t size=0;

	isInputToForm_=ImGui::InputText("ModelPath", buf, sizeof(buf));
	if (isInputToForm_ == true)
	{
		int s = 0;
	}
	if (Input::IsKeyDown(DIK_RETURN))
	{
		isInputToForm_ = false;
		((SettingObject*)obj)->LoadModel(buf);
	}
	//ImGui::SameLine();
}

void Controller::ControlObjectData(GameObject* parentObject)
{
	if (parentObject == nullptr)
	{
		return;
	}
	ImGui::Begin("ObjectData");
	for (auto i : *SOM_->GetEnemyList())
	{
		std::string str = i->GetSettingObjectName().c_str();
		//bool k = ImGui::TreeNode((const char*)str.c_str());
		if (ImGui::TreeNode(i->GetSettingObjectName().c_str()))
		{
			//座標、回転、サイズの情報を表示
			float pos[3] = { i->GetPosition().x,i->GetPosition().y ,i->GetPosition().z };
			ImGui::DragFloat3("position", pos);
			i->SetPosition({ pos[0], pos[1], pos[2] });
			float rotate[3] = { i->GetRotate().x,i->GetRotate().y ,i->GetRotate().z };
			ImGui::DragFloat3("rotation", rotate);
			i->SetRotate({ rotate[0],rotate[1],rotate[2] });
			float scale[3] = { i->GetScale().x,i->GetScale().y ,i->GetScale().z };
			ImGui::DragFloat3("scale", scale);
			i->SetScale({ scale[0],scale[1],scale[2] });


			PathForm(i);
			bool isDraw = i->IsDraw();
			ImGui::Checkbox("draw", &isDraw);
			i->SetDrawFlag(isDraw);
			ImGui::TreePop();
		}
	}
	ImGui::End();

	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Controller::Release()
{
	//CleanUpUI();
}


