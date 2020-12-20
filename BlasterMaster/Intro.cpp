#include "Intro.h"
#include "Game.h"
#include "Utils.h"

Intro::Intro()
{
	timer = GetTickCount();
	ani_set = CAnimationSets::GetInstance()->Get(ANI_SET_INTRO);
}

void Intro::Render()
{
	ani_set->at(ANI_INTRO)->Render(x, y, alpha);
}

void Intro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	
	if (GetTickCount() - timer > INTRO_TIME) {
		CGame::GetInstance()->SwitchScene(1);
	}
}

void Intro::SetState(int state)
{
	CGameObject::SetState(state);
}

void Intro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
