#include "HealthBar.h"


HealthBar::HealthBar(int maxhealth, bool isGun)
{
	this->currentHealth = maxhealth;
	this->maxHealth = maxhealth;
	this->isGun = isGun;

	for (int i = 0; i < currentHealth; i++) {
		LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(CAR_HEALTH_UNIT);
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

	if (currentHealth <= 0)
		return;
	else
	{
		for (int i = 0; i < currentHealth; i++)
		{
			if (isGun)
			{
				lsHealth_unit[i]->at(CAR_HEALTH_UNIT)->Render(0, 0 - i * UNIT_SPACE);
			}
			/*else
			{
				lsHealth_unit[j]->at(CAR_POWER_UNIT)->Render(x, y + 57 - j * UNIT_SPACE);
			}*/
		}
	}
}

void HealthBar::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
}
