#pragma once
#include "GameObject.h"

#define BIGHUMAN_WALKING_SPEED		0.1f

#define BIGHUMAN_STATE_IDLE_Y	1
#define BIGHUMAN_STATE_IDLE_X	2
#define BIGHUMAN_STATE_MOVE_UP	3
#define BIGHUMAN_STATE_MOVE_DOWN	4
#define BIGHUMAN_STATE_MOVE_RIGHT	5
#define BIGHUMAN_STATE_MOVE_LEFT	6

#define BIGHUMAN_ANI_IDLE_UP	0
#define BIGHUMAN_ANI_MOVE_UP	1
#define BIGHUMAN_ANI_IDLE_DOWN	2
#define BIGHUMAN_ANI_MOVE_DOWN	3
#define BIGHUMAN_ANI_IDLE_RIGHT	4
#define BIGHUMAN_ANI_MOVE_RIGHT	5
#define BIGHUMAN_ANI_IDLE_LEFT	6
#define BIGHUMAN_ANI_MOVE_LEFT	7

#define BIGHUMAN_BBOX_Y_WIDTH	20
#define BIGHUMAN_BBOX_Y_HEIGHT	32
#define BIGHUMAN_BBOX_X_WIDTH	24
#define BIGHUMAN_BBOX_X_HEIGHT	31

#define GATE_TIMER	500

class BigHuman : public CGameObject
{
	int level;
	int health, power;
	int untouchable;
	DWORD untouchable_start;

	float start_x;
	float start_y;
public:
	int ani = -1;
	DWORD bighuman_timer, gate_timer;
	bool isActive, isAttack;
	bool isLadder, isMovingonLadder;

	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	void SubDamage(int damage) { EnermiesHealth += damage; }

	BigHuman(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Reset();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Get_health(int& health) { health = this->health; }
	void Get_power(int& power) { power = this->power; }
	void Get_BigHumanDirection(int& direction) { direction = nx; }
};

