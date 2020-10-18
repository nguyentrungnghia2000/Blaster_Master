#include "Bee.h"

void Bee::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BEE_BBOX_WIDTH;
	bottom = y + BEE_BBOX_HEIGHT;
}

void Bee::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
		if (this->GetDistance(target) < DISTANCE)
		{
			IsActive = true;
		}
		if (IsActive)
		{
			x += dx;
			y += dy;
			if (time->GetStartTime() == 0)
				time->Start();
			else
			{
				if (IsBrickTop)
				{
					this->SetState(BEE_STATE_SURFING);
				}
				else
				{
					if (GetTickCount() - time->GetStartTime() > BEE_FLYING_TIME)
						this->SetState(BEE_STATE_SURFING);
				}
			}
			if (time->IsTimeUp())
			{
				time->Reset();
				if (vx < 0)
					this->SetState(BEE_STATE_FLYING);
				else
				{
					this->SetState(BEE_STATE_FLYING);
					vx = -vx;
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

			if (dynamic_cast<CBrick*>(e->obj)) // if e->obj is Goomba 
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (ny < 0)
				{
					//if(IsFlyingDown)
					IsFlyingDown = false;
					IsBrickTop = false;
					//else 
					//	vy += BEE_GRAVITY * dt;
				}
				if (ny > 0)
				{
					IsBrickTop = true;
				}
				if (nx != 0 && ny == 0)
				{
					vx = -vx;
					//vy = -vy;
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Bee::Render()
{
	if (IsActive)
	{
		int ani = BEE_ANI_FLYING_LEFT;
		if (vx > 0)
			ani = BEE_ANI_FLYING_RIGHT;
		else
			ani = BEE_ANI_FLYING_LEFT;

		animation_set->at(ani)->Render(x, y);
	}
}

Bee::Bee(LPGAMEOBJECT Target)
{
	this->target = Target;
	this->time = new Timer(BEE_FLYING_TIME + BEE_SURFING_TIME);
	IsBrickTop = false;
	IsActive = false;
	SetState(BEE_STATE_UNACTIVE);
}

void Bee::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BEE_STATE_UNACTIVE:
		vx = vy = 0;
		IsFlyingDown = false;
	case BEE_STATE_FLYING:
		IsFlyingDown = true;
		vx = -BEE_FLYING_SPEED_X;
		vy = BEE_FLYING_SPEED_Y;
		break;
	case BEE_STATE_SURFING:
		if (IsBrickTop)
		{
			vy += BEE_GRAVITY * dt;
		}
		else
		{
			if (IsFlyingDown)
				vy = -BEE_FLYING_SPEED_Y * 2;
			else
				vy = -BEE_FLYING_SPEED_Y * 3;
		}

	}
}
