#pragma once
#include "GameObject.h"

#define HUMAN_WALKING_SPEED		0.07f
#define HUMAN_LYING_SPEED		0.05f
#define HUMAN_MOVE_ON_LADDER_SPEED		0.05f
#define HUMAN_JUMP_SPEED_Y		0.23f
#define HUMAN_JUMP_DEFLECT_SPEED 0.1f
#define HUMAN_GRAVITY			0.0005f
#define HUMAN_DIE_DEFLECT_SPEED	 0.5f

#define HUMAN_STATE_IDLE	1
#define HUMAN_STATE_WALKING_RIGHT	3
#define HUMAN_STATE_WALKING_LEFT	4
#define HUMAN_STATE_DIE	5
#define HUMAN_STATE_JUMP	6
#define HUMAN_STATE_LIE	7
#define HUMAN_STATE_LIE_MOVE_RIGHT	8
#define HUMAN_STATE_LIE_MOVE_LEFT	9
#define HUMAN_STATE_MOVE_UP_LADDER	10
#define HUMAN_STATE_MOVE_DOWN_LADDER	11

#define HUMAN_ANI_IDLE_RIGHT	0
#define HUMAN_ANI_WALK_RIGHT	1
#define HUMAN_ANI_IDLE_LEFT		2
#define	HUMAN_ANI_WALK_LEFT		3
#define HUMAN_ANI_JUMP_RIGHT	4
#define HUMAN_ANI_JUMP_LEFT		5
#define HUMAN_ANI_IDLE_LIE_RIGHT	6
#define HUMAN_ANI_LIE_MOVE_RIGHT	7
#define HUMAN_ANI_IDLE_LIE_LEFT	8
#define HUMAN_ANI_LIE_MOVE_LEFT	9
#define HUMAN_ANI_MOVE_ON_LADDER	10

#define HUMAN_BBOX_WIDTH	10
#define HUMAN_BBOX_HEIGHT	17
#define HUMAN_LIE_BBOX_WIDTH	16
#define HUMAN_LIE_BBOX_HEIGHT	10

#define HUMAN_APPEAR_DISTANCE_WIDTH_RIGHT	10
#define HUMAN_APPEAR_DISTANCE_WIDTH_LEFT	12
#define HUMAN_APPEAR_DISTANCE_HEIGHT	0


class Human : public CGameObject
{
	int level;
	int health, power;
	int untouchable;
	DWORD untouchable_start;

	float start_x;
	float start_y;
public:
	int ani = -1;
	bool IsJumping, PressJump;
	int current_frame_human;
	DWORD human_timer;
	bool isActive, isAttack;
	bool isLying, isCollisionWithCar;
	bool isOverWorld;
	bool PressDown, PressUp;
	bool isLadder, isMovingonLadder;

	int Get_ID() { return ID; }
	float Get_width() { return width; }
	float Get_height() { return height; }
	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	void SubDamage(int damage) { EnermiesHealth += damage; }

	Human(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Reset();
	void Get_health(int& health) { health = this->health; }
	void Get_power(int& power) { power = this->power; }
	void Get_HumanDirection(int& direction) { direction = nx; }
};

