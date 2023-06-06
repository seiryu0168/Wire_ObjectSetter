#include "Controller.h"
#include"ImGui/imgui_impl_dx11.h"
#include"ImGui/imgui_impl_win32.h"
#include"SettingObject.h"
#include"SettingObjectManager.h"
#include"Engine/SceneManager.h"
#include"Stage.h"
#include<filesystem>
#include"Engine/DirectX_11/Input.h"
namespace
{
	static const XMINT2 OBJECT_LIST_WINDOW_POS = { 1520, 0 };
	static const XMINT2 OBJECT_LIST_WINDOW_SIZE = { 400,1080};
}
Controller::Controller(GameObject* parent)
	:GameObject(parent,"Controller"),
	isInputToForm_(false),
	isSelecting_(false),
	isFinalConfirmation_(false)
{
}

Controller::~Controller()
{
	SOM_->Release();
}

void Controller::Initialize()
{
	SOM_ = new SettingObjectManager(GetScene());
	fileExporter_.SetControlObject(this);
	//StartUI();
}

void Controller::Update()
{
	if (Input::IsKey(DIK_LCONTROL) && Input::IsKeyDown(DIK_S))
	{
		fileExporter_.SaveFile();
	}
	CreateStage("");
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
	
}

void Controller::AddSettingObject()
{
	if (ImGui::Button("AddObject"))
	{
		isSelecting_ = !isSelecting_;
		CountFile();
	}


	if (isSelecting_)
	{
		ImGui::TextWrapped("SelectEnemy");
		//CountFile();
		for (std::string name : nameList_)
		{
			//���O�̃I�u�W�F�N�g�𐶐�
			//(���̂�c_str�Ŗ��O�����Ă����Ȃ��Ə�肭�ʂ�Ȃ�)
			if (ImGui::Button(name.c_str()))
				SOM_->CreateSettingObject(name.c_str());

			ImGui::SameLine();
		}
	}
}

void Controller::CreateStage(std::string stageName)
{
	ImGui::Begin("Stage");
	char buf[64] = "";
	size_t size = 0;

	isInputToForm_ = ImGui::InputText("ModelPath", buf, sizeof(buf));
	
	if (Input::IsKeyDown(DIK_RETURN)&& ImGui::IsAnyItemActive()==false)
	{
			GameObject* pObj = nullptr;
			if (pObj = FindObject("Stage"))
			{
				pObj->KillMe();
			}
			isInputToForm_ = false;
			Stage* pStage = Instantiate<Stage>(GetScene());
			pStage->LoadModel(buf);
	}
	ImGui::End();
}
void Controller::Reset()
{
	if (ImGui::Button("Reset")||isFinalConfirmation_)
	{
		if (YESorNO("�{���Ƀ��Z�b�g���܂����H"))
		{
			SceneManager* manager = (SceneManager*)FindObject("SceneManager");
			manager->ChangeScene(SCENE_ID::SCENE_ID_MAIN);
		}
	}
}

bool Controller::YESorNO(std::string message)
{
	isFinalConfirmation_ = true;
	ImGui::Begin(message.c_str());
	bool isReset = false;
	if (ImGui::Button("Yes"))
	{
		isFinalConfirmation_ = false;
		isReset = true;
	}
	else if (ImGui::Button("No"))
	{
		isFinalConfirmation_ = false;
		isReset = false;
	}
	ImGui::End();

	return isReset;


}

int Controller::CountFile()
{
	//���̍�ƃf�B���N�g���擾
	WCHAR currendDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currendDir);
	
	//�f�B���N�g���ړ�
	if (SetCurrentDirectory(L"Assets") == ERROR_FILE_NOT_FOUND)
	{
		return 0;
	}
	WCHAR cDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, cDir);
	int fileCount = 0;
	std::error_code ec;
	std::filesystem::directory_iterator it{ "Image\\IconImage",ec };
	std::filesystem::directory_iterator first;
	std::filesystem::directory_iterator last;

	last = std::filesystem::end(it);
	nameList_.clear();
	for (first = std::filesystem::begin(it); first != last; first++)
	{
		if (std::filesystem::is_directory(*first) == false)
		{
			//�摜����؂藣��
			std::string str = first->path().filename().string();
			std::string name;
			_splitpath_s(str.c_str(), nullptr, NULL, nullptr, NULL, (char*)name.c_str(), str.size(), nullptr, NULL);
			nameList_.push_back(name);
			fileCount++;
		}
	}

	//�f�B���N�g����߂�
	SetCurrentDirectory(currendDir);
	return fileCount;
}

void Controller::ControlObjectData(GameObject* parentObject)
{
	if (parentObject == nullptr)
	{
		return;
	}
	bool t = true;

	ImGui::SetNextWindowPos(ImVec2(OBJECT_LIST_WINDOW_POS.x, OBJECT_LIST_WINDOW_POS.y));
	ImGui::SetNextWindowSize(ImVec2(OBJECT_LIST_WINDOW_SIZE.x, OBJECT_LIST_WINDOW_SIZE.y));
	ImGui::Begin("ObjectData", &t,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
	for (auto i : *SOM_->GetEnemyList())
	{
		std::string str = i->GetSettingObjectName()+std::to_string(i->GetObjectNum());
		//bool k = ImGui::TreeNode((const char*)str.c_str());
		if (ImGui::TreeNode(str.c_str()))
		{
			//���W�A��]�A�T�C�Y�̏���\��
			float pos[3] = { i->GetPosition().x,i->GetPosition().y ,i->GetPosition().z };
			ImGui::DragFloat3("position", pos);
			i->SetPosition({ pos[0], pos[1], pos[2] });
			float rotate[3] = { i->GetRotate().x,i->GetRotate().y ,i->GetRotate().z };
			ImGui::DragFloat3("rotation", rotate);
			i->SetRotate({ rotate[0],rotate[1],rotate[2] });
			float scale[3] = { i->GetScale().x,i->GetScale().y ,i->GetScale().z };
			ImGui::DragFloat3("scale", scale);
			i->SetScale({ scale[0],scale[1],scale[2] });


			//PathForm(i);
			bool isDraw = i->IsDraw();
			ImGui::Checkbox("draw", &isDraw);
			i->SetDrawFlag(isDraw);
			ImGui::TreePop();
		}

	}
		AddSettingObject();
		ImGui::SameLine();
		Reset();
		

	ImGui::End();

	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Controller::Release()
{
	//CleanUpUI();
}


