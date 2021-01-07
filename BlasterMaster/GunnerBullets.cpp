#include "GunnerBullets.h"

GunnerBullets::GunnerBullets()
{
}

GunnerBullets::GunnerBullets(int Posx, int Posy, int type_gun)
{
	ani = CAnimations::GetInstance()->Get(GUNNER_BULLET_ANI);
	ani_ex = CAnimations::GetInstance()->Get(GUNNER_BULLET_EX_ANI);
	this->x = Posx;
	//this->vx = GUNNER_BULLET_SPEED;
	this->y = Posy;
	//this->Time_Ex = new Timer(SKELETON_BULLET_TIME);
	//Time_Ex->Start();
	IsCollisionBrick = 0;
	IsDone = true;
	TimeDelay = 0;
	Type = type_gun;
	switch (Type)
	{
	case 1:
		vy = -GUNNER_BULLET_SPEED;
		vx = 0;
		break;
	case 2:
		vy = GUNNER_BULLET_SPEED;
		vx = 0;
		break;
	case 3:
		vx = -GUNNER_BULLET_SPEED;
		vy = 0;
		break;
	case 4:
		vx = GUNNER_BULLET_SPEED;
		vy = 0;
		break;
	}
}

GunnerBullets::~GunnerBullets()
{
}

void GunnerBullets::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GUNNER_BULLET_BBOX;
	bottom = y + GUNNER_BULLET_BBOX;
}

void GunnerBullets::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
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

		//x += min_tx * dx + nx * 0.4f;
		//y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
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

void GunnerBullets::Render()
{
	if (IsCollisionBrick == false)
		ani->Render(x, y);
	else
		ani_ex->Render(x, y);
	//RenderBoundingBox();
}
