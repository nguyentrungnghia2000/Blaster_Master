#include "Bullets.h"
#include "Brick.h"
#include <math.h>

#define SPIDER_BULLET_SPEED 0.25f

#define SPIDER_BULLET_ANI 817
#define SPIDER_BULLET_EX_ANI	1005

#define	SPIDER_BULLET_BBOX	4

#define SPIDER_BULLET_ANI	0


class SpiderBullets :public Bullets
{
	LPANIMATION ani;
	LPANIMATION ani_ex;
public:
	//SpiderBullets();
	SpiderBullets(int, int, float, LPGAMEOBJECT);
	~SpiderBullets();

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();

	float CountVx(LPGAMEOBJECT, int, int);

	bool Get_IsDead() { return isFinish; }
	void Set_IsDead(bool isfinish) { isFinish = isfinish; };
	void SubDamage(int damage) { EnermiesHealth += damage; };
};