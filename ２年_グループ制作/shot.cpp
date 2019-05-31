#include "DxLib.h"
#include "main.h"
#include "stage.h"
#include "player.h"
#include "keycheck.h"
#include "Shot.h"
#include "effect.h"

#define SHOT_LIFE_MAX  11

SHOT shot[SHOT_MAX];
int shotimage;

void ShotSysinit(void)
{
	shotimage = LoadGraph("png/Œõ’e.png", true);
}
void ShotInit(void)
{
	for (int i = 0; i < SHOT_MAX; i++)
	{
		shot[i].flag = false;
		shot[i].pos = {0,0};
		shot[i].size = {16,16};
		shot[i].offsetSize = { shot[i].size.x / 2 , shot[i].size.y / 2 };
		shot[i].moveDir = DIR_RIGHT;
		shot[i].lifeMax = SHOT_LIFE_MAX;
		shot[i].type = HIT_SHOT;
		
	}
}

void Shotcontrol(void)
{

	for (int i = 0; i < SHOT_MAX; i++)
	{
		if (shot[i].flag == true)
		{
			shot[i].life--;
			if (shot[i].life <= 0)
			{
				shot[i].flag = false;
			}
			if (!IsPass(shot[i].pos))
			{
				shot[i].flag = false;
			}
			switch (shot[i].moveDir)
			{
			case DIR_LEFT:
				shot[i].pos.x -= shot[i].movespeed;
				break;
			case DIR_RIGHT:
				shot[i].pos.x += shot[i].movespeed;
				break;
			default:
				break;
			}

		}
	}
}

void BulletFire(XY pPos, MOVE_DIR pDir)
{
	
	for (int i = 0; i < SHOT_MAX; i++) {
		if (!shot[i].flag)
		{
			shot[i].life = shot[i].lifeMax;
			shot[i].pos = { pPos.x,pPos.y };
			shot[i].moveDir = pDir;
			shot[i].movespeed = 15;
			shot[i].flag = true;
			break;
		}
				
	}
}
void ShotDraw(void)
{
	for (int i = 0; i < SHOT_MAX; i++){
		if (shot[i].flag == true)
		{
			DrawGraph(shot[i].pos.x,
				shot[i].pos.y,
				shotimage, true);
		}
	}
	
}
SHOT Getshot(int i) {
	return shot[i];
}

void DeleteShot(int i) {
	shot[i].flag = false;
}

