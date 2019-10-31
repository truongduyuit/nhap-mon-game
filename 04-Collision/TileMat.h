#pragma once

#include "GameObject.h"
#include "Contands.h"

class CTileMat
{
	int id;

	int index_x;
	int index_y;

public:
	
	void Render();

	CTileMat() { ; }
	CTileMat(int id, int x, int y);
	~CTileMat() { ; }
};