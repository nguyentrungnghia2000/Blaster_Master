#include "HUD.h"

HUD::HUD(int initCarHealth, int initPowerHealth)
{
	UIanimationSet = CAnimationSets::GetInstance()->Get(ANIMATION_SET_CAR_HEALTH_BAR);
	//PowerHB = new HealthBar(initPowerHealth, true);
	PlayerHB = new HealthBar(initCarHealth, false);
}

HUD::~HUD()
{
}

void HUD::Update(float x, float y, int currentCarHealth, int currentPowerHealth)
{
	this->x = x;
	this->y = y;

	//PowerHB->Update(currentPowerHealth, x + 2, y + 7.8);
	PlayerHB->Update(currentCarHealth, x, y);
	//DebugOut(L"current health : %d\n", currentCarHealth);
}

void HUD::Render(CGameObject* playerInfo)
{
	//UIanimationSet->at(CAR_POWER_BAR)->Render(x, y + ARTICULAR_POWER_HEALTHBAR_Y);
	UIanimationSet->at(PLAYER_HEALTH_BAR)->Render(x + 3, y + 54 + ARTICULAR_PLAYER_HEALTHBAR_Y);
	//PowerHB->Render();
	PlayerHB->Render();
	//DebugOut(L"carHUD : %d\n", CAR_HEALTH_UNIT);
}
