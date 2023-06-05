#pragma once
#include"Engine/ResourceManager/json.hpp"
class FileExporter
{
private:
	using json = nlohmann::json;
public:
	FileExporter();
	~FileExporter();
	void SaveFile();

};

