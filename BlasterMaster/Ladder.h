#pragma once
#include "GameObject.h"

#define LADDER_BBOX_WIDTH  16
#define LADDER_BBOX_HEIGHT 16

class Ladder : public CGameObject
{
	float W, H;
	bool IsFinish;
public:
	Ladder(float w, float h);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	float Get_width() { return W; }
	float Get_height() { return H; }

	void Set_IsDead(bool value) { this->IsFinish = value; }
	bool Get_IsDead() { return this->IsFinish; }
	void SubDamage(int damage) { EnermiesHealth += damage; }
};

