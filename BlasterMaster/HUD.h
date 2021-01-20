#pragma once
#include "HealthBar.h"
#include "GameObject.h"
#include "Car.h"

using namespace std;


#define ARTICULAR_POWER_HEALTHBAR_Y				40
#define ARTICULAR_PLAYER_HEALTHBAR_Y			119

class HUD
{
	float x, y;
	LPANIMATION_SET UIanimationSet;

	HealthBar* PlayerHB;
	HealthBar* PowerHB;

public:
	int health_unit, power_unit;
	HUD(int initCarHealth, int initPowerHealth);
	~HUD();

	void Update(float x, float y, int currentCarHealth, int currentPowerHealth);
	void Render(CGameObject* playerInfo);
};

