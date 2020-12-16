#pragma once
#include "GameObject.h"
#include "Utils.h"
#include <vector>

#define CAR_HEALTH_BAR	0
#define CAR_HEALTH_UNIT	1
#define CAR_POWER_BAR	2
#define CAR_POWER_UNIT	3

#define ANIMATION_SET_CAR_HEALTH_BAR	30
#define UNIT_SPACE	7

#define BBOX_HEALTH_BAR_WIDTH	13
#define BBOX_HEALTH_BAR_HEIGHT	53

class HealthBar;
typedef HealthBar* LPHEALTHBAR;
class HealthBar
{
	float x, y;
	int currentHealth, maxHealth;
	vector<LPANIMATION_SET> lsHealth_unit;
	bool isGun;
public:
	HealthBar(int maxHealth, bool isGun);
	~HealthBar();

	void Update(int currentHealth, float X, float Y);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetPosition(float X, float Y) { x = X; y = Y; }
	float Get_x() { return x; }
	float Get_y() { return y; }
	void Get_Pos(float& x, float& y) { x = this->x; y = this->y; }
	void Set_x(float X) { this->x = X; }
	void Set_y(float Y) { this->y = Y; }

};

