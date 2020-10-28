﻿#pragma once
#include "GameObject.h"

#define CAR_WALKING_SPEED		0.15f 
//0.1f
#define CAR_JUMP_SPEED_Y		0.5f
#define CAR_JUMP_DEFLECT_SPEED 0.2f
#define CAR_GRAVITY			0.002f
#define CAR_DIE_DEFLECT_SPEED	 0.5f

#define CAR_STATE_IDLE			0
#define CAR_STATE_WALKING_RIGHT	1
#define CAR_STATE_WALKING_LEFT	2
#define CAR_STATE_JUMP			3
#define CAR_STATE_DIE			4
#define CAR_STATE_UP			5
#define CAR_STATE_UP_RIGHT		6
#define CAR_STATE_UP_LEFT		7
#define CAR_STATE_WALKING_UP_RIGHT 8
#define CAR_STATE_WALKING_UP_LEFT 9


#define CAR_ANI_IDLE_RIGHT	0
#define CAR_ANI_IDLE_LEFT		1
#define CAR_ANI_WALKING_RIGHT	2
#define CAR_ANI_WALKING_LEFT	3
#define CAR_ANI_UP_RIGHT	4
#define CAR_ANI_UP_RIGHT_2	5
#define CAR_ANI_UP_RIGHT_3	6
#define CAR_ANI_UP_RIGHT_4	7
#define CAR_ANI_UP_LEFT		8
#define CAR_ANI_UP_LEFT_2	9
#define CAR_ANI_UP_LEFT_3	10
#define CAR_ANI_UP_LEFT_4	11
#define CAR_ANI_WALKING_UP_RIGHT 12
#define CAR_ANI_WALKING_UP_LEFT 13
#define CAR_ANI_JUMP_RIGHT	14
#define CAR_ANI_JUMP_LEFT	15
#define CAR_ANI_DIE		16

#define CAR_BBOX_WIDTH  30
#define CAR_BBOX_HEIGHT 18
#define CAR_UP_BBOX_HEIGHT 34
#define CAR_JUMP_BBOX_HEIGHT 21

#define CAR_SMALL_BBOX_WIDTH  13
#define CAR_SMALL_BBOX_HEIGHT 15

#define CAR_UNTOUCHABLE_TIME 5000


class CCar : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;
public:
	int ani = -1;
	int current_frame;
	bool flag = true;
	bool IsJumping;
	bool PressKeyUp = false;
	bool PressJump = false;

	CCar(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	//virtual void Render_Current_Frame();
	CAnimation *curAnimation;
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Reset();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};