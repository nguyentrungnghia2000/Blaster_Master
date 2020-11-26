﻿#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Car.h"
#include "Brick.h"
#include "Game.h"

#include "Human.h"
#include "Item.h"
#include "Bug.h"
#include "Robot.h"
#include "Bee.h"
#include "MayBug.h"
#include "Doom.h"
#include "Spider.h"
#include "Portal.h"

CCar::CCar(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(CAR_STATE_IDLE);
	health = power = PLAYER_HEALTH;
	this->IsDead = false;
	this->health_up = false;
	this->isActive = true;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CCar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isActive == true) {
		CGameObject::Update(dt);

		if (GetTickCount() - timer < EXPLOSION_TIME && health_up == true) {
			this->IsDead = true;
		}
		vy += CAR_GRAVITY * dt;

#pragma region reset render
		if (PressKeyUp == false) {
			animation_set->at(CAR_ANI_FLIP_UP_LEFT)->ResetCurrentFrame();
			animation_set->at(CAR_ANI_FLIP_UP_LEFT_2)->ResetCurrentFrame();
			animation_set->at(CAR_ANI_FLIP_UP_LEFT_3)->ResetCurrentFrame();
			animation_set->at(CAR_ANI_FLIP_UP_LEFT_4)->ResetCurrentFrame();
			animation_set->at(CAR_ANI_FLIP_UP_RIGHT)->ResetCurrentFrame();
			animation_set->at(CAR_ANI_FLIP_UP_RIGHT_2)->ResetCurrentFrame();
			animation_set->at(CAR_ANI_FLIP_UP_RIGHT_3)->ResetCurrentFrame();
			animation_set->at(CAR_ANI_FLIP_UP_RIGHT_4)->ResetCurrentFrame();
		}
		/*if (IsJumping == true) {
			IsJumping = false;
		}*/
#pragma endregion

#pragma region Xử lý va chạm (collision)
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		vector<LPGAMEOBJECT>* lsEnermies_Items_Bricks = new vector<LPGAMEOBJECT>();
		vector<LPGAMEOBJECT>* lsElse = new vector<LPGAMEOBJECT>();

		coEvents.clear();
		lsEnermies_Items_Bricks->clear();
		for (int i = 0; i < coObjects->size(); i++) {
			if (dynamic_cast<Item*>(coObjects->at(i))) {
				lsElse->push_back(coObjects->at(i));
			}
			else if (dynamic_cast<Human*>(coObjects->at(i))) {
				lsElse->push_back(coObjects->at(i));
			}
			else {
				lsEnermies_Items_Bricks->push_back(coObjects->at(i));
			}
		}
		CalcPotentialCollisions(lsEnermies_Items_Bricks, coEvents);

		if (GetTickCount() - untouchable_start > CAR_UNTOUCHABLE_TIME)
		{
			untouchable_start = 0;
			untouchable = 0;
		}
		if (IsJumping) {
			state = CAR_STATE_JUMP;
		}
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<CBrick*>(e->obj)) {
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;

					if (e->nx != 0) vx = 0;
					if (e->ny != 0) {
						vy = 0;
						if (e->ny < 0) {
							IsJumping = false;
							PressJump = false;
						}
					}
				}
				else if (dynamic_cast<CPortal*>(e->obj)){
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					CGame::GetInstance()->SwitchScene(p->GetSceneId());
				}
				else if (dynamic_cast<Robot*>(e->obj)) {
					health--;
				}
				else if (dynamic_cast<Bug*>(e->obj)) {
					health--;
				}
				else if (dynamic_cast<MayBug*>(e->obj)) {
					health--;
				}
				else if (dynamic_cast<Spider*>(e->obj)) {
					health--;
				}
				else if (dynamic_cast<Doom*>(e->obj)) {
					health--;
				}
				else if (dynamic_cast<Bee*>(e->obj)) {
					health--;
				}
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
	}
}

void CCar::Render()
{
	int alpha = 255;
	if (untouchable) alpha = 128;
	RenderBoundingBox();
	if (health == 0) {
		health_up = true;
		ani = CAR_ANI_DIE;
		animation_set->at(ani)->Render(x, y, alpha);
		timer = GetTickCount();
	}
	else if (FlippingUp == true) {
		if (vx > 0)
			ani = CAR_ANI_WALKING_UP_RIGHT;
		else if (vx < 0)
			ani = CAR_ANI_WALKING_UP_LEFT;
		else if (vx == 0) {
			if (nx < 0) {
				current_frame = animation_set->at(CAR_ANI_WALKING_UP_LEFT)->GetCurrentFrame();
				switch (current_frame) {
				case CAR_STOP_MOVING_SPRITE1:
					ani = CAR_ANI_FLIP_UP_LEFT_2;
					break;
				case CAR_STOP_MOVING_SPRITE2:
					ani = CAR_ANI_FLIP_UP_LEFT_3;
					break;
				case CAR_STOP_MOVING_SPRITE3:
					ani = CAR_ANI_FLIP_UP_LEFT_4;
					break;
				case CAR_STOP_MOVING_SPRITE4:
					ani = CAR_ANI_FLIP_UP_LEFT;
					break;
				default:
					ani = CAR_ANI_FLIP_UP_LEFT;
					break;
				}
			}
			else {
				current_frame = animation_set->at(CAR_ANI_WALKING_UP_RIGHT)->GetCurrentFrame();
				switch (current_frame) {
				case CAR_STOP_MOVING_SPRITE1:
					ani = CAR_ANI_FLIP_UP_RIGHT_2;
					break;
				case CAR_STOP_MOVING_SPRITE2:
					ani = CAR_ANI_FLIP_UP_RIGHT_3;
					break;
				case CAR_STOP_MOVING_SPRITE3:
					ani = CAR_ANI_FLIP_UP_RIGHT_4;
					break;
				case CAR_STOP_MOVING_SPRITE4:
					ani = CAR_ANI_FLIP_UP_RIGHT;
					break;
				default:
					ani = CAR_ANI_FLIP_UP_RIGHT;
					break;
				}
			}
			animation_set->at(ani)->RenderCarUp(x, y, alpha);
			PressKeyUp = false;
			return;
		}
		animation_set->at(ani)->Render(x, y, alpha);
		return;
	}
	else {
		if (IsJumping == false) {
			if (vx == 0) {
				if (nx > 0) {
					ani = CAR_ANI_WALKING_RIGHT;
					current_frame = animation_set->at(ani)->GetCurrentFrame();
				}
				else {
					ani = CAR_ANI_WALKING_LEFT;
					current_frame = animation_set->at(ani)->GetCurrentFrame();
				}
				animation_set->at(ani)->RenderCurrentFrame(current_frame, x, y, alpha);
				return;
			}
			else if (vx > 0)
				ani = CAR_ANI_WALKING_RIGHT;
			else
				ani = CAR_ANI_WALKING_LEFT;
			animation_set->at(ani)->Render(x, y, alpha);
		}
		else {
			if (nx > 0)
				ani = CAR_ANI_JUMP_RIGHT;
			else
				ani = CAR_ANI_JUMP_LEFT;
			animation_set->at(ani)->RenderCarUp(x, y, alpha);
			return;
		}
	}

}

void CCar::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case CAR_STATE_WALKING_RIGHT:
		vx = CAR_WALKING_SPEED;
		nx = 1;
		break;
	case CAR_STATE_WALKING_LEFT:
		vx = -CAR_WALKING_SPEED;
		nx = -1;
		break;
	case CAR_STATE_JUMP:
		if (IsJumping == true) {
			return;
		}
		else {
			IsJumping = true;
			vy = -CAR_JUMP_SPEED_Y;
		}
		break;
	case CAR_STATE_IDLE:
		vx = 0;
		break;
	case CAR_STATE_DIE:
		vy = -CAR_DIE_DEFLECT_SPEED;
		break;
	case CAR_STATE_WALKING_UP_LEFT:
		vx = -CAR_WALKING_SPEED;
		nx = -1;
		break;
	case CAR_STATE_WALKING_UP_RIGHT:
		vx = CAR_WALKING_SPEED;
		nx = 1;
		break;
	}
}

void CCar::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (FlippingUp == true) {
		right = x + CAR_BBOX_WIDTH;
		bottom = y + CAR_UP_BBOX_HEIGHT;
	}
	else if (PressJump == true) {
		right = x + CAR_BBOX_WIDTH;
		bottom = y + CAR_JUMP_BBOX_HEIGHT;
	}
	else if (IsDead == true) {
		right = x + CAR_DIE_BBOX_WIDTH;
		bottom = y + CAR_DIE_BBOX_HEIGTH;
	}
	else {
		right = x + CAR_BBOX_WIDTH;
		bottom = y + CAR_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CCar::Reset()
{
	SetState(CAR_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

