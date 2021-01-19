#include "Tele.h"
#include "Game.h"
#include <random>

void Tele::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + TELE_BBOX_WIDTH;
	bottom = y + TELE_BBOX_HEIGHT;
}

void Tele::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* GameCamera = CGame::GetInstance();
	if (this->EnermiesHealth == 0) {
		this->IsDead = true;
	}
	if (Time_undead->GetStartTime() == 0)
	{
		Time_undead->Start();
		this->SetState(TELE_STATE_UNDEAD);
	}
	else if (Time_undead->IsTimeUp())
	{
		this->SetState(TELE_STATE_TELE);
		if (Time_tele->GetStartTime() == 0)
		{
			Time_tele->Start();
		}
		if (Time_tele->IsTimeUp())
		{
			Time_undead->Reset();
			Time_tele->Reset();
		}
		else
		{
			if (Time_delay_tele->GetStartTime() == 0)
			{
				Time_delay_tele->Start();
			}
			else if (Time_delay_tele->IsTimeUp())
			{
				random_device m;
				mt19937 t(m());
				uniform_real_distribution<float> randomX(MinX, MinX+TELE_WIDTH - TELE_BBOX_WIDTH);
				uniform_real_distribution<float> randomY(MinY, MinY + TELE_HEIGHT - TELE_BBOX_HEIGHT);
				this->x = randomX(t);
				this->y = randomY(t);
				Time_delay_tele->Reset();
			}
		}
	}
	Attack(dt, coObjects);
}

void Tele::Render()
{
	int ani = TELE_ANI_UNDEAD;
	if (state == TELE_STATE_TELE)
	{
		ani = TELE_ANI_TELE;
	}
	else if (state == TELE_STATE_UNDEAD)
	{
		ani = TELE_ANI_UNDEAD;
	}
	animation_set->at(ani)->Render(x, y);
	for (int i = 0; i < Bullet.size(); i++)
		Bullet.at(i)->Render();
}

Tele::Tele(LPGAMEOBJECT target,int minx,int miny)
{
	this->SetState(TELE_STATE_TELE);
	Time_delay_tele = new Timer(TIME_DELAY_TELE);
	Time_tele = new Timer(TIME_TELE);
	Time_undead = new Timer(TIME_UNDEAD);
	Time_dis = new Timer(TELE_TIME_DIS);
	Delay_time = new Timer(TELE_DELAY_ATTACK);
	this->EnermiesHealth = ENERMIES_HEALTH;
	this->Target = target;
	MinX = minx;
	MinY = miny;
}

void Tele::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TELE_STATE_TELE:
		break;
	case TELE_STATE_UNDEAD:
		break;
	}
}

void Tele::Attack(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Delay_time->GetStartTime() == 0)
	{
		Delay_time->Start();
	}
	else if (Delay_time->IsTimeUp())
	{
		Bullet.push_back(new EyeballBullets(Target, this->x, this->y));
		Delay_time->Reset();
	}
	for (int i = 0; i < Bullet.size(); i++)
	{
		Bullet.at(i)->Update(dt, coObjects);
	}
	for (int i = 0; i < Bullet.size(); i++)
	{
		if (Bullet.at(i)->Get_IsCollision() == true)
		{
			if (Time_dis->GetStartTime() == 0)
				Time_dis->Start();
			if (Time_dis->IsTimeUp())
			{
				Bullet.erase(Bullet.begin() + i);
				Time_dis->Reset();
			}
		}
	}
}
