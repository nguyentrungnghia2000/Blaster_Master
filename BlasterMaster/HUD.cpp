#include "HUD.h"

HUD::HUD(int initCarHealth, int initPowerHealth)
{
	UIanimationSet = CAnimationSets::GetInstance()->Get(ANIMATION_SET_CAR_HEALTH_BAR);
	//PowerHB = new HealthBar(initPowerHealth, true);
	CarHB = new HealthBar(initCarHealth, false);
}

HUD::~HUD()
{
}

void HUD::Update(float x, float y, int currentCarHealth, int currentPowerHealth)
{
	this->x = x;
	this->y = y;

	//PowerHB->Update(currentPowerHealth, x + 2, y + 7.8);
	CarHB->Update(currentCarHealth, x, y);
}

void HUD::Render(CGameObject* playerInfo)
{
	//UIanimationSet->at(CAR_POWER_BAR)->Render(x, y + ARTICULAR_POWER_HEALTHBAR_Y);
	UIanimationSet->at(CAR_HEALTH_BAR)->Render(x + 3, y + 54 + ARTICULAR_CAR_HEALTHBAR_Y);
	//PowerHB->Render();
	CarHB->Render();
}
