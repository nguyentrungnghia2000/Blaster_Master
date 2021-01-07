#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "MineBullets.h"

#define BEE_GRAVITY 0.0005f

#define MINE_BBOX_WIDTH 16
#define MINE_BBOX_HEIGHT 10

#define MINE_STATE_NORMAL 0
#define MINE_BOOM 100

#define MINE_NORMAL_ANI 0
#define MINE_BOOM_ANI 1

#define TIME_DIS 200

class Mine :public CGameObject
{
	bool IsBoom;
	Timer* time_dis;
	vector<MineBullets*> Bullet;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	Mine();
	virtual void SetState(int state);
	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	bool Get_IsDead() { return this->IsDead; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	void SubDamage(int damage) { EnermiesHealth += damage; };
};

