#include "DebugUI.h"
#include"Engine/SceneManager.h"
#include<psapi.h>


namespace DebugUI
{
	int objectCount_;
	struct debugData
	{
		std::string objectName_;
		std::string message_;
	};
	std::vector<debugData*> debugLogs;

}
	
void DebugUI::Initialize(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io=ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice,pContext);
}

void DebugUI::Debug(GameObject* object)
{
	ImGui::Begin("Debug");
	ObjectCount(*(object->GetChildList()->begin()));
	std::string count = std::to_string(objectCount_);
	PrintProcessMemory();
	
	ImGui::Text(count.c_str());
	ImGui::Text(object->GetObjectName().c_str());
	objectCount_ = 0;
	ImGui::End();
	
}

void DebugUI::DebugLog(GameObject* object, std::string message)
{
	debugData* pData = new debugData;
	pData->objectName_ = object->GetObjectName();
	pData->message_ = message;
	debugLogs.push_back(pData);
}

void DebugUI::StartImGui()
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
}

void DebugUI::Log()
{
	ImGui::Begin("Log");
	for (int i = debugLogs.size()-1; i >=0; i--)
	{
		std::string msg = debugLogs[i]->objectName_ + " : " + debugLogs[i]->message_;
		ImGui::Text(msg.c_str());
	}
	ImGui::End();
}

void DebugUI::CleanUp()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DebugUI::PrintProcessMemory()
{
	ImGui::Begin("ProcessMemory");

	DWORD aProcesses = GetCurrentProcessId();	
	GetProcess(aProcesses);

	ImGui::End();
}

void DebugUI::GetProcess(DWORD processID)
{
	HANDLE hProcess;
	PROCESS_MEMORY_COUNTERS pmc;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	if (hProcess==NULL)
	{
		return;
	}
	GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc));	
	ImGui::Text(std::to_string(pmc.PagefileUsage).c_str());
	ImGui::Text(std::to_string(pmc.PeakPagefileUsage).c_str());
	ImGui::Text(std::to_string(pmc.QuotaPagedPoolUsage).c_str());
	ImGui::Text(std::to_string(pmc.QuotaPeakPagedPoolUsage).c_str());
	ImGui::Text(std::to_string(pmc.QuotaNonPagedPoolUsage).c_str());
	ImGui::Text(std::to_string(pmc.QuotaPeakNonPagedPoolUsage).c_str());
	ImGui::Text(std::to_string(pmc.WorkingSetSize).c_str());
	ImGui::Text(std::to_string(pmc.PeakWorkingSetSize).c_str());
	CloseHandle(hProcess);
}

void DebugUI::ObjectCount(GameObject* object)
{
	if (object == nullptr)
	{
		return;
	}
	objectCount_++;
	if (ImGui::TreeNode(object->GetObjectName().c_str()))
	{
		//座標、回転、サイズの情報を表示
		float pos[3] = { object->GetPosition().x,object->GetPosition().y ,object->GetPosition().z };
		ImGui::DragFloat3("position", pos);
		object->SetPosition({ pos[0], pos[1], pos[2] });
		float rotate[3] = { object->GetRotate().x,object->GetRotate().y ,object->GetRotate().z };
		ImGui::DragFloat3("rotation", rotate);
		object->SetRotate({ rotate[0],rotate[1],rotate[2] });
		float scale[3] = { object->GetScale().x,object->GetScale().y ,object->GetScale().z };
		ImGui::DragFloat3("scale", scale);
		object->SetScale({ scale[0],scale[1],scale[2] });



		bool isDraw = object->IsDraw();
		ImGui::Checkbox("draw",&isDraw);
		object->SetDrawFlag(isDraw);
		//再帰で自分の子の情報を表示
		for (auto itr = object->GetChildList()->begin(); itr != object->GetChildList()->end(); itr++)
		{
			ObjectCount(*itr);
		}
		ImGui::TreePop();
	}
}

void DebugUI::CountSub(GameObject* object)
{
	if (object == nullptr)
	{
		return;
	}

	if (ImGui::TreeNode(object->GetObjectName().c_str()))
	{

		float pos[3] = { object->GetPosition().x,object->GetPosition().y ,object->GetPosition().z };
		ImGui::DragFloat3("position", pos);
		object->SetPosition({ pos[0], pos[1], pos[2] });
		float rotate[3] = { object->GetRotate().x,object->GetRotate().y ,object->GetRotate().z };
		ImGui::DragFloat3("rotation", rotate);
		object->SetRotate({ rotate[0],rotate[1],rotate[2] });
		float scale[3] = { object->GetScale().x,object->GetScale().y ,object->GetScale().z };
		ImGui::DragFloat3("scale", scale);
		object->SetScale({ scale[0],scale[1],scale[2] });
		ImGui::TreePop();
	}

}