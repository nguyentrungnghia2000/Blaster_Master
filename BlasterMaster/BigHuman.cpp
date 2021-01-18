#include "BigHuman.h"

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
#include "Ladder.h"

BigHuman::BigHuman(float x, float y)
{
	SetState(BIGHUMAN_STATE_IDLE_X);
	health = power = PLAYER_HEALTH;
	this->IsDead = false;
	this->isActive = true;
	this->isLadder = false;
	this->isMovingonLadder = false;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void BigHuman::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt, colliable_objects);
#pragma region Xử lý va chạm (collision)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* lsEnermies_Items_Bricks = new vector<LPGAMEOBJECT>();
	vector<LPGAMEOBJECT>* lsElse = new vector<LPGAMEOBJECT>();
	if (isLadder == false) {
		isMovingonLadder = false;
	}
	if (isLadder == true) {
		if (nx != 0) {
			if (nx > 0)
				vx = BIGHUMAN_WALKING_SPEED;
			else if (nx < 0)
				vx = -BIGHUMAN_WALKING_SPEED;
		}
		else if (ny != 0) {
			if (ny > 0)
				vy = -BIGHUMAN_WALKING_SPEED;
			else if (ny < 0)
				vy = BIGHUMAN_WALKING_SPEED;
		}
	}

	coEvents.clear();
	lsEnermies_Items_Bricks->clear();
	for (int i = 0; i < colliable_objects->size(); i++) {
			lsEnermies_Items_Bricks->push_back(colliable_objects->at(i));
	}
	/*DebugOut(L"ladder : %d\n", isLadder);
	DebugOut(L"moving on ladder : %d\n", isMovingonLadder);*/
	for (UINT i = 0; i < lsEnermies_Items_Bricks->size(); i++) {
		if (dynamic_cast<Ladder*>(lsEnermies_Items_Bricks->at(i))) {
			if (this->IsColidingAABB(lsEnermies_Items_Bricks->at(i))) {
				isLadder = true;
				break;
			}
			else {
				isLadder = false;
				isMovingonLadder = false;
			}
		}
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
				if (isLadder == false) {
					if (e->nx != 0) vx = 0;
					if (e->ny != 0) vy = 0;
				}
				else {
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
			}
			else if (dynamic_cast<Ladder*>(e->obj)) {
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else {
				health--;
			}
			
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
	/*DebugOut(L"moving on ladder : %d\n", isMovingonLadder);
	DebugOut(L"ladder : %d\n", isLadder);*/
	//DebugOut(L"x : %f\n", x);
	//DebugOut(L"y : %f\n", y);
}

void BigHuman::Render()
{
	RenderBoundingBox();
	if (isLadder == false) {
		if (isActive == true) {
			if (state == BIGHUMAN_STATE_MOVE_UP)
				ani = BIGHUMAN_ANI_MOVE_UP;
			else if (state == BIGHUMAN_STATE_MOVE_DOWN)
				ani = BIGHUMAN_ANI_MOVE_DOWN;
			else if (state == BIGHUMAN_STATE_MOVE_RIGHT)
				ani = BIGHUMAN_ANI_MOVE_RIGHT;
			else if (state == BIGHUMAN_STATE_MOVE_LEFT)
				ani = BIGHUMAN_ANI_MOVE_LEFT;
			else if (state == BIGHUMAN_STATE_IDLE_Y) {
				if (ny > 0)
					ani = BIGHUMAN_ANI_IDLE_UP;
				else
					ani = BIGHUMAN_ANI_IDLE_DOWN;
			}
			else if (state == BIGHUMAN_STATE_IDLE_X) {
				if (nx > 0)
					ani = BIGHUMAN_ANI_IDLE_RIGHT;
				else
					ani = BIGHUMAN_ANI_IDLE_LEFT;
			}
			animation_set->at(ani)->Render(x, y, alpha);
		}
	}
	
}

void BigHuman::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state == BIGHUMAN_STATE_IDLE_Y || state == BIGHUMAN_STATE_MOVE_DOWN || state == BIGHUMAN_STATE_MOVE_UP) {
		right = x + BIGHUMAN_BBOX_Y_WIDTH;
		bottom = y + BIGHUMAN_BBOX_Y_HEIGHT;
	}
	else if (state == BIGHUMAN_STATE_IDLE_X || state == BIGHUMAN_STATE_MOVE_RIGHT || state == BIGHUMAN_STATE_MOVE_LEFT) {
		right = x + BIGHUMAN_BBOX_X_WIDTH;
		bottom = y + BIGHUMAN_BBOX_X_HEIGHT;
	}
}

void BigHuman::Reset()
{
	SetState(BIGHUMAN_STATE_IDLE_X);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void BigHuman::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state) {
	case BIGHUMAN_STATE_MOVE_UP:
		vy = -BIGHUMAN_WALKING_SPEED;
		nx = 0;
		ny = 1;
		break;
	case BIGHUMAN_STATE_MOVE_DOWN:
		vy = BIGHUMAN_WALKING_SPEED;
		nx = 0;
		ny = -1;
		break;
	case BIGHUMAN_STATE_MOVE_RIGHT:
		vx = BIGHUMAN_WALKING_SPEED;
		ny = 0;
		nx = 1;
		break;
	case BIGHUMAN_STATE_MOVE_LEFT:
		vx = -BIGHUMAN_WALKING_SPEED;
		ny = 0;
		nx = -1;
		break;
	case BIGHUMAN_STATE_IDLE_X:
	case BIGHUMAN_STATE_IDLE_Y:
		vx = 0;
		vy = 0;
		break;
	}
}
