#pragma once
#include "GameObject.h"
#include"../../EngineTime.h"

class RootJob : public GameObject
{
public:
	RootJob();
	~RootJob();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

