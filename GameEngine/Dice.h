#pragma once
#include"Quad.h"
class Dice : public Quad
{
public:

	Dice();
	~Dice();
	//HRESULT Initialize() override;
	void InitVertex() override;
	void InitIndex() override;

};
