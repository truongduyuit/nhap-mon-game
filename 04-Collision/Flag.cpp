#include "Flag.h"

void CFlag::setSize(int w, int h)
{
	this->width = w;
	this->height = h;
}

void CFlag::Render()
{
	if (renderBBox)RenderBoundingBox();
}

void CFlag::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = left + this->width;
	bottom = top + this->height;
}