#include "BossBullets.h"

BossBullets::BossBullets(int PosX, int PosY, float speed, LPGAMEOBJECT Target)
{
	ani = CAnimations::GetInstance()->Get(BOSS_BULLET_ANI);
	//ani_ex = CAnimations::GetInstance()->Get(SPIDER_BULLET_EX_ANI);
	this->x = PosX;
	this->y = PosY;
	vy = speed;
	vx = CountVx(Target, PosX, PosY);
	alpha = 0;
	IsCollisionBrick = 0;
	IsDone = true;
	TimeDelay = 0;
	this->Target = Target;
}

BossBullets::~BossBullets()
{
}

void BossBullets::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOSS_BULLET_BBOX;
	bottom = y + BOSS_BULLET_BBOX;
}

void BossBullets::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt, colliable_objects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* OnlyBrick = new vector<LPGAMEOBJECT>();
	OnlyBrick->clear();
	for (int i = 0; i < colliable_objects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(colliable_objects->at(i)))
		{
			OnlyBrick->push_back(colliable_objects->at(i));
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

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

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
					IsCollisionBrick = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
				if (e->ny != 0)
				{
					IsCollisionBrick = 1;
					x += min_tx * dx + nx * 0.4f;
					y += min_ty * dy + ny * 0.4f;
					vx = 0;
					vy = 0;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void BossBullets::Render()
{
	ani->Render(x, y);
}

float BossBullets::CountVx(LPGAMEOBJECT target, int x, int y)
{
	float disX = x - target->x;
	float disY = y - target->y;
	//float dis = (float)sqrt(pow(disX,2)+pow(disY,2));
	vx = (float)vy * disX / disY;
	return this->vx;
}
