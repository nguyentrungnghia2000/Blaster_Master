#include "EndGame.h"

EndGame::EndGame()
{
	TowerMoveDown = false;
	ChangeWorld = false;
	change = false;
	tower = GetTickCount();
	ani1_x = x;
	ani1_y = y + DISTANCE_ANI;
	ani2_x = x;
	ani2_y = y;
	ani3_x = x + DISTANCE_ANI_2_3_X;
	ani3_y = y + DISTANCE_ANI_2_3_Y;
	ani_set = CAnimationSets::GetInstance()->Get(ANI_SET_ENDGAME);
}

void EndGame::Render()
{
	if (change == false) {
		ani_set->at(ANI_ENDGAME_2)->Render(ani2_x, ani2_y, alpha);
		ani_set->at(ANI_ENDGAME_3)->Render(ani3_x, ani3_y, alpha);
		ani_set->at(ANI_ENDGAME_1)->Render(ani1_x, ani1_y, alpha);
	}
	
}

void EndGame::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	GameCamera->SetCamPos(cx, cy);

	CGameObject::Update(dt, coObjects);
	if (GetTickCount() - tower > TOWER_TIME) {
		TowerMoveDown = true;
		ChangeWorld = true;
	}
	if (GetTickCount() - tower > WORLD_TIME) {
		change = true;
	}
	if (GetTickCount() - tower > CAM_TIME) {
		if (cx <= 230)
			cx++;
	}
	if (TowerMoveDown == true) {
		if (ani3_y > 143) {
			TowerMoveDown = false;
		}
		else
			ani3_y++;
	}
}

void EndGame::SetState(int state)
{
}

void EndGame::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
