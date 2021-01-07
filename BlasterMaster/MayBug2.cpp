#pragma once
#include "MayBug2.h"
#include "Utils.h"

void MayBug2::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MAYBUG_BBOX_WIDTH;
	bottom = y + MAYBUG_BBOX_HEIGHT;
}

void MayBug2::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* OnlyBrick = new vector<LPGAMEOBJECT>();
	OnlyBrick->clear();
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i)))
		{
			OnlyBrick->push_back(coObjects->at(i));
		}
	}
	CalcPotentialCollisions(OnlyBrick, coEvents);
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

		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (e->nx != 0 && ny == 0)
				{
					vx = 0;
					vy = 0;
				}
				if (e->ny != 0)
				{
					vx = 0;
					vy = 0;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void MayBug2::Render()
{
	int ani = MAYBUG_ANI_ROLLING;
	animation_set->at(ani)->Render(x, y);
}

MayBug2::MayBug2(LPGAMEOBJECT target, int x, int y)
{
	this->Target = target;
	vy = MAYBUG_SPEED_Y;
	vx = CountVx(target, x, y);
	DebugOut(L"%d", x);
}

void MayBug2::SetState(int state)
{
	switch (state)
	{
	case MAYBUG2_STATE_ATTACK:
		break;
	}
}

float MayBug2::CountVx(LPGAMEOBJECT target, int posx, int posy)
{
	DebugOut(L"%d", Target->x);
	float disX = posx - target->x;
	float disY = posy - target->y;
	//float dis = (float)sqrt(pow(disX,2)+pow(disY,2));
	vx = (float)vy * disX / disY;
	return this->vx;
}
