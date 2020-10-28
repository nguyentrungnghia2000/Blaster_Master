#include "Doom.h"
Doom::Doom()
{
	IsState = DOOM_STATE_WALKING_DOWN;
	SetState(DOOM_STATE_WALKING_DOWN);
}

void Doom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state == DOOM_STATE_WALKING_DOWN||state==DOOM_STATE_WALKING_TOP)
	{
		right = x + DOOM_BBOX_WIDTH_TD;
		bottom = y + DOOM_BBOX_HEIGHT_TD;
	}
	else
	{
		right = x + DOOM_BBOX_WIDTH_LR;
		bottom = y + DOOM_BBOX_HEIGHT_LR;
	}
}

void Doom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	//if(state==DOOM_STATE_WALKING_DOWN)
		//vy += DOOM_GRAVITY * dt;
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
	//for (int i = 0; i < coObjects->size(); i++)
	//{
	//	if (dynamic_cast<CMario*>(coObjects->at(i)))
	//	{
	//		Player->push_back(coObjects->at(i));
	//	}
	//}

	CalcPotentialCollisions(OnlyBrick, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		/*if (this->GetDistance(target) < DISTANCE * 2)
		{
			IsActive = true;
		}
		if (IsActive)
		{
			x += dx;
			y += dy;
			if (vx < 0)
			{
				if ((this->GetDistance(target) < DISTANCE) && (IsJump == false))
				{
					if (time->GetStartTime() == 0)
					{
						randomJump = 1 + rand() % 3;
						time->Start();
						this->SetState(ROBOT_STATE_HUNTING);
					}
					else
						this->SetState(ROBOT_STATE_HUNTING);
					if (time->IsTimeUp())
					{
						time->Reset();
						randomJump = NULL;
					}
				}
			}
			else
			{
				if ((this->GetDistance(target) < DISTANCE) && (IsJump == false))
				{
					if (time->GetStartTime() == 0)
					{
						randomJump = 1 + rand() % 4;
						time->Start();
						this->SetState(ROBOT_STATE_HUNTING);
					}
					else
						this->SetState(ROBOT_STATE_HUNTING);
					if (time->IsTimeUp())
					{
						time->Reset();
						randomJump = NULL;
						IsJump = false;
					}
				}
			}
		}*/
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
				brick->Render();
				// jump on top >> kill Goomba and deflect a bit 
				//if (ny < 0)
				//{
				//	//vx = -vx;
				//	//this->SetState(DOOM_STATE_WALKING_TOP);
				//}
				if (vx < 0 && ny == 0)
				{
					this->SetState(DOOM_STATE_WALKING_LEFT);
				}else
				if (ny > 0)
				{
					this->SetState(DOOM_STATE_WALKING_TOP);
				}else
				if (vx > 0 && ny == 0)
				{
					this->SetState(DOOM_STATE_WALKING_RIGHT);
				}else
				if (ny < 0 && nx == 0)
				{
					this->SetState(DOOM_STATE_WALKING_DOWN);
				}
				//if (ny != 0 && nx == 0)
				//{

				//}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Doom::Render()
{
	int ani = DOOM_ANI_WALKING_DOWN;
	if (IsState == DOOM_STATE_WALKING_DOWN)
		ani = DOOM_ANI_WALKING_DOWN;
	else if (IsState == DOOM_STATE_WALKING_RIGHT)
		ani = DOOM_ANI_WALKING_RIGHT;
	else if (IsState == DOOM_STATE_WALKING_LEFT)
		ani = DOOM_ANI_WALKING_LEFT;
	else if (state == DOOM_STATE_WALKING_TOP)
		ani = DOOM_ANI_WALKING_TOP;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void Doom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case DOOM_STATE_WALKING_DOWN:
		if (IsState == DOOM_STATE_WALKING_LEFT)
			vx = DOOM_WALKING_SPEED_X;
		else 
			vx = -DOOM_WALKING_SPEED_X;
		vy = 0;
		IsState = DOOM_STATE_WALKING_DOWN;
		break;
	case DOOM_STATE_WALKING_LEFT:
		if (IsState == DOOM_STATE_WALKING_DOWN)
			vy = -DOOM_WALKING_SPEED_Y;
		else
			vy = DOOM_WALKING_SPEED_Y;
		vx = 0;
		IsState = DOOM_STATE_WALKING_LEFT;
		break;
	case DOOM_STATE_WALKING_RIGHT:
		if (IsState == DOOM_STATE_WALKING_DOWN)
			vy = -DOOM_WALKING_SPEED_Y;
		else
			vy = DOOM_WALKING_SPEED_Y;
		vx = 0;
		IsState = DOOM_STATE_WALKING_RIGHT;
		break;
	case DOOM_STATE_WALKING_TOP:
		if (IsState == DOOM_STATE_WALKING_LEFT)
			vx = DOOM_WALKING_SPEED_X;
		else
			vx = -DOOM_WALKING_SPEED_X;
		vy = 0;
		IsState = DOOM_ANI_WALKING_TOP;
		break;
	}
}
