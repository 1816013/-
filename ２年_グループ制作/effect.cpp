#include <math.h>
#include "main.h"
#include "effect.h"
#include "stage.h"


#define EFFECT_MAX			9000
#define TOBICHIRI_MAX		300
#define BLOCK_EFFECT_MAX	12	
#define TIME_MAX			150
#define ANIM_MAX			30


typedef struct {
	XY pos;				// �ʒu
	XY size;			// ����
	XY offsetSize;		// ��ג�������̍���ʒu
	int time;			// �̪�Ă�������܂ł̎���
	int timeMax;		// �̪�Ă�������܂ł̍ő厞��
	float speed;		// ��юU�鑬�x
	float angle;		// ��юU��p�x
	bool flag;			// true: �̪�Ă��o���@false: �������Ȃ�
	int animCnt;		// ��Ұ��ݗp����
	int animMax;		// ��Ұ��ݗp�����̍ő�
	XY move;			// �p�x���狁�߂��ړ���
}EFFECT;

EFFECT effect[EFFECT_MAX];

int effectImage;	
int headImage[8];
int tobichiriAcc;	// ��юU�������̉��ւ̉���
int cnt;			// ��ڲ԰������Ŕ�юU������

void EffectSysInit(void)
{
	if((effectImage = LoadGraph("png/��2.png", true))== -1)AST();
	LoadDivGraph("png/��]����v���C���[�̓�.png", 8, 8, 1, 48, 48, headImage, true);

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
		effect[i].animMax = ANIM_MAX;
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
			
			// ���A�j���[�V�����p
			if (i % (TOBICHIRI_MAX + 1) == 0) {
				if (effect[i].animCnt < effect[i].animMax) {
					if (IsPass(effect[i].pos)) {
						effect[i].animCnt++;
					}
				}
			}
		}
	}
	tobichiriAcc += 1;
	
}

void TobichiriGenerator(XY pos)
{
	int effectCnt = 0;
	tobichiriAcc = 0;	// ��юU��̉����x
	int angleD = 0;
	for (int i = 0; i < EFFECT_MAX; i++) {
		if (!effect[i].flag) {
			effectCnt++;
			effect[i].animMax = GetRand(ANIM_MAX) + ANIM_MAX ;
			effect[i].animCnt = 0;
			effect[i].pos = pos;
			effect[i].speed = GetRand(20) + 1;
			effect[i].angle = angleD;
			effect[i].move = { (int)(cos(PI / 180 * angleD) * effect[i].speed) , (int)(sin(PI / 180 * angleD) * effect[i].speed) };
			effect[i].flag = true;
			effect[i].angle = GetRand(179) + 180;
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
			if (i % (TOBICHIRI_MAX + 1) == 0) {
				DrawGraph(effect[i].pos.x - effect[i].offsetSize.x - tmpMapPos.x, effect[i].pos.y - effect[i].offsetSize.y - tmpMapPos.y, headImage[effect[i].animCnt % 8], true);
			}
		}
		
		/*DrawFormatString(0, 15 * i + 60, 0x000000, "effectpos[%d] = %d , %d", i, effect[i].pos.x, effect[i].pos.y, true);*/
	}
}
