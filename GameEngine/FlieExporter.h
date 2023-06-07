#pragma once
#include"Engine/GameObject/GameObject.h"
#include"Engine/ResourceManager/json.hpp"
class FileExporter
{
private:
	using json = nlohmann::json;
	GameObject* pControlObject_;
public:

	FileExporter();
	~FileExporter();
	void SaveFile();
	std::string EnemyExport();
	std::string ItemExport();
	void SetControlObject(GameObject* obj) { pControlObject_ = obj; }
	std::string CreateExportFile();
	void OpenFile();

};

