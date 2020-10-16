#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Car.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CCar::CCar(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(CAR_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CCar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += CAR_GRAVITY * dt;

#pragma region reset render car up
	if (PressKeyUp == false) {
		/*animation_set->at(CAR_ANI_IDLE_RIGHT_1)->ResetCurrentFrame();
		animation_set->at(CAR_ANI_IDLE_RIGHT_2)->ResetCurrentFrame();
		animation_set->at(CAR_ANI_IDLE_RIGHT_3)->ResetCurrentFrame();
		animation_set->at(CAR_ANI_IDLE_RIGHT_4)->ResetCurrentFrame();
		animation_set->at(CAR_ANI_IDLE_LEFT_1)->ResetCurrentFrame();
		animation_set->at(CAR_ANI_IDLE_LEFT_2)->ResetCurrentFrame();
		animation_set->at(CAR_ANI_IDLE_LEFT_3)->ResetCurrentFrame();
		animation_set->at(CAR_ANI_IDLE_LEFT_4)->ResetCurrentFrame();*/
		animation_set->at(CAR_ANI_UP_LEFT)->ResetCurrentFrame();
		animation_set->at(CAR_ANI_UP_RIGHT)->ResetCurrentFrame();
	}
#pragma endregion

#pragma region Xử lý va chạm (collision)
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	//if (state != CAR_STATE_IDLE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > CAR_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	//swept aabb      aabb
	// No collision occured, proceed normally
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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -CAR_DIE_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							SetState(CAR_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
}

void CCar::Render()
{
	int ani = -1;
	

	int alpha = 255;
	if (untouchable) alpha = 128;
	//RenderBoundingBox();
	if (state == CAR_STATE_DIE)
		ani = CAR_ANI_DIE;
	else
	{
		switch (state) {
		case CAR_STATE_WALKING_RIGHT:
			flag = true;
			ani = CAR_ANI_WALKING_RIGHT;
			animation_set->at(ani)->Render(x, y, alpha);
			break;
		case CAR_STATE_WALKING_LEFT:
			flag = true;
			ani = CAR_ANI_WALKING_LEFT;
			animation_set->at(ani)->Render(x, y, alpha);
			break;
		case CAR_STATE_UP:
			if (vx == 0) {
				if (nx < 0) {
					ani = CAR_ANI_UP_LEFT;
				}
				else {
					ani = CAR_ANI_UP_RIGHT;
				}
				animation_set->at(ani)->RenderCarUp(x, y, alpha);
				PressKeyUp = false;
			}
			else {
				if (vx < 0) {
					ani = CAR_ANI_WALKING_UP_LEFT;
				}
				else if (vx > 0) {
					ani = CAR_ANI_WALKING_UP_RIGHT;
				}
				animation_set->at(ani)->Render(x, y, alpha);
				PressKeyUp = false;
			}
			
		break;
		default:
			PressKeyUp = false;
			if (nx > 0) {
				ani = CAR_ANI_WALKING_RIGHT;
				current_frame = animation_set->at(ani)->GetCurrentFrame();
			}
			else if (nx < 0){
				ani = CAR_ANI_WALKING_LEFT;
				current_frame = animation_set->at(CAR_ANI_WALKING_LEFT)->GetCurrentFrame();
			}
			animation_set->at(ani)->RenderCurrentFrame(current_frame , x, y, alpha);
			break;
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
		DebugOut(L"vx : %d\n", vx);
		nx = 1;
		break;
	case CAR_STATE_WALKING_LEFT:
		vx = -CAR_WALKING_SPEED;
		DebugOut(L"vx : %d\n", vx);
		nx = -1;
		break;
	case CAR_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		//IsJump == true;
		//if (IsJump == true) {
			DebugOut(L"IsJump car : %d\n", IsJump);
			vy = -CAR_JUMP_SPEED_Y;
			IsJump = false;
		//}
		//else {
			//return;
		//}
		break;
	case CAR_STATE_IDLE:
		IsJump = false;
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
	right = x + CAR_BBOX_WIDTH;
	bottom = y + CAR_BBOX_HEIGHT;

	if (state == CAR_STATE_UP) {

		right = x + CAR_UP_BBOX_WIDTH;
		bottom = y + CAR_UP_BBOX_HEIGHT;
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

