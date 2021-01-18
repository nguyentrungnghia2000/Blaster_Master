#include "Boss.h"
#include <random>
#include "Game.h"


void Boss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOSS_BBOX_WIDTH;
	bottom = y + BOSS_BBOX_HEIGHT;
}

void Boss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* GameCamera = CGame::GetInstance();
	if (this->Time_move->GetStartTime() == 0)
	{
		Time_move->Start();
	}
	else if(this->Time_move->IsTimeUp())
	{
		random_device rm;
		mt19937 t(rm());
		uniform_int_distribution<int>	randi(1, 4);
		where = randi(t);
		Time_move->Reset();
		this->SetState(BOSS_STATE_MOVE);
	}
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	RightHand->Set_FollowBossX(this->dx);
	RightHand->Set_FollowBossY(this->dy);
	LeftHand->Set_FollowBossX(this->dx);
	LeftHand->Set_FollowBossY(this->dy);
	RightHand->Update(dt, coObjects);
	LeftHand->Update(dt, coObjects);
	for (int i = 0; i < Flange.size(); i++)
	{
		Flange.at(i)->Set_FollowBossX(this->dx);
		Flange.at(i)->Set_FollowBossY(this->dy);
		Flange2.at(i)->Set_FollowBossX(this->dx);
		Flange2.at(i)->Set_FollowBossY(this->dy);
		Flange.at(i)->Update_Right(dt,coObjects);
		Flange2.at(i)->Update_Left(dt, coObjects);
	}
	if ((this->x < GameCamera->GetCamx())|| (this->x + BOSS_BBOX_WIDTH > GameCamera->GetCamx() + SCREEN_WIDTH))
	{
		vx = -vx;
	}
	if ((this->y + BOSS_BBOX_HEIGHT > GameCamera->GetCamy() + SCREEN_HEIGHT / 2) || (this->y < GameCamera->GetCamy()))
	{
		vy = -vy;
	}
	Attack(dt,coObjects);
	//DebugOut(L"%d\nVan toc nek:", vx);
}

void Boss::Render()
{
	int ani = 0;
	animation_set->at(ani)->Render(x, y);
	RightHand->Render();
	LeftHand->Render();
	for (int i = 0; i < Flange.size(); i++)
	{
		Flange.at(i)->Render();
		Flange2.at(i)->Render();
	}
	for (int i = 0; i < Bullets.size(); i++)
		Bullets.at(i)->Render();
}

Boss::Boss(LPGAMEOBJECT target,int posX,int posY)
{
	this->x = posX;
	this->y = posY;
	this->Time_delay_attack = new Timer(TIME_DELAY_ATTACK);
	RightHand = new BossHand(this->x + BOSS_BBOX_WIDTH - BOSSHAND_BBOX_WIDTH / 2, this->y + FLANGEHAND_BBOX_HEIGHT, 1);
	LeftHand = new BossHand(this->x - BOSSHAND_BBOX_WIDTH/2, this->y + FLANGEHAND_BBOX_HEIGHT, 2);
	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			Flange.push_back(new FlangeHand(RightHand, this->x + BOSS_BBOX_WIDTH , this->y+FLANGEHAND_BBOX_HEIGHT, i + 1));
			Flange2.push_back(new FlangeHand(LeftHand, this->x - BOSSHAND_BBOX_WIDTH/2, this->y + FLANGEHAND_BBOX_HEIGHT, i + 1));
		}
		else
		{
			Flange.push_back(new FlangeHand(Flange.at(i - 1), this->x + BOSS_BBOX_WIDTH, this->y + FLANGEHAND_BBOX_HEIGHT, i + 1));
			Flange2.push_back(new FlangeHand(Flange2.at(i - 1), this->x - BOSSHAND_BBOX_WIDTH/2, this->y + FLANGEHAND_BBOX_HEIGHT, i + 1));
		}
	}
	this->Time_move = new Timer(BOSS_TIME_MOVE);
	where = 1;
	this->Target = target;
	this->SetState(BOSS_STATE_MOVE);
}

void Boss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOSS_STATE_MOVE:
		switch (where)
		{
		case 1:
			vx = BOSS_SPEED;
			vy = BOSS_SPEED;
			break;
		case 2:
			vx = BOSS_SPEED;
			vy = -BOSS_SPEED;
			break;
		case 3:
			vx = -BOSS_SPEED;
			vy = BOSS_SPEED;
			break;
		case 4:
			vx = -BOSS_SPEED;
			vy = -BOSS_SPEED;
			break;
		}
		break;
		//vx = vy = 0;
	}
}

void Boss::Attack(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Time_delay_attack->GetStartTime() == 0)
	{
		Time_delay_attack->Start();
	}
	else if (Time_delay_attack->IsTimeUp())
	{
		Bullets.push_back(new BossBullets(this->x, this->y,0.05f,this->Target));
		Time_delay_attack->Reset();
	}
	for (int i = 0; i < Bullets.size(); i++)
	{
		Bullets.at(i)->Update(dt, coObjects);
	}
	for (int i = 0; i < Bullets.size(); i++)
	{
		if (Bullets.at(i)->Get_IsCollision() == true)
		{
			Bullets.erase(Bullets.begin() + i);
		}
	}
}
