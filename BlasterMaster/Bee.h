#pragma once
#include"GameObject.h"
#include"Brick.h"

#define BEE_FLYING_SPEED_X 0.02f
#define BEE_FLYING_SPEED_Y 0.03f
#define BEE_GRAVITY 0.0005f

#define BEE_FLYING_TIME 700
#define BEE_SURFING_TIME 300

#define BEE_BBOX_WIDTH 18
#define BEE_BBOX_HEIGHT 18

#define	BEE_STATE_UNACTIVE 0
#define BEE_STATE_FLYING 100
#define BEE_STATE_SURFING 200

#define BEE_ANI_FLYING_LEFT 0
#define BEE_ANI_FLYING_RIGHT 1

#define DISTANCE 100

class Bee :public CGameObject
{
	bool FlyDown;
	Timer* time;
	LPGAMEOBJECT target;
	bool IsFlyingDown;
	bool IsBrickTop;
	bool IsActive;
	//bool IsFinish;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	Bee(LPGAMEOBJECT);
	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	virtual void SetState(int state);

	float Get_width() { return width; }
	float Get_height() { return height; }
	void SubDamage(int damage) { EnermiesHealth += damage; }
};

