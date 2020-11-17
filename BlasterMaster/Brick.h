#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject
{
	float width, height;
public:
	CBrick(float width, float height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetHeight() { return height; }
	int GetWidth() { return width; }


	// these funtion are useless
	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	void SubDamage(int damage) { EnermiesHealth += damage; }
};