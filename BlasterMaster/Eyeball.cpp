#include "Eyeball.h"
#include <random>
#include "Game.h"

void Eyeball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + EYEBALL_BBOX;
	bottom = y + EYEBALL_BBOX;
}

void Eyeball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (type == 1)
	{
		Update_Type_1(dt, coObjects);
	}
	else if (type == 2)
	{
		Update_Type_2(dt, coObjects);
	}
	else if (type == 3)
	{
		Update_Type_3(dt, coObjects);
	}
	//CGame* GameCamera = CGame::GetInstance();
	//DebugOut(L" %d", GameCamera->GetX());
	//GameCamera->GetX();
}

void Eyeball::Render()
{
	int ani = EYEBALL_NORMAL_ANI;
	animation_set->at(ani)->Render(x, y);
	for (int i = 0; i < Bullet.size(); i++)
		Bullet.at(i)->Render();
}

Eyeball::Eyeball(LPGAMEOBJECT target, int x, int y)
{
	this->Target = target;
	this->x = x;
	this->y = y;
	random_device rm;
	mt19937 t(rm());
	uniform_int_distribution<int>	randi(1, 3);
	type = randi(t);
	//type = 2;
	if (type == 1)
	{
		time_hunting_type1 = new Timer(TIME_HUNTING);
		time_delay_type1 = new Timer(TIME_DELAY_HUNTING);
		this->SetState(EYEBALL_STATE_NORMAL_TYPE1);
	}
	else if (type == 2)
	{
		random_device rm;
		mt19937 t(rm());
		uniform_int_distribution<int>	randi(1, 3);
		type_2 = randi(t);
		//type_2 = 1;
		time_delay_type2 = new Timer(TIME_DELAY_JUMP);
		this->SetState(EYEBALL_STATE_NORMAL_TYPE2);
	}
	else
	{
		random_device rm;
		mt19937 t(rm());
		uniform_int_distribution<int>	randi(1, 3);
		type_3 = randi(t);
		//type_3 = 2;
		time_delay_type3 = new Timer(TIME_DELAY_JUMP);
		this->SetState(EYEBALL_STATE_NORMAL_TYPE3);
	}
	Delay_time = new Timer(EYEBALL_DELAY_ATTACK);
	Time_dis = new Timer(TIME_DIS);
}

void Eyeball::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case EYEBALL_STATE_NORMAL_TYPE1:
		vy = EYEBALL_SPEED_Y_TYPE1;
		vx = CountVx(Target, this->x, this->y);
		break;
	case EYEBALL_STATE_NORMAL_TYPE2:
		break;
	case EYEBALL_STATE_NORMAL_TYPE3:
		if (type_3 == 1)
		{
			vx = -EYEBALL_SPEED_TYPE3;
		}
		else if (type_3 == 2)
		{
			vy = -EYEBALL_SPEED_TYPE3;
		}
		else
		{
			vy = EYEBALL_SPEED_TYPE3;
		}
		//vx = -EYEBALL_SPEED_X_TYPE3;
	case EYEBALL_STATE_ATTACK:
		break;
	}
}
float Eyeball::CountVx(LPGAMEOBJECT target, int x, int y)
{
	float disX = x - target->x;
	float disY = y - target->y;
	//float dis = (float)sqrt(pow(disX,2)+pow(disY,2));
	vx = (float)vy * disX / disY;
	return this->vx;
}
void Eyeball::Update_Type_1(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
		if (time_hunting_type1->GetStartTime() == 0)
		{
			time_hunting_type1->Start();
			this->SetState(EYEBALL_STATE_NORMAL_TYPE1);
		}
		else if (time_hunting_type1->IsTimeUp())
		{
			this->SetState(EYEBALL_STATE_NORMAL_TYPE1);
			vx = vy = 0;
			if (time_delay_type1->GetStartTime() == 0)
			{
				time_delay_type1->Start();
			}
			if (time_delay_type1->IsTimeUp())
			{
				time_hunting_type1->Reset();
			}
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny < 0)
					vy = -vy;
				if (e->ny > 0)
					vy = -vy;
				if (nx != 0 && ny == 0)
					vx = -vx;
				//vx = vy = 0;
			}
		}
	}
	//DebugOut(L"%d", time_delay_type1->IsTimeUp());
	Attack(dt, coObjects);
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Eyeball::Update_Type_2(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* OnlyBrick = new vector<LPGAMEOBJECT>();
	OnlyBrick->clear();
	switch (type_2)
	{
	case 1:
		vy -= EYEBALL_GRAVITY_TYPE2 * dt;
		break;
	case 2:
		vx -= EYEBALL_GRAVITY_TYPE2 * dt;
		break;
	case 3:
		vx += EYEBALL_GRAVITY_TYPE2 * dt;
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

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{

				if (time_delay_type2->GetStartTime() == 0)
				{
					time_delay_type2->Start();
				}
				else if (time_delay_type2->IsTimeUp())
				{
					switch (type_2)
					{
					case 1:
						vy = EYEBALL_JUMP_TYPE2;
						//DebugOut(L"true");
						break;
					case 2:
						vx = EYEBALL_JUMP_TYPE2;
						break;
					case 3:
						vx = -EYEBALL_JUMP_TYPE2;
						break;
					}
					time_delay_type2->Reset();
				}
			}
		}
	}
	Attack(dt, coObjects);
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Eyeball::Update_Type_3(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* OnlyBrick = new vector<LPGAMEOBJECT>();
	OnlyBrick->clear();
	switch (type_3)
	{
	case 1:
		vy -= EYEBALL_GRAVITY_TYPE3 * dt;
		break;
	case 2:
		vx -= EYEBALL_GRAVITY_TYPE3 * dt;
		break;
	case 3:
		vx += EYEBALL_GRAVITY_TYPE3 * dt;
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

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (time_delay_type3->GetStartTime() == 0)
				{
					time_delay_type3->Start();
					vx = vy = 0;
				}
				else if (time_delay_type3->IsTimeUp())
				{
					this->SetState(EYEBALL_STATE_NORMAL_TYPE3);
					if (type_3 == 1)
					{
						vy = EYEBALL_JUMP_TYPE3;
					}
					else if (type_3 == 2)
					{
						vx = EYEBALL_JUMP_TYPE3;
					}
					else
					{
						vx = -EYEBALL_JUMP_TYPE3;
					}
					count_jump++;
				}
				if (e->ny != 0)
				{
					if (type_3 == 2 || type_3 == 3)
					{
						vy = -vy;
					}
				}
				//if (e->ny > 0)
				//	vy = -vy;
				if (nx != 0 && ny == 0)
				{
					if (type_3 == 1)
					{
						vx = -vx;
					}
				}
				if (count_jump == 2)
				{
					time_delay_type3->Reset();
					count_jump = 0;
				}
				//DebugOut(L"%d",count_jump);
			}
		}
	}
	Attack(dt, coObjects);
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Eyeball::Attack(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

