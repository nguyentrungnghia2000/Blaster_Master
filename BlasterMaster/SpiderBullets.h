#include "Bullets.h"
#include <math.h>

class SpiderBullets :public Bullets
{
public:

	SpiderBullets();
	~SpiderBullets();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
};