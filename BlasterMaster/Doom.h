#pragma once
#include "GameObject.h"
#include "Brick.h"

#define DOOM_WALKING_SPEED_X 0.03f
#define DOOM_WALKING_SPEED_Y 0.03f
#define DOOM_GRAVITY 0.015f

#define DOOM_BBOX_WIDTH_LR 17
#define DOOM_BBOX_HEIGHT_LR 18
#define DOOM_BBOX_WIDTH_TD 18
#define DOOM_BBOX_HEIGHT_TD 17

#define DOOM_STATE_WALKING_DOWN 100
#define DOOM_STATE_WALKING_LEFT 200
#define DOOM_STATE_WALKING_RIGHT 300
#define DOOM_STATE_WALKING_TOP 400
#define DOOM_STATE_SURFING	500

#define DOOM_ANI_WALKING_DOWN 0
#define DOOM_ANI_WALKING_LEFT 1
#define DOOM_ANI_WALKING_RIGHT 2
#define DOOM_ANI_WALKING_TOP 3

class Doom : public CGameObject
{
	int LastState;
	bool IsBrick;
	//bool IsFinish;
	LPGAMEOBJECT Target;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	Doom(LPGAMEOBJECT,int,int);

	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	void SubDamage(int damage) { EnermiesHealth += damage; }

	virtual void SetState(int state);
};