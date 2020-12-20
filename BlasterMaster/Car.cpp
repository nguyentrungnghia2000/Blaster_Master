#include <algorithm>
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
#include "Portal1.h"
#include "Ladder.h"
#include "Lava.h"
#include "Arrows.h"

CCar::CCar(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(CAR_STATE_IDLE);
	health = power = PLAYER_HEALTH;
	this->IsDead = false;
	this->health_up = false;
	this->isActive = true;
	this->isOverWorld = false;
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
			animation_set->at(CAR_ANI_MOVE_UP_RIGHT_1)->ResetCurrentFrame();
			animation_set->at(CAR_ANI_MOVE_UP_LEFT_1)->ResetCurrentFrame();
		}
		if (IsJumping == false) {
			animation_set->at(CAR_ANI_JUMP_RIGHT)->ResetCurrentFrame();
			animation_set->at(CAR_ANI_JUMP_LEFT)->ResetCurrentFrame();
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
				lsEnermies_Items_Bricks->push_back(coObjects->at(i));
			}
			else if (dynamic_cast<Human*>(coObjects->at(i))) {
				lsElse->push_back(coObjects->at(i));
			}
			else {
				lsEnermies_Items_Bricks->push_back(coObjects->at(i));
			}
		}
		CalcPotentialCollisions(lsEnermies_Items_Bricks, coEvents);

		if (GetTickCount() - untouchable_start > CAR_UNTOUCHABLE_TIME) {
			untouchable_start = 0;
			untouchable = 0;
			isK = false;
		}
		else
		{
			isK = true;
		}

		if (IsJumping) {
			state = CAR_STATE_JUMP;
		}
		if (coEvents.size() == 0) {
			x += dx;
			y += dy;
		}
		else {
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
			for (UINT i = 0; i < coEventsResult.size(); i++) {
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<CBrick*>(e->obj)) {
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.05f;

					if (e->nx != 0) vx = 0;
					if (e->ny != 0) {
						vy = 0;
						if (e->ny < 0) {
							IsJumping = false;
							PressJump = false;
						}
					}
				}
				else if (dynamic_cast<Arrows*>(e->obj)) {
					if (e->nx != 0) vx = 0;
				}
				else if (dynamic_cast<Ladder*>(e->obj)) {
					if (e->nx != 0) x += dx;
					//if (e->ny != 0) y += dy;
				}
				else if (dynamic_cast<CPortal*>(e->obj)) {
					if (e->nx < 0) {
						CPortal* p = dynamic_cast<CPortal*>(e->obj);
						CGame::GetInstance()->SwitchScene(p->GetSceneId());
					}
				}
				else if (dynamic_cast<CPortal1*>(e->obj)) {
					if (e->nx > 0) {
						CPortal1* p = dynamic_cast<CPortal1*>(e->obj);
						CGame::GetInstance()->SwitchScene(p->GetSceneId());
					}
				}
				else if (dynamic_cast<Item*>(e->obj)) {
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
					if (e->obj->Get_ID() == 1)
					{
						if (power < PLAYER_HEALTH)
							power++;
						else
							continue;
					}
					else if (e->obj->Get_ID() == 2)
					{
						if (health < PLAYER_HEALTH)
							health++;
						else
							continue;
					}
					e->obj->Set_IsDead(true);
				}
				else {
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
					if (untouchable == 0) {
						health--;
					}
					StartUntouchable();
				}
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
#pragma endregion
	/*DebugOut(L"x : %f\n", this->x);
	DebugOut(L"y : %f\n", this->y);*/
}

void CCar::Render()
{
	if (isOverWorld == false) {

		int alpha = 255;

		if (isK) {
			alpha = 0 + rand() % (255 + 1 - 0);
		}

		RenderBoundingBox();
		if (health == 0) {
			health_up = true;
			ani = CAR_ANI_DIE;
			animation_set->at(ani)->Render(x, y, alpha);
			timer = GetTickCount();
		}
		else if (FlippingUp == true) {
			if (vx > 0) {
				ani = CAR_ANI_MOVE_UP_RIGHT_1;
				current_frame = animation_set->at(ani)->GetCurrentFrame();
				if (current_frame < 3) {
					animation_set->at(ani)->RenderCarUp(x, y, alpha);
					return;
				}
				else {
					animation_set->at(CAR_ANI_WALKING_UP_RIGHT)->Render(x, y, alpha);
					return;
				}
			}
			else if (vx < 0) {
				ani = CAR_ANI_MOVE_UP_LEFT_1;
				current_frame = animation_set->at(ani)->GetCurrentFrame();
				if (current_frame < 3) {
					animation_set->at(ani)->RenderCarUp(x, y, alpha);
					return;
				}
				else {
					animation_set->at(CAR_ANI_WALKING_UP_LEFT)->Render(x, y, alpha);
					return;
				}

			}
			else if (vx == 0) {
				if (nx < 0) {
					ani = CAR_ANI_WALKING_LEFT;
					current_frame = animation_set->at(ani)->GetCurrentFrame();
					switch (current_frame) {
					case 1:
						ani = CAR_ANI_FLIP_UP_LEFT_2;
						break;
					case 2:
						ani = CAR_ANI_FLIP_UP_LEFT_3;
						break;
					case 3:
						ani = CAR_ANI_FLIP_UP_LEFT_4;
						break;
					case 4:
						ani = CAR_ANI_FLIP_UP_LEFT;
						break;
					}
				}
				else {
					ani = CAR_ANI_WALKING_RIGHT;
					current_frame = animation_set->at(ani)->GetCurrentFrame();
					switch (current_frame) {
					case 1:
						ani = CAR_ANI_FLIP_UP_RIGHT_2;
						break;
					case 2:
						ani = CAR_ANI_FLIP_UP_RIGHT_3;
						break;
					case 3:
						ani = CAR_ANI_FLIP_UP_RIGHT_4;
						break;
					case 4:
						ani = CAR_ANI_FLIP_UP_RIGHT;
						break;
					}
				}
				animation_set->at(ani)->RenderCarUp(x, y, alpha);
				return;
			}
			/*animation_set->at(ani)->RenderCurrentFrame(x, y, alpha);
			return;*/
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
				if (nx > 0) {
					ani = CAR_ANI_JUMP_RIGHT;
					current_frame = animation_set->at(ani)->GetCurrentFrame();
				}
				else {
					ani = CAR_ANI_JUMP_LEFT;
					current_frame = animation_set->at(ani)->GetCurrentFrame();
				}

				animation_set->at(ani)->RenderCarUp(x, y, alpha);
			}
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
	else if (IsJumping == true) {
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

