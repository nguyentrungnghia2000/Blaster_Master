#include "Portal1.h"


CPortal1::CPortal1(float l, float t, float r, float b, int scene_id)
{
	this->scene_id = scene_id;
	x = l;
	y = t;
	width = r - l + 1;
	height = b - t + 1;
}

void CPortal1::Render()
{
	RenderBoundingBox();
}

void CPortal1::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}