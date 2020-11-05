#include "MainBullets.h"

MainBullets::MainBullets()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ANIMATION_SET_SMALL_JASON_BULLET));
	x = 0;
	y = 0;
	alpha = 0;
	IsCollision = 0;
	IsDone = true;
	TimeDelay = 0;
	TimeDelayMax = SMALL_JASON_BULLET_DELAY;
}

MainBullets::~MainBullets()
{
}

void MainBullets::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (IsTargetTop == false)
	{
		right = x + SMALL_BULLET_BBOX_WIDTH;
		bottom = y + SMALL_BULLET_BBOX_HEIGHT;
	}
	else
	{
		right = x + SMALL_BULLET_BBOX_HEIGHT;
		bottom = y + SMALL_BULLET_BBOX_WIDTH;
	}
}

void MainBullets::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	if (IsTargetTop == false) {
		vx = BULLET_SPEED * BulletDirection;
	}
	if (GetTickCount() - timer < EXPLOTION_TIME && flag == true) {
		isFinish = true;
		flag = false;
	}
#pragma region Xử lý va chạm
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(colliable_objects, coEvents);

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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->nx != 0)
			{
				IsCollision = 1;
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
				vx = 0;
				vy = 0;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion
	//}
}

void MainBullets::Render()
{
	//RenderBoundingBox();
	int ani;
	if (TimeDelay >= TimeDelayMax)
	{
		IsDone = true;
		TimeDelay = 0;
	}
	else
	{
		if (IsCollision == 0)
		{
			if (IsTargetTop == true)
			{
				/*ani = SMALL_BULLET_JASON_ANI_TOP;
				animation_set->at(ani)->Render(x, y, alpha);*/
			}
			else
			{
				ani = SMALL_BULLET_JASON_ANI_RIGHT;
				animation_set->at(ani)->Render(x, y, alpha);
			}
		}
		else
		{
			timer = GetTickCount();
			flag = true;
			ani = SMALL_BULLET_JASON_BANG_ANI;
			animation_set->at(ani)->Render(x, y - DISTANCE_TO_BANG, alpha);
			if (animation_set->at(ani)->GetFrame() == 2)
			{
				IsDone = true;
				TimeDelay = 0;
			}
		}
	}
}
