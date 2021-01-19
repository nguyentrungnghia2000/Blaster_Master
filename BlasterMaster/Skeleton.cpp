#include "Skeleton.h"

void Skeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SKELETON_BBOX_WALKING;
	bottom = y + SKELETON_BBOX_WALKING;
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* OnlyBrick = new vector<LPGAMEOBJECT>();
	OnlyBrick->clear();

	if (this->EnermiesHealth == 0) {
		this->IsDead = true;
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
		if (vx <= 0)
		{
			if ((0 > x - Target->x) && (Target->x - x < 10))
			{
				if (time_attack->GetStartTime() == 0)
				{
					time_attack->Start();
					LastDis = 0;
					state = SKELETON_STATE_ATTACK;
					this->SetState(SKELETON_STATE_ATTACK);
					Bullet.push_back(new SkeletonBullet(this->x + SKELETON_BBOX_WALKING, this->y + SKELETON_BBOX_WALKING));
					//DebugOut(L"Size bullet %d", Bullet.size());
				}
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

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (vx <= 0)
				{
					if ((0 > x - Target->x) && (Target->x - x < 10))
					{
						if (time_attack->GetStartTime() == 0)
						{
							time_attack->Start();
							LastDis = 0;
							state = SKELETON_STATE_ATTACK;
							this->SetState(SKELETON_STATE_ATTACK);
						}
					}
				}
				if (state == SKELETON_STATE_ATTACK)
				{
					if (time_attack->IsTimeUp())
					{
						//time_attack->Reset();
						state = SKELETON_STATE_WALKING;
						this->SetState(SKELETON_STATE_WALKING);
					}
				}
				if (nx != 0 && ny == 0)
					vx = -vx;
			}
		}
	}
	for (int i = 0; i < Bullet.size(); i++)
	{
		Bullet.at(i)->Update(dt, coObjects);
	}
	for (int i = 0; i < Bullet.size(); i++)
	{
		if (Bullet.at(i)->TimeUp() == true)
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
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Skeleton::Render()
{
	int ani = SKELETON_ANI_WALKING_LEFT;
	if (state == SKELETON_STATE_WALKING)
	{
		if (vx > 0)
			ani = SKELETON_ANI_WALKING_RIGHT;
		else if (vx < 0)
			ani = SKELETON_ANI_WALKING_LEFT;
	}
	else if (state == SKELETON_STATE_ATTACK)
	{
		if (LastDis == 1)
			ani = SKELETON_ANI_ATTACK_LEFT;
		else if (LastDis == 0)
			ani = SKELETON_ANI_ATTACK_RIGHT;
	}
	//DebugOut(L"%d\n", state);
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
	for (int i = 0; i < Bullet.size(); i++)
		Bullet.at(i)->Render();
}

Skeleton::Skeleton(LPGAMEOBJECT target)
{
	time_attack = new Timer(SKELETON_ATTACK_TIME);
	time_dis = new Timer(SKELETON_BULLET_DIS);
	Target = target;
	this->EnermiesHealth = ENERMIES_HEALTH;
	this->SetState(SKELETON_STATE_WALKING);
}

void Skeleton::SetState(int state)
{
	switch (state)
	{
	case SKELETON_STATE_WALKING:
		vx = -SKELETON_WALKING_SPEED;
		vy = 0;
		break;
	case SKELETON_STATE_ATTACK:
		vx = 0;
		vy = -SKELETON_WALKING_SPEED;
	}
}
