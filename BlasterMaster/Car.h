#pragma once
#include "GameObject.h"

#define CAR_WALKING_SPEED		0.1f 
//0.1f
#define CAR_JUMP_SPEED_Y		0.23f
#define CAR_JUMP_DEFLECT_SPEED 0.1f
#define CAR_GRAVITY			0.0004f
#define CAR_DIE_DEFLECT_SPEED	 0.5f

#define CAR_STATE_IDLE			0
#define CAR_STATE_WALKING_RIGHT	1
#define CAR_STATE_WALKING_LEFT	2
#define CAR_STATE_UP			3
#define CAR_STATE_UP_RIGHT		4
#define CAR_STATE_UP_LEFT		5
#define CAR_STATE_WALKING_UP_RIGHT 6
#define CAR_STATE_WALKING_UP_LEFT 7
#define CAR_STATE_JUMP			8
#define CAR_STATE_FALL_DOWN		9
#define CAR_STATE_DIE			10

#define CAR_ANI_IDLE_RIGHT	0
#define CAR_ANI_IDLE_LEFT		1
#define CAR_ANI_WALKING_RIGHT	2
#define CAR_ANI_WALKING_LEFT	3
#define CAR_ANI_FLIP_UP_RIGHT	4
#define CAR_ANI_FLIP_UP_RIGHT_2	5
#define CAR_ANI_FLIP_UP_RIGHT_3	6
#define CAR_ANI_FLIP_UP_RIGHT_4	7
#define CAR_ANI_FLIP_UP_LEFT	8
#define CAR_ANI_FLIP_UP_LEFT_2	9
#define CAR_ANI_FLIP_UP_LEFT_3	10
#define CAR_ANI_FLIP_UP_LEFT_4	11
#define CAR_ANI_WALKING_UP_RIGHT 12
#define CAR_ANI_WALKING_UP_LEFT 13
#define CAR_ANI_JUMP_RIGHT	14
#define CAR_ANI_JUMP_LEFT	15
#define CAR_ANI_DIE		16

#define CAR_BBOX_WIDTH  30
#define CAR_BBOX_HEIGHT 18
#define CAR_UP_BBOX_HEIGHT 34
#define CAR_JUMP_BBOX_HEIGHT 21
#define CAR_DIE_BBOX_WIDTH	45
#define CAR_DIE_BBOX_HEIGTH	40

#define CAR_SMALL_BBOX_WIDTH  13
#define CAR_SMALL_BBOX_HEIGHT 15

#define CAR_UNTOUCHABLE_TIME 5000
#define EXPLOSION_TIME 200


class CCar : public CGameObject
{
	int level;
	int untouchable;
	int health;
	int power;
	DWORD untouchable_start;

	float start_x;
	float start_y;
public:
	int ani = -1;
	int current_frame;
	DWORD timer;
	bool flag = true;
	bool health_up;
	bool IsJumping;
	bool PressKeyUp = false;
	bool PressJump = false;
	bool FlippingUp = false;
	bool isAttack = false;
	bool isDead;
	bool isActive;
	//bool IsFinish;
	CCar(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	//virtual void Render_Current_Frame();
	CAnimation* curAnimation;

	void Set_IsDead(bool value) { this->IsDead = value; }
	bool Get_IsDead() { return this->IsDead; }
	void SubDamage(int damage) { EnermiesHealth += damage; }

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void Get_CarDirection(int& direction) { direction = nx; }
	void Get_CarFlipUp(bool& isTargetTop) { isTargetTop = FlippingUp; }
	void Get_health(int& health) { health = this->health; }
	void Get_power(int& power) { power = this->power; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Reset();
	void Get_CarStateForBullet(int& direct, bool& flipup, float Px, float Py) { direct = direction; flipup = FlippingUp; Px = x; Py = y; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};