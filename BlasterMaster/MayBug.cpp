#include "MayBug.h"

void MayBug::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BEE_BBOX_WIDTH;
	bottom = y + BEE_BBOX_HEIGHT;
}

void MayBug::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	//vy += BEE_GRAVITY * dt;
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
		int random;
		if (this->GetDistance(target) < DISTANCE * 2)
		{
			IsActive = true;
		}
		if (IsActive)
		{
			x += dx;
			y += dy;
			if (time->GetStartTime() == 0)
			{
				random = rand() % 2;
				RandomUpDown = rand() % 2;
				if (random == 1)
				{
					time->Reset(MAYBUG_FLYING_TIME + MAYBUG_ROLL_TIME_MIN);
				}
				else
					time->Reset(MAYBUG_FLYING_TIME + MAYBUG_ROLL_TIME_MAX);
				time->Start();
			}
			else
			{
				if (GetTickCount() - time->GetStartTime() > MAYBUG_FLYING_TIME)
					this->SetState(MAYBUG_STATE_ROLLING);
			}
			if (time->IsTimeUp())
			{
				random = NULL;
				RandomUpDown = NULL;
				time->Reset();
				this->SetState(MAYBUG_STATE_FLYING);
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

				if (nx != 0 && ny == 0)
				{
					vx = -vx;
					if (IsFlyingLeft)
					{
						IsFlyingLeft = false;
					}
					else
						IsFlyingLeft = true;
				}
				else
				{
					time->Reset();
					this->SetState(MAYBUG_STATE_FLYING);
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void MayBug::Render()
{
	if (IsActive == true)
	{
		int ani = MAYBUG_ANI_FLYING_LEFT;
		if (vx != 0)
		{
			if (vx > 0)
				ani = MAYBUG_ANI_FLYING_RIGHT;
			else
				ani = MAYBUG_ANI_FLYING_LEFT;
		}
		else
			ani = MAYBUG_ANI_ROLLING;
		animation_set->at(ani)->Render(x, y);
	}
	RenderBoundingBox();
}

MayBug::MayBug(LPGAMEOBJECT Target)
{
	this->time = new Timer(MAYBUG_FLYING_TIME);
	IsActive = false;
	RandomUpDown = 0;
	this->target = Target;
	IsFlyingLeft = true;
	SetState(MAYBUG_STATE_UNACTIVE);
}

void MayBug::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MAYBUG_STATE_UNACTIVE:
		vx = vy = 0;
	case MAYBUG_STATE_FLYING:
		//IsFlyingDown = true;
		if (IsFlyingLeft)
		{
			vx = -MAYBUG_SPEED_X;
			vy = 0;
		}
		else
		{
			vx = MAYBUG_SPEED_X;
			vy = 0;
		}
		break;
	case MAYBUG_STATE_ROLLING:
		vx = 0;
		if (RandomUpDown == 1)
			vy = -MAYBUG_SPEED_Y;
		else
			vy = MAYBUG_SPEED_Y;
	}
}
