#pragma once
#include "GameObject.h"

#include "Brick.h"

#define MAYBUG_SPEED_Y 0.05f

#define MAYBUG_BBOX_WIDTH 18
#define MAYBUG_BBOX_HEIGHT 18

#define MAYBUG2_STATE_ATTACK	100

#define MAYBUG_ANI_ROLLING 0

class MayBug2 :public CGameObject
{
	LPGAMEOBJECT Target;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	MayBug2(LPGAMEOBJECT, int, int);
	virtual void SetState(int state);
	float CountVx(LPGAMEOBJECT, int, int);

	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	bool Get_IsDead() { return this->IsDead; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	void SubDamage(int damage) { EnermiesHealth += damage; };
};

