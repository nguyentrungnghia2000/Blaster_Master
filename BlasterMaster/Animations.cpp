#include "Animations.h"
#include "Utils.h"

CAnimationSets* CAnimationSets::__instance = NULL;

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

// NOTE: sometimes Animation object is NULL ??? HOW ??? 
void CAnimation::Render(float x, float y,bool PressKeyUp, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) {
				//if(PressKeyUp != true)
					currentFrame = 0;
				//else
					//currentFrame = frames.size() - 1;
			}
		}
	}
	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
	
}

void CAnimation::RenderCarUp(float x, float y, int alpha)
{
	//currentFrame = 0;
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = frames.size() - 1;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

void CAnimation::ResetRender(float x, float y, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = frames.size() - 1;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (lastFrameTime - now > t)
		{
			currentFrame--;
			lastFrameTime = now;
			if (currentFrame == 0) currentFrame = 0;
		}
	}
	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

void CAnimation::RenderCurrentFrame(int idFrame, float x, float y, int alpha)
{
	frames[idFrame]->GetSprite()->Draw(x, y, alpha);
}

int CAnimation::GetCurrentFrame()
{
	switch (currentFrame) {
	case CAR_ANI_WALKING_SPRITE1:
		currentFrame = 1;
		break;
	case CAR_ANI_WALKING_SPRITE2:
		currentFrame = 2;
		break;
	case CAR_ANI_WALKING_SPRITE3:
		currentFrame = 3;
		break;
	case CAR_ANI_WALKING_SPRITE4:
		currentFrame = 4;
		break;
	default:
		currentFrame = 1;
		break;
	}
	return currentFrame;
}

CAnimations* CAnimations::__instance = NULL;

CAnimations* CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}

CAnimationSets::CAnimationSets()
{
}

CAnimationSets* CAnimationSets::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimationSets();
	return __instance;
}

LPANIMATION_SET CAnimationSets::Get(unsigned int id)
{
	LPANIMATION_SET ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n", id);

	return ani_set;
}

void CAnimationSets::Add(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}
