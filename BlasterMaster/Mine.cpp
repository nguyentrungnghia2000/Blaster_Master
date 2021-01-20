#include "Mine.h"
#include <random>

void Mine::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state == MINE_STATE_NORMAL)
	{
		right = x + MINE_BBOX_WIDTH;
		bottom = y + MINE_BBOX_HEIGHT;
	}
	else
	{
		right = x;
		bottom = y;
	}
}

void Mine::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (this->EnermiesHealth == 0) {
		this->IsDead = true;
	}
	vy += BEE_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* OnlyBrick = new vector<LPGAMEOBJECT>();
	OnlyBrick->clear();
	if (this->EnermiesHealth == 0) {
		this->IsDead = true;
	}
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
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

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
				//DebugOut(L"true");
			}
		}
	}
	if (IsDead == true)
	{
		random_device rm;
		mt19937 t(rm());
		uniform_int_distribution<int>	randi(2, 3);
		int random = randi(t);
		for (int i = 1; i <= random; i++)
		{
			this->Bullet.push_back(new MineBullets(this->x + MINE_BBOX_WIDTH / 2, this->y));
		}
	}
	for (int i = 0; i < Bullet.size(); i++)
	{
		Bullet.at(i)->Update(dt, coObjects);
	}
	for (int i = 0; i < Bullet.size(); i++)
	{
		if (Bullet.at(i)->Get_IsCollision() == true)
		{
			if (time_dis->GetStartTime() == 0)
				time_dis->Start();
			if (time_dis->IsTimeUp())
			{
				Bullet.erase(Bullet.begin() + i);
				time_dis->Reset();
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Mine::Render()
{
	int ani = MINE_NORMAL_ANI;
	if (state == MINE_BOOM)
	{
		ani = MINE_BOOM_ANI;
	}
	else
	{
		ani = MINE_NORMAL_ANI;
	}
	animation_set->at(ani)->Render(x, y);
	for (int i = 0; i < Bullet.size(); i++)
		Bullet.at(i)->Render();
	//RenderBoundingBox();
}

Mine::Mine()
{
	this->EnermiesHealth = ENERMIES_HEALTH;
	IsBoom = false;
	this->SetState(MINE_STATE_NORMAL);
	this->time_dis = new Timer(TIME_DIS);
	this->EnermiesHealth = ENERMIES_HEALTH;
}

void Mine::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MINE_STATE_NORMAL:
		break;
	case MINE_BOOM:
		break;
	}
}
