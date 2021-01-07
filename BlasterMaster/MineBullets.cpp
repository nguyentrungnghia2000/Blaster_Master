#include "MineBullets.h"
#include <random>

MineBullets::MineBullets()
{
}

MineBullets::MineBullets(int posX, int posY)
{
	ani = CAnimations::GetInstance()->Get(817);
	ani_ex = CAnimations::GetInstance()->Get(828);
	this->x = posX;
	this->y = posY;
	this->Time_jump = new Timer(TIME_JUMP);
	vy = -MINE_JUMP_SPEED_Y;
	random_device m;
	mt19937 t(m());
	uniform_real_distribution<float> minh(-.05f, .05f);
	vx = minh(t);
	//vx = -MINE_BULLET_SPEED;
}

MineBullets::~MineBullets()
{
}

void MineBullets::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MINE_BULLET_BBOX;
	bottom = y + MINE_BULLET_BBOX;
}

void MineBullets::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt, colliable_objects);
	vy += MINE_GRAVITY * dt;
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
					vx = 0;
					vy = 0;
				}
				if (e->ny != 0)
				{
					IsCollisionBrick = 1;
					vx = 0;
					vy = 0;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void MineBullets::Render()
{
	if (IsCollisionBrick == false)
		ani->Render(x, y);
	else
		ani_ex->Render(x, y);
	RenderBoundingBox();
}
