#include <math.h>
#include "main.h"
#include "effect.h"
#include "stage.h"


#define EFFECT_MAX			2000
#define TOBICHIRI_MAX		200
#define BLOCK_EFFECT_MAX	12	
#define TIME_MAX			150


typedef struct {
	XY pos;				// ˆÊ’u
	XY size;			// »²½Ş
	XY offsetSize;		// ·¬×’†‰›‚©‚ç‚Ì¶ãˆÊ’u
	int time;
	int timeMax;
	float speed;
	float angle;
	bool flag;
	int animCnt;
	XY move;			// Šp“x‚©‚ç‹‚ß‚½ˆÚ“®—Ê
}EFFECT;

EFFECT effect[EFFECT_MAX];

int effectImage;	
int tobichiriAcc;	// ”ò‚ÑU‚Á‚½‚Ì‰º‚Ö‚Ì‰Á‘¬
int cnt;			// ÌßÚ²Ô°‚ª€‚ñ‚Å”ò‚ÑU‚Á‚½‰ñ”

void EffectSysInit(void)
{
	if((effectImage = LoadGraph("png/ŒŒ2.png", true))== -1)AST();
}

void EffectInit(void)
{

	for (int i = 0; i < EFFECT_MAX; i++) {
		effect[i].pos = { 0,0 };
		effect[i].move = { 0, 0 };
		effect[i].timeMax = TIME_MAX;
		effect[i].time = 0;
		effect[i].size = { EFFECT_SIZE_X, EFFECT_SIZE_Y };
		effect[i].offsetSize = { effect[i].size.x / 2, effect[i].size.y / 2 };
		effect[i].angle = 0.0f;
		effect[i].speed = 8.0f;
		effect[i].flag = false;
		effect[i].animCnt = 0;
	}
	tobichiriAcc = 0;
	cnt = 0;
}

void EffectUpdate(void)
{

	for (int i = 0; i < EFFECT_MAX; i++) {
		if (effect[i].flag) {

			effect[i].time++;
			if (IsPass(effect[i].pos)) {
				effect[i].pos = { effect[i].pos.x + effect[i].move.x, effect[i].pos.y + effect[i].move.y };
			}

			effect[i].speed -= 0.1f;
			if (effect[i].speed < 0)
			{
				effect[i].speed = 0;
			}
			effect[i].move = { (int)(cos(PI / 180 * effect[i].angle) * effect[i].speed) ,(int)(sin(PI / 180 * effect[i].angle) * effect[i].speed + tobichiriAcc) };

			if (effect[i].time > effect[i].timeMax) {
				effect[i].flag = false;
				effect[i].time = 0;
			}
		}
	}
	tobichiriAcc += 1;
	
}

void TobichiriGenerator(XY pos)
{
	int effectCnt = 0;
	tobichiriAcc = 0;	// ”ò‚ÑU‚è‚Ì‰Á‘¬“x
	int angleD = 0;
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (!effect[i].flag) {
			effectCnt++;
			effect[i].pos = pos;
			effect[i].speed = GetRand(20) + 1;
			effect[i].angle = angleD;
			effect[i].move = { (int)(cos(PI / 180 * angleD) * effect[i].speed) , (int)(sin(PI / 180 * angleD) * effect[i].speed) };
			effect[i].flag = true;
			effect[i].angle = GetRand(359);
			/*angleD += 3;*/

			if (effectCnt > TOBICHIRI_MAX) {
				break;
			}
		}
	}
}



void EffectDraw(void)
{
	XY tmpMapPos = GetMapPos();
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (effect[i].flag) {
			DrawGraph(effect[i].pos.x - effect[i].offsetSize.x - tmpMapPos.x, effect[i].pos.y - effect[i].offsetSize.y - tmpMapPos.y, effectImage, true);
		}
		/*DrawFormatString(0, 15 * i + 60, 0x000000, "effectpos[%d] = %d , %d", i, effect[i].pos.x, effect[i].pos.y, true);*/
	}
}
