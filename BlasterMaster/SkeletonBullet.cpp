#include "SkeletonBullet.h"

SkeletonBullet::SkeletonBullet()
{
	this->Time_Ex = new Timer(SKELETON_BULLET_TIME);
	Time_Ex->Start();
}

SkeletonBullet::SkeletonBullet(int Posx, int Posy)
{
	ani = CAnimations::GetInstance()->Get(SKELETON_BULLET_ANI);
	ani_ex = CAnimations::GetInstance()->Get(SKELETON_BULLET_EX_ANI);
	this->x = Posx;
	this->vx = SKELETON_BULLET_SPEED;
	this->y = Posy;
	this->Time_Ex = new Timer(SKELETON_BULLET_TIME);
	Time_Ex->Start();
	IsCollisionBrick = 0;
	IsDone = true;
	TimeDelay = 0;
}

SkeletonBullet::~SkeletonBullet()
{
}

void SkeletonBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SKELETON_BULLET_BBOX;
	bottom = y + SKELETON_BULLET_BBOX;
}

void SkeletonBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt, colliable_objects);
	vy += SKELETON_GRAVITY * dt;
	//this->vx = SKELETON_BULLET_SPEED;
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
		DebugOut(L"x %f", dx);
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
				//CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (nx != 0 && ny == 0)
				{
					vx = -vx;
				}
				if (Time_Ex->IsTimeUp() == true)
				{
					vx = 0;
					vy = 0;
					DebugOut(L"toc do %d", vx);
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void SkeletonBullet::Render()
{
	if (Time_Ex->IsTimeUp() == false)
		ani->Render(x, y);
	else
		ani_ex->Render(x, y);
	//RenderBoundingBox();
}

bool SkeletonBullet::TimeUp()
{
	return Time_Ex->IsTimeUp();
}
