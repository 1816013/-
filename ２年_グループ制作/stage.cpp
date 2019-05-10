#include <DxLib.h>
#include "main.h""
#include "stage.h"
#include "keycheck.h"
#include "player.h"

int block[16];
XY mapPos;
int coin;
CHARACTER trap[20]; 
int trapCnt;	// まとめて動かすﾄﾗｯﾌﾟのｶｳﾝﾀ
EVENT_MODE event;
STAGE_NUM stage;

int nowStage[20][27] = {
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0
};

int stage1[20][27] = {
	1,1,1,1,1,0,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,10,0,0,9,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,2,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,1,1,1, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,12,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,1,1,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,1,1,1,0,0,0,0,1,
	1,0,0,0,0,0,12,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,1,1,1,1, 0,0,0,0,10,0,0,0,1, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,13,0, 0,0,0,1,1,1,1,1,13, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,7,0,0,0,0,0,12,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1, 1,1,7,1,2,1,1,1,7, 1,1,1,1,1,1,0,0,1,
	1,0,0,0,0,0,0,0,13, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,2,2,1,1,1,1, 1,1,1,1,1,1,1,1,1
};

int stage2[20][27] = {
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,13,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,9,
	1,0,0,0,0,0,0,0,0, 0,1,1,1,0,0,0,0,0, 0,0,0,1,1,1,2,2,1,
	1,0,0,0,0,0,0,0,12, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,1,1,1,1, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,12,0,0,0,0,0,0, 0,0,0,0,0,0,14,1,1, 1,1,7,0,0,0,0,0,1,
	1,1,1,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,1,7,2,0,0,0, 0,0,1,1,1,1,7,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,1,0,0,1,0,0, 0,0,13,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,1,0,0,0,1,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,1,0,0,0,0,1, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,1,0,0,0,0,0, 1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,1,7,1,1,1,1,1,1, 1,7,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1
};

int stage3[20][27] = {
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,9,
	1,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,1,1,1,1,1,
	1,0,0,0,0,0,1,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 10,0,12,0,0,0,0,0,1,
	1,0,0,0,12,0,0,0,0, 0,0,0,0,0,0,0,0,1, 1,1,1,1,0,0,0,0,1,
	1,0,0,0,1,1,1,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	12,0,0,0,0,0,0,0,1, 1,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,2,1,0,0,0,0,0,0, 0,0,0,0,0,12,0,0,0, 0,0,0,0,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,7,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,1,0,0,0,12,0,0, 1,1,1,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,1,1,0,0,1,1,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 1,1,1,0,0,1,1,1,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,10,0,0,0,0,1, 1,1,1,0,0,1,1,1,1, 0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,2,2,1,1,1,1, 1,1,1,1,1,1,1,1,1
};

int stage4[20][27] = {
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,9,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,1,1,
	1,0,0,0,0,0,0,1,1, 1,1,0,0,0,0,0,0,0, 0,1,1,2,0,0,0,0,1,
	1,1,2,2,1,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,1,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,1,1,1,1, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 1,1,2,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,1,1,1,0,0,0,0,1,
	1,12,12,12,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,1,1,1,0,0,0,0,0, 0,0,0,0,0,0,0,1,1, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 1,1,2,1,1,1,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,2, 2,2,2,2,2,2,2,1,1, 1,1,1,1,1,1,1,1,1
};

int stage5[20][27]{
	1,1,1,1,1,1,1,1,0, 0,0,0,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,9,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,1,1,1,1,1,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,12,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,1,1,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,2,1,1,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,12,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,1,1,1,1,1,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,1,1,0,0, 0,2,0,0,0,2,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,15,0,0,0,0,0,0,0, 0,1,0,0,0,1,0,1,1, 7,1,0,0,0,0,0,0,1,
	1,1,1,1,0,0,0,0,0, 0,1,1,2,0,1,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,14,1,1,1,1, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,1,1,1,1,1,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,1,1,1,7,1,1,1,1, 1,2,2,2,2,2,2,2,1, 1,1,1,1,1,1,1,1,1
};

int x1, x2;

void stageSysInit(void) 
{
	if((LoadDivGraph("png/stage2.png", 16, 16, 1, CHIP_SIZE_X, CHIP_SIZE_Y, block, true))== -1)AST();
	coin = LoadGraph("png/コイン1.png", true);
}

void stageInit(void)	
{
	stage = (STAGE_NUM)(rand() % STAGE_MAX);
	for (int i = 0; i < 20; i++) {
		trapInit(i);
	}
}

void trapInit(int i) {
	// ﾄﾗｯﾌﾟ
	trap[i].flag = false;
	trap[i].pos = { 0, 0 };
	trap[i].size = { CHIP_SIZE_X, CHIP_SIZE_Y };
	trap[i].cnt = 0;
	trap[i].moveSpeed = 13;
	trap[i].type = 0;	// 矩形で初期化
	// ｽﾃｰｼﾞ1用
	if (stage == STAGE1)
	{
		switch (i) {
		case 0:		// 落下針
			trap[i].pos = { 5 * CHIP_SIZE_X, 5 * CHIP_SIZE_Y };
			break;
		case 1:		// 高速落下ﾌﾞﾛｯｸ
			trap[i].pos = { 5 * CHIP_SIZE_X, 0 };
			break;
		case 2:		// 普通落下ﾌﾞﾛｯｸ
		case 3:
		case 4:
		case 5:
			trap[i].pos = { 5 * CHIP_SIZE_X, (i - 1) * CHIP_SIZE_Y };
			break;
		case 6:		// 半分針
			trap[i].pos = { 4 * CHIP_SIZE_X, 19 * CHIP_SIZE_Y - CHIP_SIZE_Y / 6};
			trap[i].type = 1;
			trap[i].flag = true;	// 動きがないため常にtrue
			break;
		case 7:
			trap[i].pos = { 8 * CHIP_SIZE_X , 11 * CHIP_SIZE_Y };
			trap[i].type = 2;
			break;
		default:
			break;
		}
		/*trap[0].pos = { 5 * CHIP_SIZE_X, 5 * CHIP_SIZE_Y };
		trap[1].pos = { 5 * CHIP_SIZE_X, 0 };
		if (i >= 2 && i <= 5) {
			trap[i].pos = { 5 * CHIP_SIZE_X, (i - 1) * CHIP_SIZE_Y };
		}*/
		trap[1].moveSpeed = 50;
	}
	if (stage == STAGE3) {
		trap[i].pos = { 25 * CHIP_SIZE_X, 3 * CHIP_SIZE_Y - CHIP_SIZE_Y / 6 };
		trap[i].type = 1;
		trap[i].flag = true;	// 動きがないため常にtrue
	}
	if (stage == STAGE5) {
		switch (i) {
		case 0:		// 普通落下ﾌﾞﾛｯｸ
		case 1:	
		case 2:		
		case 3:
		case 4:
			trap[i].pos = { 7 * CHIP_SIZE_X + (i + 1) * CHIP_SIZE_X, 0 };
			break;
		default:
			break;
		}
		trapCnt = 5;
	}
}

void stageMain(void)
{
	for (int i = 0; i < SCREEN_SIZE_X / CHIP_SIZE_X; i++)
	{
		for (int j = 0; j < SCREEN_SIZE_Y / CHIP_SIZE_Y; j++)
		{
			switch (stage) {					// ﾗﾝﾀﾞﾑで出した値でｽﾃｰｼﾞを決める
			case STAGE1:
				nowStage[j][i] = stage1[j][i];
				break;
			case STAGE2:
				nowStage[j][i] = stage2[j][i];
				break;
			case STAGE3:
				nowStage[j][i] = stage3[j][i];
				break;
			case STAGE4:
				nowStage[j][i] = stage4[j][i];
				break;
			case STAGE5:
				nowStage[j][i] = stage5[j][i];
				break;
			default:
				AST();
				break;
			}
		}
	}
	if (trgKey[P2_SHOT]) {
		
		stage2[x1][1] = stage2[0][1];
		x1++;
		x2++;
	}

	
	CHARACTER tmp = GetPlayer();
	if (stage == STAGE1) {		
		for (int i = 0; i < 20; i++) {
			// 岩落としﾄﾗｯﾌﾟ
			if (trap[i].type == 0) {
				if (tmp.pos.x < trap[i].pos.x + 2 * CHIP_SIZE_X && tmp.pos.x > trap[i].pos.x - 3 * CHIP_SIZE_X && tmp.pos.y < trap[i].pos.y + 7 * CHIP_SIZE_Y) {
					trap[i].flag = true;						// ﾄﾗｯﾌﾟの起動
				}
				if (trap[i].flag) {
					trap[i].pos.y += trap[i].moveSpeed;
					if (trap[i].pos.y > SCREEN_SIZE_Y) {
						trap[i].cnt++;
						if (trap[i].cnt > 200) {				// 再生成の時間
							trapInit(i);
							trap[i].cnt = 0;
							trap[i].flag = false;
						}
					}
				}
			}
		}
	}
	
	if (stage == STAGE5) {
		for (int i = 0; i < 20; i++) {
			if (trap[i].type == 0) {
				if (tmp.pos.x < trap[i].pos.x + 2 * CHIP_SIZE_X && tmp.pos.x > trap[i].pos.x - 3 * CHIP_SIZE_X && tmp.pos.y < trap[i].pos.y + 7 * CHIP_SIZE_Y) {
					trap[i].flag = true;						// ﾄﾗｯﾌﾟの起動
				}
				if (trap[i].flag) {
					trap[i].pos.y += trap[i].moveSpeed;
					if (trap[i].pos.y > SCREEN_SIZE_Y) {
						trap[i].cnt++;

						if (trap[i].cnt > 200) {				// 再生成の時間
							trapInit(i);
							trap[i].cnt = 0;
							trap[i].flag = false;
						}
					}
				}
			}
		}
	}
}

void stageDraw(void)
{
	//ステージの描画
	for (int i = 0; i < SCREEN_SIZE_X / CHIP_SIZE_X; i++)
	{
		for (int j = 0; j < SCREEN_SIZE_Y / CHIP_SIZE_Y; j++)
		{
			DrawGraph(i * CHIP_SIZE_X, j * CHIP_SIZE_Y, block[nowStage[j][i]], true);

			// ｽﾃｰｼﾞ1
			if (nowStage[j][i] != stage2[j][i] && nowStage[j][i] != stage3[j][i] && nowStage[j][i] != stage4[j][i] && nowStage[j][i] != stage5[j][i])
			{
				//落下ﾄﾗｯﾌﾟ
				for (int y = 1; y < 6; y++)
				{
					DrawGraph(trap[y].pos.x, trap[y].pos.y, block[1], true);					//trap[] = { 1 ~ 5}
				}
				DrawGraph(trap[0].pos.x, trap[0].pos.y, block[4], true);						//5 * CHIP_SIZE_X,5 * CHIP_SIZE_Y

				// 小さい針
				DrawGraph(trap[6].pos.x, trap[6].pos.y, block[3], true);
				DrawGraph(4 * CHIP_SIZE_X, 19 * CHIP_SIZE_Y, block[1], true);					// ﾄﾗｯﾌﾟの下側を隠すﾌﾞﾛｯｸ

				//動くトラップとそれを隠すブロック群
				DrawGraph(26 * CHIP_SIZE_X - 16, SCREEN_SIZE_Y - 96, block[5], true);		//ステ－ジ右の針
	//			DrawGraph(26 * CHIP_SIZE_X, SCREEN_SIZE_Y - 96, block[1], true);

				DrawGraph(20 * CHIP_SIZE_X, SCREEN_SIZE_Y - 192, block[4], true);
//				DrawGraph(20 * CHIP_SIZE_X, SCREEN_SIZE_Y - 192, block[1], true);

				DrawGraph(12 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y, block[3], true);
//				DrawGraph(12 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(15 * CHIP_SIZE_X, 5 * CHIP_SIZE_Y, block[3], true);
//				DrawGraph(15 * CHIP_SIZE_X, 5 * CHIP_SIZE_Y, block[1], true);

				//消える足場
				DrawGraph(13 * CHIP_SIZE_X, 16 * CHIP_SIZE_Y, block[2], true);
				DrawGraph(13 * CHIP_SIZE_X, 19 * CHIP_SIZE_Y, block[2], true);
				DrawGraph(12 * CHIP_SIZE_X, 19 * CHIP_SIZE_Y, block[2], true);
			}
			// ｽﾃｰｼﾞ2
			if (nowStage[j][i] != stage1[j][i] && nowStage[j][i] != stage3[j][i] && nowStage[j][i] != stage4[j][i] && nowStage[j][i] != stage5[j][i])
			{
				for (int h = 8; h < 11; h++)
				{
					DrawGraph(SCREEN_SIZE_X - CHIP_SIZE_X, h * CHIP_SIZE_Y, block[5], true);		
//					DrawGraph(SCREEN_SIZE_X - CHIP_SIZE_X, h * CHIP_SIZE_Y, block[1], true);
				}

				for (int h = 20; h < 24; h++)
				{
					DrawGraph(h * CHIP_SIZE_X, 13 * CHIP_SIZE_Y, block[3], true);
//					DrawGraph(h * CHIP_SIZE_X, 13 * CHIP_SIZE_Y, block[1], true);
				}

				DrawGraph(9 * CHIP_SIZE_X, 19 * CHIP_SIZE_Y, block[3], true);
//				DrawGraph(9 * CHIP_SIZE_X, 19 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(24 * CHIP_SIZE_X, 4 * CHIP_SIZE_Y, block[3], true);
//				DrawGraph(24 * CHIP_SIZE_X, 4 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(20 * CHIP_SIZE_X, 0, block[4], true);
//				DrawGraph(20 * CHIP_SIZE_X, 0, block[1], true);

				DrawGraph(2 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y, block[3], true);
//				DrawGraph(2 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(0, 6 * CHIP_SIZE_Y, block[6], true);
//				DrawGraph(0, 6 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(8 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y, block[3], true);
//				DrawGraph(8 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(16 * CHIP_SIZE_X, 7 * CHIP_SIZE_Y, block[5], true);

//				DrawGraph(5 * CHIP_SIZE_X, 13 * CHIP_SIZE_Y, block[1], true);
			}
			//ステージ3
			if (nowStage[j][i] != stage1[j][i] && nowStage[j][i] != stage2[j][i] && nowStage[j][i] != stage4[j][i] && nowStage[j][i] != stage5[j][i])
			{
				DrawGraph(trap[0].pos.x, trap[0].pos.y, block[3], true);
				DrawGraph(25 * CHIP_SIZE_X, 3 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(0, 10 * CHIP_SIZE_Y, block[9], true);

				DrawGraph(0, 11 * CHIP_SIZE_Y, block[3], true);
				for (int y = 0; y < 3; y++)
				{
		//			DrawGraph(y * CHIP_SIZE_X, 11 * CHIP_SIZE_Y, block[1], true);
				}

				for (int h = 12; h < 14; h++)
				{
		//			DrawGraph(h * CHIP_SIZE_X, SCREEN_SIZE_Y - CHIP_SIZE_Y, block[1], true);
				}

				DrawGraph(20 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y, block[3], true);
				//DrawGraph(20 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(15 * CHIP_SIZE_X, 16 * CHIP_SIZE_Y, block[3], true);
				//DrawGraph(15 * CHIP_SIZE_X, 16 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(4 * CHIP_SIZE_X, 7 * CHIP_SIZE_Y, block[3], true);
				//DrawGraph(4 * CHIP_SIZE_X, 7 * CHIP_SIZE_Y, block[1], true);

				//DrawGraph(14 * CHIP_SIZE_X, 12 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(13 * CHIP_SIZE_X, 17 * CHIP_SIZE_Y, coin, false);
			}
			//ステージ4
			if (nowStage[j][i] != stage1[j][i] && nowStage[j][i] != stage2[j][i] && nowStage[j][i] != stage3[j][i] && nowStage[j][i] != stage5[j][i])
			{

				for (int y = 8; y < 16; y++)
				{
					DrawGraph(y * CHIP_SIZE_X, SCREEN_SIZE_Y - CHIP_SIZE_Y, block[1], true);
				}

				for (int h = 1; h < 4; h++)
				{
					DrawGraph(h * CHIP_SIZE_X, 13 * CHIP_SIZE_Y, block[12], true);
			//		DrawGraph(13 * CHIP_SIZE_X, w * CHIP_SIZE_Y, block[12], true);
				}

				DrawGraph(11 * CHIP_SIZE_X, 17 * CHIP_SIZE_Y, block[1], true);
				DrawGraph(11 * CHIP_SIZE_X, 11 * CHIP_SIZE_Y, block[1], true);

				for (int g = 22; g < 26; g++)
				{
					DrawGraph(g * CHIP_SIZE_X, 9 * CHIP_SIZE_Y, block[3], true);
					DrawGraph(g * CHIP_SIZE_X, 9 * CHIP_SIZE_Y, block[1], true);
				}

				for (int h = 2; h < 4; h++)
				{
					DrawGraph(h * CHIP_SIZE_X, 5 * CHIP_SIZE_Y, block[1], true);
				}

				DrawGraph(21 * CHIP_SIZE_X, 4 * CHIP_SIZE_Y, block[1], true);
				DrawGraph(25 * CHIP_SIZE_X, 7 * CHIP_SIZE_Y, coin, false);
			}
			//ステージ5
			if (nowStage[j][i] != stage1[j][i] && nowStage[j][i] != stage2[j][i] && nowStage[j][i] != stage3[j][i] && nowStage[j][i] != stage4[j][i])
			{
				for (int k = 0; k < 5; k++)
				{
 					DrawGraph(trap[k].pos.x, trap[k].pos.y, block[1], true);			// //trap[] = { 0 ~ 3}
				}
				DrawGraph(5 * CHIP_SIZE_X, 15 * CHIP_SIZE_Y, block[5], true);
//				DrawGraph(5 * CHIP_SIZE_X, 15 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(14 * CHIP_SIZE_X, 16 * CHIP_SIZE_Y, block[3], true);
//				DrawGraph(14 * CHIP_SIZE_X, 16 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(10 * CHIP_SIZE_X, 10 * CHIP_SIZE_Y, block[6], true);
	//			DrawGraph(10 * CHIP_SIZE_X, 10 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(13 * CHIP_SIZE_X, 12 * CHIP_SIZE_Y, block[3], true);
	//			DrawGraph(13 * CHIP_SIZE_X, 12 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(2 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y, block[3], true);
	//			DrawGraph(2 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(12 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y, block[3], true);
	//			DrawGraph(12 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y, block[1], true);

				DrawGraph(12 * CHIP_SIZE_X, 10 * CHIP_SIZE_Y, coin, false);
				DrawGraph(CHIP_SIZE_X, 5 * CHIP_SIZE_Y, coin, false);
			}
		}
	}
	//DrawFormatString(0, 60, 0x000000, "trap.pos x %d, y %d" , trap[0].pos.x, trap[0].pos.y);
}

bool IsPass(XY pos)
{
	bool ret = true;
	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = nowStage[mapIndex.y][mapIndex.x];

	switch (mapNo) {
	case 1:		// 壁
	case 7:		// ｼﾞｬﾝﾌﾟﾌﾞﾛｯｸ
	case 8:		// 消えるﾌﾞﾛｯｸ
		ret = false;
		break;
	}
	return ret;
}


// ﾃﾚﾎﾟｰﾄ
bool TelIsPass(XY pos)
{
	bool ret = true;
	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = nowStage[mapIndex.y][mapIndex.x];

	switch (mapNo) {
	case 10:
		ret = false;
		break;
	}
	return ret;
}

// 針
bool NeedleIsPass(XY pos)
{
	bool ret = true;
	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = nowStage[mapIndex.y][mapIndex.x];

	switch (mapNo) {
	case 3:
	case 4:
	case 5:
	case 6:
	case 12:
	case 13:
	case 14:
	case 15:
		ret = false;
		break;
	}
	return ret;
}

bool JumpIsPass(XY pos)
{
	bool ret = true;
	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = nowStage[mapIndex.y][mapIndex.x];

	switch (mapNo) {
	case 7:
		ret = false;
		break;
	}
	return ret;
}

// ｺﾞｰﾙ
bool GoalIsPass(XY pos)
{
	bool ret = true;
	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = nowStage[mapIndex.y][mapIndex.x];

	switch (mapNo) {
	case 9:
		ret = false;
		break;
	}
	return ret;
}

// ﾃﾞﾊﾞｯｸﾞ用
bool HitIsPass(XY pos)
{
	bool ret = true;
	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = nowStage[mapIndex.y][mapIndex.x];

	switch (mapNo) {
	case 2:
		ret = false;
		break;
	}
	return ret;
}

XY MapPosToIndex(XY pos)
{
	XY mapIndex;
	mapIndex = { pos.x / CHIP_SIZE_X,  pos.y / CHIP_SIZE_Y };

	return mapIndex;
}

XY MapIndexToPos(XY index) {
	XY mapPos;
	mapPos = { index.x * CHIP_SIZE_X, index.y * CHIP_SIZE_Y };
	return mapPos;
}

XY MapPosToMoveIndex(XY pos, bool flag, XY_F velocity) {

	XY tmpIndex;
	XY tmpPos;
	tmpIndex = MapPosToIndex(pos);

	if (flag) {
		tmpIndex.y++;
	}
	if (velocity.x < 0) {			// 左
		tmpIndex.x++;
	}
	tmpPos = MapIndexToPos(tmpIndex);

	return tmpPos;
}

XY GetMapPos() {
	return mapPos;
}

CHARACTER GetTrap(int idx) {
	return trap[idx];
}