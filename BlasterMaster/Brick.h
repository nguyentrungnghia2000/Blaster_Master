#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject
{
	float width, height;
	bool IsFinish;
public:
	CBrick(float width, float height);
	void Set_IsDead(bool value) { this->IsFinish = value; }
	bool Get_IsDead() { return this->IsFinish; }
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetHeight() { return height; }
	int GetWidth() { return width; }

	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	void SubDamage(int damage) { EnermiesHealth += damage; }
};