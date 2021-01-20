#include "Spider.h"
#include <random>

void Spider::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SPIDER_BBOX_WIDTH;
	bottom = y + SPIDER_BBOX_HEIGHT;
}

void Spider::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	srand(time(NULL));
	//DebugOut(L"%d",randomX);
	CGameObject::Update(dt, coObjects);
	if (this->EnermiesHealth == 0) 
	{
		this->IsDead = true;
	}
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
	CalcPotentialCollisions(OnlyBrick, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (time_attack->GetStartTime() == 0)
		{
			time_attack->Start();
		}
		else if (time_attack->IsTimeUp())
		{
			IsAttack = true;
			if (IsAttack)
			{
				//SpiderBullets* Bullet = new SpiderBullets(this->x, this->y, 0);
				//Bullet->Update(dt,coObjects);
				Bullet.push_back(new SpiderBullets(this->x, this->y, 0.05f, this->Target));
			}
			IsAttack = false;
			time_attack->Reset();
		}
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

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (e->ny < 0)
					vy = -vy;
				if (e->ny > 0)
					vy = -vy;
				if (nx != 0 && ny == 0)
					vx = -vx;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Spider::Render()
{
	int ani = SPIDER_ANI_WALKING_LEFT;
	if (vx > 0)
		ani = SPIDER_ANI_WALKING_RIGHT;
	else
		ani = SPIDER_ANI_WALKING_LEFT;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
	for (int i = 0; i < Bullet.size(); i++)
		Bullet.at(i)->Render();
}

Spider::Spider(LPGAMEOBJECT target)
{
	this->Target = target;
	this->EnermiesHealth = ENERMIES_HEALTH;
	this->time_attack = new Timer(SPIDER_ATTACK_TIME);
	this->time_dis = new Timer(SPIDER_BULLET_DIS);
	IsAttack = true;
	random_device m;
	mt19937 t(m());
	uniform_real_distribution<float> minh(-.05f, .05f);

	vx = minh(t);
	vy = sqrt(0.005f - pow(vx, 2)) * (minh(t) < 0 ? 1 : -1);
	SetState(SPIDER_STATE_WALKING);
}

void Spider::SetState(int state)
{
	switch (state)
	{
	case SPIDER_STATE_WALKING:
		vx = vx;
		vy = vy;
		break;
	}
}
