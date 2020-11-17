#include "Bug.h"

void Bug::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BUG_BBOX_WIDTH;
	bottom = y + BUG_BBOX_HEIGHT;
}

void Bug::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	//jump = x;
	vy += BUG_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	vector<LPGAMEOBJECT>* OnlyBrick = new vector<LPGAMEOBJECT>();
	//vector<LPGAMEOBJECT>* Player = new vector<LPGAMEOBJECT>();
	//Player->clear();
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
		//if (this->GetDistance(target) < DISTANCE * 2)
		//{
		//	//IsActive = true;
		//}
		//if (IsActive)
		//{
			x += dx;
			y += dy;
			if (fabs(this->x - target->x) > DISTANCE_TURNAROUND)
			{
				this->SetState(BUG_STATE_HUNTING);
			}
			//if(fabs(this->x - target->x) > DISTANCE_TURNAROUND*2)
			//	this->SetState(BUG_STATE_WALKING);
		//}
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

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					IsJump = false;
				}
				if (nx != 0 && ny == 0)
				{
					//IsJump = true;
					this->SetState(BUG_STATE_JUMPING);
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Bug::Render()
{
	//if (IsActive)
	//{
		int ani = BUG_ANI_WALKING_LEFT;
		if (vx > 0)
		{
			ani = BUG_ANI_WALKING_RIGHT;
		}
		else
			ani = BUG_ANI_WALKING_LEFT;
		animation_set->at(ani)->Render(x, y);
		RenderBoundingBox();
	//} 
}

Bug::Bug(LPGAMEOBJECT Target)
{
	this->IsDead = false;
	this->EnermiesHealth = ENERMIES_HEALTH;

	IsJump = false;
	IsActive = false;
	this->target = Target;
	this->SetState(BUG_STATE_WALKING);
}

void Bug::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BUG_STATE_UNACTIVE:
		IsJump = false;
		vx = vy = 0;
		break;
	case BUG_STATE_WALKING:
		//IsJump = false;
		vx = -BUG_WALKING_SPEED;
		break;
	case BUG_STATE_HUNTING:
		if (target->x - this->x < 0)
			this->vx = -fabs(this->vx);
		else
			this->vx = fabs(this->vx);
		break;
	case BUG_STATE_JUMPING:
		if (IsJump == false)
			vy = -BUG_JUMP_SPEED_Y;
		IsJump = true;
	}
}
