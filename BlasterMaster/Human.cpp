#include "Human.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Car.h"
#include "Brick.h"
#include "Game.h"
#include "Item.h"
#include "Bug.h"
#include "Robot.h"
#include "Bee.h"
#include "MayBug.h"
#include "Doom.h"
#include "Spider.h"
#include "Portal.h"

Human::Human(float x, float y)
{
	SetState(HUMAN_STATE_IDLE);
	health = power = PLAYER_HEALTH;
	this->IsDead = false;
	this->isActive = false;
	this->isLying = false;
	this->isOverWorld = false;
	this->PressDown = false;
	this->PressUp = false;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void Human::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt, colliable_objects);
	vy += HUMAN_GRAVITY * dt;

	if (IsJumping == true) {
		state = HUMAN_STATE_JUMP;
	}
	if (PressDown == true)
		isLying = true;
	if (isActive == false)
		isCollisionWithCar = false;

#pragma region Xử lý va chạm (collision)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* lsEnermies_Items_Bricks = new vector<LPGAMEOBJECT>();
	vector<LPGAMEOBJECT>* lsElse = new vector<LPGAMEOBJECT>();

	coEvents.clear();
	lsEnermies_Items_Bricks->clear();
	for (int i = 0; i < colliable_objects->size(); i++) {
		if (dynamic_cast<Item*>(colliable_objects->at(i))) {
			lsEnermies_Items_Bricks->push_back(colliable_objects->at(i));
		}
		else if (dynamic_cast<CCar*>(colliable_objects->at(i))) {
			lsEnermies_Items_Bricks->push_back(colliable_objects->at(i));
		}
		else {
			lsEnermies_Items_Bricks->push_back(colliable_objects->at(i));
		}
		//lsEnermies_Items_Bricks->push_back(colliable_objects->at(i));
	}
	CalcPotentialCollisions(lsEnermies_Items_Bricks, coEvents);

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
			else if (dynamic_cast<CCar*>(e->obj)) {
				isCollisionWithCar = true;
				if (e->nx != 0) vx += dx;
				if (e->ny != 0) vy = 0;
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (dynamic_cast<Robot*>(e->obj)) {
				health--;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void Human::Render()
{
	if (isOverWorld == false) {
		if (isActive == true) {
			alpha = 255;
			if (untouchable) alpha = 128;
			//RenderBoundingBox();
			if (isLying == true) {

				if (vx > 0) {
					ani = HUMAN_ANI_LIE_MOVE_RIGHT;
				}
				else if (vx < 0)
					ani = HUMAN_ANI_LIE_MOVE_LEFT;
				else {
					if (nx > 0)
						ani = HUMAN_ANI_IDLE_LIE_RIGHT;
					else
						ani = HUMAN_ANI_IDLE_LIE_LEFT;
				}
			}
			else {
				if (IsJumping == true) {
					if (nx > 0)
						ani = HUMAN_ANI_JUMP_RIGHT;
					else
						ani = HUMAN_ANI_JUMP_LEFT;
				}
				else {
					if (state == HUMAN_STATE_WALKING_RIGHT)
						ani = HUMAN_ANI_WALK_RIGHT;
					else if (state == HUMAN_STATE_WALKING_LEFT)
						ani = HUMAN_ANI_WALK_LEFT;
					else if (state == HUMAN_STATE_IDLE) {
						if (nx > 0)
							ani = HUMAN_ANI_IDLE_RIGHT;
						else
							ani = HUMAN_ANI_IDLE_LEFT;
					}
				}
			}


			animation_set->at(ani)->Render(x, y, alpha);
		}
	}
}

void Human::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (isLying == true) {
		right = x + HUMAN_LIE_BBOX_WIDTH;
		bottom = y + HUMAN_LIE_BBOX_HEIGHT;
	}
	else {
		right = x + HUMAN_BBOX_WIDTH;
		bottom = y + HUMAN_BBOX_HEIGHT;
	}
}

void Human::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case HUMAN_STATE_WALKING_LEFT:
		vx = -HUMAN_WALKING_SPEED;
		nx = -1;
		break;
	case HUMAN_STATE_WALKING_RIGHT:
		vx = HUMAN_WALKING_SPEED;
		nx = 1;
		break;
	case HUMAN_STATE_JUMP:
		if (IsJumping == true)
			return;
		else {
			IsJumping = false;
			vy = -HUMAN_JUMP_SPEED_Y;
		}
		break;
	case HUMAN_STATE_LIE:
	case HUMAN_STATE_IDLE:
		vx = 0;
		break;
	case HUMAN_STATE_LIE_MOVE_RIGHT:
		vx = HUMAN_LYING_SPEED;
		nx = 1;
		break;
	case HUMAN_STATE_LIE_MOVE_LEFT:
		vx = -HUMAN_LYING_SPEED;
		nx = -1;
		break;
	}
}

void Human::Reset()
{
	SetState(HUMAN_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}
