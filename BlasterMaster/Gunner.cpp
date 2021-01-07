#include "Gunner.h"

void Gunner::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GUNNER_BBOX;
	bottom = y + GUNNER_BBOX;
}

void Gunner::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (type == 3)
	{
		type = 1;
	}
	DebugOut(L"\nLoai dan:%d", type);
	if (Time_Delay->GetStartTime() == 0)
	{
		Time_Delay->Start();
	}
	else
	{
		if (Time_Delay->IsTimeUp())
		{
			if (type % 2 == 0)
			{
				Bullet.push_back(new GunnerBullets(this->x + GUNNER_BBOX / 2 - GUNNER_BULLET_BBOX / 2, this->y, 1));
				Bullet.push_back(new GunnerBullets(this->x + GUNNER_BBOX / 2 - GUNNER_BULLET_BBOX / 2, this->y + GUNNER_BBOX, 2));
			}
			else
			{
				Bullet.push_back(new GunnerBullets(this->x, this->y + GUNNER_BBOX / 2 - GUNNER_BULLET_BBOX / 2, 3));
				Bullet.push_back(new GunnerBullets(this->x + GUNNER_BBOX / 2, this->y + GUNNER_BBOX / 2 - GUNNER_BULLET_BBOX / 2, 4));
			}
			Time_Delay->Reset();
			type++;
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
}

void Gunner::Render()
{
	int ani = GUNNER_ANI_ATTACK_TD;
	if (type % 2 == 0)
	{
		ani = GUNNER_ANI_ATTACK_LR;
	}
	else
	{
		ani = GUNNER_ANI_ATTACK_TD;
	}
	animation_set->at(ani)->Render(x, y);
	for (int i = 0; i < Bullet.size(); i++)
		Bullet.at(i)->Render();
	RenderBoundingBox();
}

Gunner::Gunner()
{
	type = 1;
	SetState(GUNNER_STATE_NOLMAL);
	this->Time_Delay = new Timer(DELAY_TIME);
	this->time_dis = new Timer(GUNNER_BULLET_DIS);
	Time_Delay->Start();
}

void Gunner::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GUNNER_STATE_NOLMAL:
		break;
	case GUNNER_STATE_ATTACK:
		break;
	}
}
