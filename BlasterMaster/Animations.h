#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Sprites.h"

#define CAR_ANI_WALKING_SPRITE1	1
#define CAR_ANI_WALKING_SPRITE2	2
#define CAR_ANI_WALKING_SPRITE3	3
#define CAR_ANI_WALKING_SPRITE4	4
//#define CAR_ANI_WALKING_SPRITE5	5
//#define CAR_ANI_WALKING_SPRITE6	6
//#define CAR_ANI_WALKING_SPRITE7	7
//#define CAR_ANI_WALKING_SPRITE8	8
//#define CAR_ANI_WALKING_SPRITE9	9
//#define CAR_ANI_WALKING_SPRITE10 10
//#define CAR_ANI_WALKING_SPRITE11 11
//#define CAR_ANI_WALKING_SPRITE12 12

//#define CAR_ANI_UP_LEFT_SPRITE1	1
//#define CAR_ANI_UP_LEFT_SPRITE2	2
//#define CAR_ANI_UP_LEFT_SPRITE3	3
//#define CAR_ANI_UP_LEFT_SPRITE4	4
//#define CAR_ANI_UP_LEFT_SPRITE5	5
//
//#define CAR_ANI_UP_RIGHT_SPRITE1	6
//#define CAR_ANI_UP_RIGHT_SPRITE2	7
//#define CAR_ANI_UP_RIGHT_SPRITE3	8
//#define CAR_ANI_UP_RIGHT_SPRITE4	9
//#define CAR_ANI_UP_RIGHT_SPRITE5	10

/*
Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;

class CAnimation
{
	DWORD lastFrameTime;
	
	int defaultTime;
	vector<LPANIMATION_FRAME> frames;
public:
	int currentFrame;
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);

	void Render(float x, float y, bool PressKeyUp = false, int alpha = 255);
	void RenderCarUp(float x, float y, int alpha = 255); // render car trong trạng thái ngửa súng lên trên
	void ResetRender(float x, float y, int alpha = 255);
	void RenderCurrentFrame(int idFrame, float x, float y, int alpha = 255); // vẽ sprite theo id nhận từ GetCurrentFrame
	
	int GetFrame() { return currentFrame; }
	void SetFrame(int current_Frame) { currentFrame = current_Frame; }
	void ResetCurrentFrame() { currentFrame = -1; }
	int GetCurrentFrame(); // nhận id của sprite hiện tại trong khi di chuyển để trả về khi dừng xe
	LPANIMATION_FRAME GetAnimationCurrentFrame(int id) { return frames[id]; }
};

typedef CAnimation* LPANIMATION;

class CAnimations
{
	static CAnimations* __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();

	static CAnimations* GetInstance();
};

typedef vector<LPANIMATION> CAnimationSet;

typedef CAnimationSet* LPANIMATION_SET;

/*
	Manage animation set database
*/
class CAnimationSets
{
	static CAnimationSets* __instance;

	unordered_map<int, LPANIMATION_SET> animation_sets;

public:
	CAnimationSets();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);


	static CAnimationSets* GetInstance();
};