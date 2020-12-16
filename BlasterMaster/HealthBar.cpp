#include "HealthBar.h"


HealthBar::HealthBar(int maxhealth, bool isGun)
{
	this->currentHealth = maxhealth;
	this->maxHealth = maxhealth;
	this->isGun = isGun;

	for (int i = 0; i < currentHealth; i++) {
		LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(ANIMATION_SET_CAR_HEALTH_BAR);
		//DebugOut(L"carHUD : %d\n", CAR_HEALTH_UNIT);
		lsHealth_unit.push_back(ani_set);
	}
}

HealthBar::~HealthBar()
{
}

void HealthBar::Update(int currenthealth, float X, float Y)
{
	currentHealth = currenthealth;
	if (currentHealth <= 0)
		return;
	x = X;
	y = Y;
}

void HealthBar::Render()
{
	if (this->currentHealth <= 0)
		return;
	else
	{
		for (int i = 0; i < currentHealth; i++)
		{
			lsHealth_unit[i]->at(CAR_HEALTH_UNIT)->Render(x + 4, y + 166 - i * UNIT_SPACE);
		}
	}
}

void HealthBar::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
}
