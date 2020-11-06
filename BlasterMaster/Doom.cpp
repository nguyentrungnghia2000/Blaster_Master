#include "Doom.h"
Doom::Doom(LPGAMEOBJECT target, int state, int brick)
{
	Target = target;
	if (brick == 1)
		IsBrick = true;
	else
		IsBrick = false;
	switch (state)
	{
	case DOOM_STATE_WALKING_DOWN:
		LastState = DOOM_STATE_WALKING_DOWN;
		SetState(DOOM_STATE_WALKING_DOWN);
		break;
	case DOOM_STATE_WALKING_TOP:
		LastState = DOOM_STATE_WALKING_TOP;
		SetState(DOOM_STATE_WALKING_TOP);
		break;
	case DOOM_STATE_WALKING_LEFT:
		LastState = DOOM_STATE_WALKING_LEFT;
		SetState(DOOM_STATE_WALKING_LEFT);
		break;
	case DOOM_STATE_WALKING_RIGHT:
		LastState = DOOM_STATE_WALKING_RIGHT;
		SetState(DOOM_STATE_WALKING_RIGHT);
		break;
	}
}

void Doom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state == DOOM_STATE_WALKING_DOWN || state == DOOM_STATE_WALKING_TOP)
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
	if (state == DOOM_STATE_WALKING_DOWN)
		vy += DOOM_GRAVITY * dt;
	else if (state == DOOM_STATE_WALKING_TOP)
		vy -= DOOM_GRAVITY * dt;
	else if (state == DOOM_STATE_WALKING_LEFT)
		vx = -DOOM_WALKING_SPEED_X;
	else if (state == DOOM_STATE_WALKING_RIGHT)
		vx = +DOOM_WALKING_SPEED_X;
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
				if (state == DOOM_STATE_WALKING_RIGHT)
				{
					if (y + 0.4f > brick->y + brick->GetHeight())
					{
						//x = x + DOOM_BBOX_WIDTH_LR;
						y = brick->y + brick->GetHeight();
						this->SetState(DOOM_STATE_WALKING_TOP);
					}
					if (e->ny < 0)
					{
						LastState = DOOM_STATE_WALKING_RIGHT;
						IsBrick = true;
						this->SetState(DOOM_STATE_WALKING_DOWN);
					}
					else if (e->ny > 0)
					{
						LastState = DOOM_STATE_WALKING_RIGHT;
						IsBrick = true;
						this->SetState(DOOM_STATE_WALKING_TOP);
					}
					if (fabs(y - Target->y) < 4.0f && fabs(y - Target->y) > 2.0f)
					{
						if (Target->x - x < 0)
						{
							LastState = DOOM_STATE_WALKING_RIGHT;
							this->SetState(DOOM_STATE_SURFING);
						}
					}
				}
				else if (state == DOOM_STATE_WALKING_LEFT)
				{
					if (y + 0.4f > brick->y + brick->GetHeight())
					{
						this->SetState(DOOM_STATE_WALKING_TOP);
					}
					if (e->ny < 0)
					{
						LastState = DOOM_STATE_WALKING_LEFT;
						IsBrick = true;
						this->SetState(DOOM_STATE_WALKING_DOWN);
					}
					else if (e->ny > 0)
					{
						LastState = DOOM_STATE_WALKING_LEFT;
						IsBrick = true;
						this->SetState(DOOM_STATE_WALKING_TOP);
					}
					if (fabs(y - Target->y) < 4.0f && fabs(y - Target->y) > 2.0f)
					{
						if (Target->x - x > 0)
						{
							LastState = DOOM_STATE_WALKING_LEFT;
							this->SetState(DOOM_STATE_SURFING);
						}
					}
				}
				else if (state == DOOM_STATE_WALKING_DOWN)
				{
					if (vx > 0 && ny == 0)
					{
						LastState = DOOM_STATE_WALKING_DOWN;
						IsBrick = true;
						this->SetState(DOOM_STATE_WALKING_RIGHT);
					}
					else if (vx < 0 && ny == 0)
					{
						LastState = DOOM_STATE_WALKING_DOWN;
						IsBrick = true;
						this->SetState(DOOM_STATE_WALKING_LEFT);
					}
					else if (x + DOOM_BBOX_WIDTH_TD < brick->x + 0.4f)
					{
						SetState(DOOM_STATE_WALKING_RIGHT);
					}
				}
				else if (state == DOOM_STATE_WALKING_TOP)
				{
					if (x + 0.4f > brick->x + brick->GetWidth())
						this->SetState(DOOM_STATE_WALKING_LEFT);
					if (vx > 0 && ny == 0)
					{
						LastState = DOOM_STATE_WALKING_TOP;
						IsBrick = true;
						this->SetState(DOOM_STATE_WALKING_RIGHT);
					}
					else if (vx < 0 && ny == 0)
					{
						LastState = DOOM_STATE_WALKING_TOP;
						IsBrick = true;
						this->SetState(DOOM_STATE_WALKING_LEFT);
					}
					if (fabs(x - Target->x) < 0.5f && fabs(x - Target->x) > 0)
					{
						if (Target->y - y > 0)
						{
							LastState = DOOM_STATE_WALKING_TOP;
							IsBrick = true;
							this->SetState(DOOM_STATE_SURFING);
						}
					}
				}
				else if (state == DOOM_STATE_SURFING)
				{
					if (ny < 0)
					{
						IsBrick = true;
						this->SetState(DOOM_STATE_WALKING_DOWN);
					}
					else if (vx > 0 && ny == 0)
					{
						IsBrick = true;
						this->SetState(DOOM_STATE_WALKING_RIGHT);
					}
					else if (vx < 0 && ny == 0)
					{
						IsBrick = true;
						this->SetState(DOOM_STATE_WALKING_LEFT);
					}
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Doom::Render()
{
	int ani = DOOM_ANI_WALKING_DOWN;
	if (state == DOOM_STATE_WALKING_DOWN)
		ani = DOOM_ANI_WALKING_DOWN;
	else if (state == DOOM_STATE_WALKING_RIGHT)
		ani = DOOM_ANI_WALKING_RIGHT;
	else if (state == DOOM_STATE_WALKING_LEFT)
		ani = DOOM_ANI_WALKING_LEFT;
	else if (state == DOOM_STATE_WALKING_TOP)
		ani = DOOM_ANI_WALKING_TOP;
	else if (state == DOOM_STATE_SURFING)
	{
		if (LastState == DOOM_STATE_WALKING_LEFT)
			ani = DOOM_ANI_WALKING_LEFT;
		else if (LastState == DOOM_STATE_WALKING_RIGHT)
			ani = DOOM_ANI_WALKING_RIGHT;
		else if (LastState == DOOM_STATE_WALKING_TOP)
			ani = DOOM_ANI_WALKING_TOP;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void Doom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case DOOM_STATE_WALKING_DOWN:
		if (IsBrick)
		{
			if (LastState == DOOM_STATE_WALKING_LEFT)
				vx = DOOM_WALKING_SPEED_X;
			else
				vx = -DOOM_WALKING_SPEED_X;
		}
		else
		{
			if (LastState == DOOM_STATE_WALKING_LEFT)
				vx = -DOOM_WALKING_SPEED_X;
			else
				vx = DOOM_WALKING_SPEED_X;
		}
		break;
	case DOOM_STATE_WALKING_LEFT:
		if (IsBrick)
		{
			if (LastState == DOOM_STATE_WALKING_DOWN || LastState == DOOM_STATE_WALKING_RIGHT)
				vy = -DOOM_WALKING_SPEED_Y;
			else
				vy = DOOM_WALKING_SPEED_Y;
		}
		else
		{
			if (LastState == DOOM_STATE_WALKING_DOWN)
				vy = DOOM_WALKING_SPEED_Y;
			else
				vy = -DOOM_WALKING_SPEED_Y;
		}
		IsBrick = false;
		break;
	case DOOM_STATE_WALKING_RIGHT:
		if (IsBrick)
		{
			if (LastState == DOOM_STATE_WALKING_TOP || LastState == DOOM_STATE_WALKING_LEFT)
				vy = DOOM_WALKING_SPEED_Y;
			else
				vy = -DOOM_WALKING_SPEED_Y;
		}
		else
		{
			if (LastState == DOOM_STATE_WALKING_DOWN)
				vy = DOOM_WALKING_SPEED_Y;
			else
				vy = -DOOM_WALKING_SPEED_Y;
		}
		IsBrick = false;
		break;
	case DOOM_STATE_WALKING_TOP:
		if (IsBrick)
		{
			if (LastState == DOOM_STATE_WALKING_LEFT)
				vx = DOOM_WALKING_SPEED_X;
			else
				vx = -DOOM_WALKING_SPEED_X;
		}
		else
		{
			if (LastState == DOOM_ANI_WALKING_LEFT)
				vx = -DOOM_WALKING_SPEED_X;
			else
				vx = DOOM_WALKING_SPEED_X;
		}
		IsBrick = false;
		break;
	case DOOM_STATE_SURFING:
		switch (LastState)
		{
		case DOOM_STATE_WALKING_LEFT:
			vy = 0;
			vx = DOOM_WALKING_SPEED_X * 5;
			break;
		case DOOM_STATE_WALKING_RIGHT:
			vy = 0;
			vx = -DOOM_WALKING_SPEED_X * 5;
			break;
		case DOOM_STATE_WALKING_TOP:
			vx = 0;
			vy = DOOM_WALKING_SPEED_Y * 5;
			break;
		}
	}
}
