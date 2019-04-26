#include <DxLib.h>
#include "main.h""
#include "stage.h"
#include "keycheck.h"

int block[11];
XY mapPos;
int coin;

EVENT_MODE event;

int nowStage[20][27] = {
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1
};

int stage1[20][27] = {
	0,0,0,0,0,1,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,10,1,1,9,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1 ,1,1,1,1,1,0,0,0,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,0,0,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,0,0,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,0,0,0,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,0,0,0,8, 1,1,1,1,10,1,1,1,0, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,4,1, 1,1,1,0,0,0,0,0,4, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,7,1,1,1,1,1,3,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0, 0,0,7,0,1,0,0,0,7, 0,0,0,0,0,0,1,1,0,
	0,1,1,1,1,1,1,1,4, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0
};

int stage2[20][27] = {
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,9,
	0,1,1,1,1,1,1,1,1, 1,0,0,0,1,1,1,1,1, 1,1,1,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1 ,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,0,0,0,0, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,0, 0,0,7,1,1,1,1,1,0,
	0,0,0,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,0,7,1,1,1,1, 1,1,0,0,0,0,7,1,1, 1,1,0,0,0,0,1,1,0,
	0,1,1,0,1,1,0,1,1, 1,1,4,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,0,1,1,1,0,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,0,1,1,1,1,0, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,0,1,1,1,1,1, 0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,0,7,0,0,0,0,0,0, 0,7,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0
};

int stage3[20][27] = {
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,0,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,0,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,9,
	0,1,1,1,1,1,0,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,0,0,0,0,0,
	0,1,1,1,1,1,0,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 10,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0, 0,0,0,0,1,1,1,1,0,
	0,1,1,1,0,0,0,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	1,1,1,1,1,1,1,1,0, 0,0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,5,0,0,7,1, 1,1,1,1,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,0,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,0,0,1,1,0,0,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 0,0,0,1,1,0,0,0,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,10,1,1,1,1,0, 0,0,0,1,1,0,0,0,0, 1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0, 0,0,0,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0
};

int stage4[20][27] = {
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,9,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,0,0,0,0,
	0,1,1,1,1,1,1,0,0, 0,0,1,1,1,1,1,1,1, 1,0,0,1,1,1,1,1,0,
	0,0,1,1,0,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,0,0,0,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,0,0,0,0, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 0,0,1,0,0,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,0,0,0,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,0, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 0,0,1,0,0,0,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,1, 1,1,1,1,1,1,1,0,0, 0,0,0,0,0,0,0,0,0
};

int stage5[20][27]
{
	0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,9,
	0,1,0,0,0,0,1,1,1, 1,1,1,1,1,0,0,1,1, 1,1,1,1,1,1,0,0,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 0,0,0,1,1,1,1,1,1, 1,1,1,0,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,0,0,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,0,
	0,1,1,1,1,1,1,1,1, 3,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,0,0,0,1,1,1, 0,1,1,0,0,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,1, 1,1,1,0,0,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,1,1,1,3,1,1, 1,1,1,1,1,1,1,1,3, 1,1,1,1,1,1,1,0,0,
	0,1,1,3,1,1,0,0,1, 1,1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,0,
	0,1,1,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,1,1,0,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,0
};

int x1, x2;

void stageSysInit(void) 
{
	LoadDivGraph("png/stage.png", 11, 11, 1, CHIP_SIZE_X, CHIP_SIZE_Y, block, true);
}

void stageInit(void)
{
	x1 = 0; 
	x2 = 1;
}

void stageMain(void)
{
	for (int i = 0; i < SCREEN_SIZE_X / CHIP_SIZE_X; i++)
	{
		for (int j = 0; j < SCREEN_SIZE_Y / CHIP_SIZE_Y; j++)
		{
			nowStage[j][i] = stage2[j][i];
		}
	}
	if (trgKey[P2_SHOT]) {
		
		stage2[x1][1] = stage2[0][1];
		x1++;
		x2++;
	
	}
}

void stageDraw(void)
{
	//ステージの描画
	for (int i = 0; i < SCREEN_SIZE_X / CHIP_SIZE_X; i++)
	{
		for (int j = 0; j < SCREEN_SIZE_Y / CHIP_SIZE_Y; j++)
		{
			//DrawGraph(i * 48, j*48, block[1], true);
			DrawGraph(i * CHIP_SIZE_X, j * CHIP_SIZE_Y, block[nowStage[j][i]], false);

			//落下する足場
			if (nowStage[j][i] != stage2[j][i])
			{
				for (int h = 19; h < 22; h++)
				{
					DrawGraph(h * CHIP_SIZE_X, 3 * CHIP_SIZE_Y, block[8], false);
				}

				for (int y = 0; y < 5; y++)
				{
					DrawGraph(5 * CHIP_SIZE_X, y * CHIP_SIZE_Y, block[0], false);
				}
				DrawGraph(5 * CHIP_SIZE_X, 5 * CHIP_SIZE_Y, block[4], false);

				for (int g = 19; g < 22; g++)
				{
					DrawGraph(g * CHIP_SIZE_X, 9 * CHIP_SIZE_Y, block[3], false);
					DrawGraph(g * CHIP_SIZE_X, 9 * CHIP_SIZE_Y, block[0], false);
				}

				DrawGraph(8 * CHIP_SIZE_X, 11 * CHIP_SIZE_Y, block[8], false);

				//動くトラップとそれを隠すブロック群
				DrawGraph(26 * CHIP_SIZE_X - 16, SCREEN_SIZE_Y - 96, block[5], false);
				DrawGraph(26 * CHIP_SIZE_X, SCREEN_SIZE_Y - 96, block[0], false);

				DrawGraph(20 * CHIP_SIZE_X, SCREEN_SIZE_Y - 192, block[4], false);
				DrawGraph(20 * CHIP_SIZE_X, SCREEN_SIZE_Y - 192, block[0], false);

				DrawGraph(11 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y, block[3], false);
				DrawGraph(11 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y, block[0], false);

				DrawGraph(15 * CHIP_SIZE_X, 5 * CHIP_SIZE_Y, block[3], false);
				DrawGraph(15 * CHIP_SIZE_X, 5 * CHIP_SIZE_Y, block[0], false);

				DrawGraph(3 * CHIP_SIZE_X, SCREEN_SIZE_Y - CHIP_SIZE_Y, block[3], false);
				DrawGraph(3 * CHIP_SIZE_X, SCREEN_SIZE_Y - CHIP_SIZE_Y, block[0], false);

				//消える足場
				DrawGraph(13 * CHIP_SIZE_X, 16 * CHIP_SIZE_Y, block[0], false);
				DrawGraph(13 * CHIP_SIZE_X, 19 * CHIP_SIZE_Y, block[0], false);
				DrawGraph(12 * CHIP_SIZE_X, 19 * CHIP_SIZE_Y, block[0], false);
			}
			// ｽﾃｰｼﾞ2
			if (nowStage[j][i] != stage1[j][i])
			{
				for (int h = 8; h < 11; h++)
				{
					DrawGraph(SCREEN_SIZE_X - CHIP_SIZE_X, h * CHIP_SIZE_Y, block[5], false);		
					DrawGraph(SCREEN_SIZE_X - CHIP_SIZE_X, h * CHIP_SIZE_Y, block[0], false);
				}

				for (int h = 20; h < 24; h++)
				{
					DrawGraph(h * CHIP_SIZE_X, 13 * CHIP_SIZE_Y, block[3], false);
					DrawGraph(h * CHIP_SIZE_X, 13 * CHIP_SIZE_Y, block[0], false);
				}

				DrawGraph(9 * CHIP_SIZE_X, 19 * CHIP_SIZE_Y, block[3], false);
				DrawGraph(9 * CHIP_SIZE_X, 19 * CHIP_SIZE_Y, block[0], false);

				DrawGraph(24 * CHIP_SIZE_X, 4 * CHIP_SIZE_Y, block[3], false);
				DrawGraph(24 * CHIP_SIZE_X, 4 * CHIP_SIZE_Y, block[0], false);

				DrawGraph(20 * CHIP_SIZE_X, 0, block[4], false);
				DrawGraph(20 * CHIP_SIZE_X, 0, block[0], false);

				DrawGraph(2 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y, block[3], false);
				DrawGraph(2 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y, block[0], false);

				DrawGraph(0, 6 * CHIP_SIZE_Y, block[6], false);
				DrawGraph(0, 6 * CHIP_SIZE_Y, block[0], false);

				DrawGraph(8 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y, block[3], false);
				DrawGraph(8 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y, block[0], false);

				DrawGraph(5 * CHIP_SIZE_X, 13 * CHIP_SIZE_Y, block[0], false);
			}
			//ステージ3
			if (nowStage[j][i] != stage1[j][i] && nowStage[j][i] != stage2[j][i] && nowStage[j][i] != stage4[j][i] && nowStage[j][i] != stage5[j][i])
			{
				DrawTurnGraph(0, 10 * CHIP_SIZE_Y, block[9], false);

				DrawGraph(0, 11 * CHIP_SIZE_Y, block[3], false);
				for (int y = 0; y < 3; y++)
				{
					DrawGraph(y * CHIP_SIZE_X, 11 * CHIP_SIZE_Y, block[0], false);
				}

				for (int h = 12; h < 14; h++)
				{
					DrawGraph(h * CHIP_SIZE_X, SCREEN_SIZE_Y - CHIP_SIZE_Y, block[0], false);
				}

				DrawGraph(20 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y, block[3], false);
				DrawGraph(20 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y, block[0], false);

				DrawGraph(15 * CHIP_SIZE_X, 16 * CHIP_SIZE_Y, block[3], false);
				DrawGraph(15 * CHIP_SIZE_X, 16 * CHIP_SIZE_Y, block[0], false);

				DrawGraph(4 * CHIP_SIZE_X, 7 * CHIP_SIZE_Y, block[3], false);
				DrawGraph(4 * CHIP_SIZE_X, 7 * CHIP_SIZE_Y, block[0], false);

				DrawGraph(13 * CHIP_SIZE_X, 17 * CHIP_SIZE_Y, coin, false);
			}
			//ステージ4
			if (nowStage[j][i] != stage1[j][i] && nowStage[j][i] != stage2[j][i] && nowStage[j][i] != stage3[j][i] && nowStage[j][i] != stage5[j][i])
			{
				for (int y = 8; y < 16; y++)
				{
					DrawGraph(y * CHIP_SIZE_X, SCREEN_SIZE_Y - CHIP_SIZE_Y, block[0], false);
				}

				for (int h = 2; h < 4; h++)
				{
					DrawGraph(h * CHIP_SIZE_X, 5 * CHIP_SIZE_Y, block[0], false);
				}

				DrawGraph(11 * CHIP_SIZE_X, 17 * CHIP_SIZE_Y, block[0], false);
				DrawGraph(11 * CHIP_SIZE_X, 11 * CHIP_SIZE_Y, block[0], false);

				for (int g = 22; g < 26; g++)
				{
					DrawGraph(g * CHIP_SIZE_X, 9 * CHIP_SIZE_Y, block[3], false);
					DrawGraph(g * CHIP_SIZE_X, 9 * CHIP_SIZE_Y, block[0], false);
				}
				DrawGraph(21 * CHIP_SIZE_X, 4 * CHIP_SIZE_Y, block[0], false);
				DrawGraph(25 * CHIP_SIZE_X, 7 * CHIP_SIZE_Y, coin, false);
			}
			//ステージ5
			if (nowStage[j][i] != stage1[j][i] && nowStage[j][i] != stage2[j][i] && nowStage[j][i] != stage3[j][i] && nowStage[j][i] != stage4[j][i])
			{
				for (int h = 9; h < 26; h++)
				{
					DrawGraph(h * CHIP_SIZE_X, SCREEN_SIZE_Y - CHIP_SIZE_Y, block[0], false);
				}
				DrawGraph(12 * CHIP_SIZE_X, 9 * CHIP_SIZE_Y, coin, false);
			}
		}
	}
}

bool IsPass(XY pos)
{
	bool ret = true;
	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = nowStage[mapIndex.y][mapIndex.x];

	switch (mapNo) {
	case 0:		// 壁
	case 2:
	case 7:
	case 8:
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
