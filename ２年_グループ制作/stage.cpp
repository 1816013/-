#include <DxLib.h>
#include <vector>
#include "main.h""
#include "stage.h"
#include "keycheck.h"
#include "player.h"

FILE* fp;

//int mapData[20*27];
int block[18];
XY mapPos;
int coin;
int arrow;
int bowgun[6];
int saveP[2];
CHARACTER trap[20]; 
int trapCnt;	// �܂Ƃ߂ē������ׯ�߂̶���

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
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,12,0, 0,0,2,1,1,1,1,1,1,
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
	1,0,0,0,0,0,0,0,13, 0,0,0,0,0,0,0,0,13, 0,0,13,0,0,0,0,0,1,
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
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,12,12,12,12,0,0,1,
	1,0,0,1,7,2,0,0,0, 0,0,1,1,1,1,7,0,0, 0,0,1,1,1,1,0,0,1,
	1,0,0,1,0,0,1,0,0, 0,0,13,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,1,0,0,0,1,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,1,0,0,0,0,1, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,1,0,0,0,0,0, 1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,1,0,0,0,0,0, 12,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
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
	1,2,1,0,0,0,0,0,0, 0,0,0,0,0,0,12,0,0, 0,0,0,0,1,1,1,1,1,
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
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,2,1,
	1,0,0,0,0,0,0,1,1, 1,1,0,0,0,0,0,0,0, 0,1,1,2,0,0,0,0,1,
	1,1,2,2,1,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,1,1,1,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,1,1,2,1, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,12,1,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 1,1,2,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,1,2,1,0,0,0,0,1,
	1,12,12,12,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,1,1,1,0,0,0,0,0, 0,0,0,0,0,0,0,1,1, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 1,2,2,1,1,1,0,0,0, 0,0,0,0,0,0,0,0,1,
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
	1,0,0,0,0,0,0,0,0, 0,1,15,0,0,1,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,15,0,0,0,0,0,0,0, 0,1,0,0,12,1,0,1,1, 7,1,0,0,0,0,0,0,1,
	1,1,1,1,0,0,0,0,0, 0,1,1,2,1,1,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,14,1,1,1,1, 0,0,0,0,0,12,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,1,1,1,1,1,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,1,1,1,7,1,1,1,1, 1,2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2,1
};

int Iwanna[20][27] = {
	1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,
	1,0,0,13,0,0,0,1,0, 0,0,0,0,0,0,0,0,1, 0,0,0,0,0,0,0,0,1,
	1,9,0,0,0,0,0,1,0, 0,0,0,1,16,1,1,1,1, 0,0,0,0,0,0,0,0,1,
	1,1,1,1,0,0,0,13,0, 0,0,0,0,0,0,13,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,0,0,12,0,0,0, 0,0,1,1,1,1,15,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,0,1,1,1,1,1,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,12, 0,0,0,0,12,0,0,0,0, 0,1,1,2,1,1,1,0,1,
	1,0,0,0,0,1,0,0,1, 15,0,0,14,1,15,0,0,0, 0,0,0,0,13,0,0,0,1,
	1,0,0,0,0,1,0,0,1, 15,0,0,0,13,0,0,0,0, 0,0,0,0,0,0,0,0,1,
	1,0,0,0,12,1,0,0,1, 15,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,1,
	1,0,1,1,1,1,0,0,1, 15,0,0,0,0,12,0,0,0, 12,0,0,0,1,0,0,0,1,
	1,0,0,0,0,0,0,0,1, 15,0,1,1,0,1,0,0,0, 1,0,0,0,1,0,16,0,1,
	1,0,0,0,0,0,0,0,1, 15,0,1,0,0,1,0,0,0, 1,0,0,14,1,0,0,0,1,
	1,0,0,0,0,0,12,0,1, 15,0,1,0,0,1,0,0,0, 1,15,0,0,1,0,0,0,1,
	1,1,1,1,1,1,1,1,1, 15,0,1,0,0,1,12,12,12, 1,0,0,0,1,1,1,0,1,
	1,0,0,0,13,0,0,0,0, 0,0,1,0,14,1,1,1,1, 1,0,1,0,13,0,13,0,1,
	1,0,0,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,0, 0,0,1,0,0,0,0,0,1,
	1,0,0,12,0,0,12,12,0,0,0,1,0,0,0,0,0,0, 0,0,1,0,0,12,0,0,1,
	1,1,1,1,1,1,1,1,1, 2,7,1,1,1,1,1,2,2, 1,1,1,1,1,1,1,1,1
};

int x1, x2;

void stageSysInit(void) 
{
	if((LoadDivGraph("png/stage3.png", 18, 16, 2, CHIP_SIZE_X, CHIP_SIZE_Y, block, true))== -1)AST();

	coin = LoadGraph("png/�R�C��1.png", true);
	if((arrow = LoadGraph("png/��1.png", true)) == - 1) AST();
	if (LoadDivGraph("png/�{�E�K����.png", 6, 6, 1, CHIP_SIZE_X, CHIP_SIZE_Y, bowgun, true) == -1) AST();
	if (LoadDivGraph("png/�Z�u�|.png", 2, 2, 1, CHIP_SIZE_X, CHIP_SIZE_Y, saveP, true) == -1) AST();
}

void stageInit(void)	
{
	stage = EX_STAGE1/*(STAGE_NUM)(rand() % STAGE_MAX)*/;

	/*switch (stage) {
	case STAGE1:
		fopen_s(&fp, "mapData/stage1.bin", "rb");
		break;
	case STAGE2:
		fopen_s(&fp, "mapData/stage2.bin", "rb");
		break;
	case STAGE3:
		fopen_s(&fp, "mapData/stage3.bin", "rb");
		break;
	case STAGE4:
		fopen_s(&fp, "mapData/stage4.bin", "rb");
		break;
	case STAGE5:
		fopen_s(&fp, "mapData/stage5.bin", "rb");
		break;
	}
	
	fread(&mapData, sizeof(mapData), 1, fp);
	fclose(fp);*/
	
	for (int i = 0; i < 20; i++) {
		trapInit(i);
	}
}

void trapInit(int i) {
	// �ׯ��
	trap[i].flag = false;
	trap[i].pos = { 0, 0 };
	trap[i].size = { CHIP_SIZE_X, CHIP_SIZE_Y };
	trap[i].cnt = 0;
	trap[i].moveSpeed = 15;
	trap[i].type = 0;															// �����蔻�����`�Ƌ�`�ŏ�����
	trap[i].tEvent = BLOCK_FALL;												// �C�x���g�͗����ŏ�����

	// �ð��1�p
	if (stage == STAGE1)
	{
		switch (i) {
			// �����j
		case 0:
			trap[i].pos = { 5 * CHIP_SIZE_X, 5 * CHIP_SIZE_Y };
			break;
			// ����������ۯ�
		case 1:
			trap[i].pos = { 5 * CHIP_SIZE_X, 0 };
			trap[i].moveSpeed = 50;
			break;
			// ���ʗ�����ۯ�
		case 2:
		case 3:
		case 4:
		case 5:
			trap[i].pos = { 5 * CHIP_SIZE_X, (i - 1) * CHIP_SIZE_Y };
			break;
			// 1/6�j
		case 6:
			trap[i].pos = { 4 * CHIP_SIZE_X, 19 * CHIP_SIZE_Y - CHIP_SIZE_Y / 6 };
			trap[i].type = 1;																	// �����蔻������������邽�߁@1
			trap[i].flag = true;																// �������Ȃ����ߏ��true
			trap[i].tEvent = BLOCK_STOP;
			break;
			// ��яo���j(���ۂ͔�яo���̂ł͂Ȃ��j�̏�ɔw�i��`�悵�ĉB���Ă����j���o��������) 
		case 7:
			trap[i].pos = { 20 * CHIP_SIZE_X, 17 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 8:
			trap[i].pos = { 12 * CHIP_SIZE_X, 7 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 9:
			trap[i].pos = { 16 * CHIP_SIZE_X, 4 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 10:
			trap[i].pos = { 17 * CHIP_SIZE_X, 17 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
			// ��
		case 11:
			trap[i].pos = { 25 * CHIP_SIZE_X, 18 * CHIP_SIZE_Y };
			trap[i].type = 2;
			trap[i].tEvent = BLOCK_MOVE;
		default:
			break;
		}
		/*trap[0].pos = { 5 * CHIP_SIZE_X, 5 * CHIP_SIZE_Y };
		trap[1].pos = { 5 * CHIP_SIZE_X, 0 };
		if (i >= 2 && i <= 5) {
			trap[i].pos = { 5 * CHIP_SIZE_X, (i - 1) * CHIP_SIZE_Y };
		}*/

	}

	if (stage == STAGE2) {
		switch (i) {
			// ��яo���j(���ۂ͔�яo���̂ł͂Ȃ��j�̏�ɔw�i��`�悵�ĉB���Ă����j���o��������) 
		case 0:
		case 1:
		case 2:
		case 3:
			trap[i].pos = { 20 * CHIP_SIZE_X + i * CHIP_SIZE_X, 12 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 4:
			trap[i].pos = { 9 * CHIP_SIZE_X, 18 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 5:
			trap[i].pos = { 20 * CHIP_SIZE_X, CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 6:
			trap[i].pos = { 2 * CHIP_SIZE_X, 7 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 7:
			trap[i].pos = { 8 * CHIP_SIZE_X, 5 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 8:
			trap[i].pos = { 15 * CHIP_SIZE_X, 7 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
			// ��
		case 9:
		case 10:
		case 11:
			trap[i].pos = { 25 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y + (i - 8) * CHIP_SIZE_Y };
			trap[i].type = 2;
			trap[i].tEvent = BLOCK_MOVE;
			break;
		default:
			break;
		}
	}

	if (stage == STAGE3) {
		switch (i) {
		case 0:
			trap[i].pos = { 25 * CHIP_SIZE_X, 3 * CHIP_SIZE_Y - CHIP_SIZE_Y / 6 };
			trap[i].type = 1;
			trap[i].flag = true;	// �������Ȃ����ߏ��true
			trap[i].tEvent = BLOCK_STOP;
			break;
			// ��яo���j
		case 1:
			trap[i].pos = { 20 * CHIP_SIZE_X, 5 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 2:
			trap[i].pos = { 15 * CHIP_SIZE_X, 15 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 3:
			trap[i].pos = { 4 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		default:
			break;
		}
	}

	if (stage == STAGE5) {
		switch (i) {
		case 0:		// ���ʗ�����ۯ�
		case 1:
		case 2:
		case 3:
		case 4:
			trap[i].pos = { 7 * CHIP_SIZE_X + (i + 1) * CHIP_SIZE_X, 0 };
			break;
		case 5:
			trap[i].pos = { 4 * CHIP_SIZE_X, 15 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@3
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 6:
			trap[i].pos = { 14 * CHIP_SIZE_X, 15 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 7:
			trap[i].pos = { 11 * CHIP_SIZE_X, 10 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 8:
			trap[i].pos = { 13 * CHIP_SIZE_X, 11 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 9:
			trap[i].pos = { 2 * CHIP_SIZE_X, 5 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
			break;
		case 10:
			trap[i].pos = { 12 * CHIP_SIZE_X, 7 * CHIP_SIZE_Y };
			trap[i].type = 4;																	// �����蔻�肪�Ȃ����߁@4
			trap[i].flag = true;																// ��ڲ԰���߂Â��܂�true
			trap[i].tEvent = BLOCK_POP;
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
			switch (stage) {					// ����тŏo�����l�Žð�ނ����߂�
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
			case EX_STAGE1:
				nowStage[j][i] = Iwanna[j][i];
				break;
			default:
				AST();
				break;
			}
			//nowStage[j][i] = mapData[j * 27 + i];
		}
	}
	
	CHARACTER tmp = GetPlayer();
	for (int i = 0; i < 20; i++) {
		if (stage == STAGE1) {
			// �◎�Ƃ��ׯ��
			if (trap[i].tEvent == BLOCK_FALL) {
				if (tmp.pos.x < trap[i].pos.x + 2 * CHIP_SIZE_X && tmp.pos.x > trap[i].pos.x - 3 * CHIP_SIZE_X && tmp.pos.y < trap[i].pos.y + 7 * CHIP_SIZE_Y) {
					trap[i].flag = true;						// �ׯ�߂̋N��
				}
				if (trap[i].flag) {
					trap[i].pos.y += trap[i].moveSpeed;
					if (trap[i].pos.y > SCREEN_SIZE_Y) {
						trap[i].cnt++;
						if (trap[i].cnt > 200) {				// �Đ����̎���
							trapInit(i);
							trap[i].cnt = 0;
							trap[i].flag = false;
						}
					}
				}
			}
			// �j��яo��
			if (trap[i].tEvent == BLOCK_POP) {
				if (tmp.pos.x < trap[i].pos.x + CHIP_SIZE_X * 2
					&& tmp.pos.x > trap[i].pos.x - CHIP_SIZE_X
					&& tmp.pos.y < trap[i].pos.y + CHIP_SIZE_Y
					&& tmp.pos.y > trap[i].pos.y - CHIP_SIZE_Y) {
					trap[i].flag = false;
				}
				else {
					trap[i].flag = true;
				}
			}
			// ��
			if (trap[i].tEvent == BLOCK_MOVE) {
				if (tmp.pos.x > trap[i].pos.x - 13 * CHIP_SIZE_X
					&& tmp.pos.y < trap[i].pos.y + CHIP_SIZE_Y * 2
					&& tmp.pos.y > trap[i].pos.y - CHIP_SIZE_Y) {
					trap[i].flag = true;
				}
				if (trap[i].flag) {
					trap[i].pos.x -= trap[i].moveSpeed;
					if (trap[i].pos.x < 0) {
						trap[i].cnt++;
						if (trap[i].cnt > 200) {				// �Đ����̎���
							trapInit(i);
							trap[i].cnt = 0;
							trap[i].flag = false;
						}
					}
				}
			}
		}

		if (stage == STAGE2) {
			// �j��яo��
			if (trap[i].tEvent == BLOCK_POP) {
				if (tmp.pos.x < trap[i].pos.x + CHIP_SIZE_X * 2
					&& tmp.pos.x > trap[i].pos.x - CHIP_SIZE_X
					&& tmp.pos.y < trap[i].pos.y + CHIP_SIZE_Y
					&& tmp.pos.y > trap[i].pos.y - CHIP_SIZE_Y) {
					trap[i].flag = false;
				}
				else {
					trap[i].flag = true;
				}
			}
			// ��
			if (trap[i].tEvent == BLOCK_MOVE) {
				trap[i].flag = true;
				if (trap[i].flag) {
					trap[i].pos.x -= trap[i].moveSpeed;
					if (trap[i].pos.x < 0) {
						trap[i].cnt++;
						trapInit(i);
						trap[i].cnt = 0;
						trap[i].flag = false;
					}
					
				}
			}
		}

		if (stage == STAGE3) {
			// �j��яo��
			if (trap[i].tEvent == BLOCK_POP) {
				if (tmp.pos.x < trap[i].pos.x + CHIP_SIZE_X * 2
					&& tmp.pos.x > trap[i].pos.x - CHIP_SIZE_X
					&& tmp.pos.y < trap[i].pos.y + CHIP_SIZE_Y
					&& tmp.pos.y > trap[i].pos.y - CHIP_SIZE_Y) {
					trap[i].flag = false;
				}
				else {
					trap[i].flag = true;
				}
			}
		}

		if (stage == STAGE5) {
			// �◎�Ƃ��ׯ��
			if (trap[i].tEvent == BLOCK_FALL) {
				if (tmp.pos.x < trap[i].pos.x + 2 * CHIP_SIZE_X && tmp.pos.x > trap[i].pos.x - 3 * CHIP_SIZE_X && tmp.pos.y < trap[i].pos.y + 7 * CHIP_SIZE_Y) {
					trap[i].flag = true;						// �ׯ�߂̋N��
				}
				if (trap[i].flag) {
					trap[i].pos.y += trap[i].moveSpeed;
					if (trap[i].pos.y > SCREEN_SIZE_Y) {
						trap[i].cnt++;

						if (trap[i].cnt > 200) {				// �Đ����̎���
							trapInit(i);
							trap[i].cnt = 0;
							trap[i].flag = false;
						}
					}
				}
			}
			// �j��яo��
			if (trap[i].tEvent == BLOCK_POP) {
				if (tmp.pos.x < trap[i].pos.x + CHIP_SIZE_X * 2
					&& tmp.pos.x > trap[i].pos.x - CHIP_SIZE_X
					&& tmp.pos.y < trap[i].pos.y + CHIP_SIZE_Y
					&& tmp.pos.y > trap[i].pos.y - CHIP_SIZE_Y) {
					trap[i].flag = false;
				}
				else {
					trap[i].flag = true;
				}
			}

		}

	}
}

void stageDraw(void)
{
	//�X�e�[�W�̕`��
	for (int i = 0; i < SCREEN_SIZE_X / CHIP_SIZE_X; i++)										// SCREEN_SIZE_X / CHIP_SIZE_X = 27
	{	
		for (int j = 0; j < SCREEN_SIZE_Y / CHIP_SIZE_Y; j++)									// SCREEN_SIZE_Y / CHIP_SIZE_Y = 20
		{
			DrawGraph(i * CHIP_SIZE_X, j * CHIP_SIZE_Y, block[nowStage[j][i]], true);

			// �ð��1
			if (stage == STAGE1)
			{
				//�����ׯ��
				DrawGraph(trap[0].pos.x, trap[0].pos.y, block[4], true);						//5 * CHIP_SIZE_X,5 * CHIP_SIZE_Y
				for (int y = 1; y < 6; y++)
				{
					DrawGraph(trap[y].pos.x, trap[y].pos.y, block[1], true);					//trap[] = { 1 ~ 5}
				}
				// �������j
				DrawGraph(trap[6].pos.x, trap[6].pos.y, block[3], true);
				DrawGraph(4 * CHIP_SIZE_X, 19 * CHIP_SIZE_Y, block[1], true);					// �ׯ�߂̉������B����ۯ�

				// ��яo���j���B���w�i
				for (int k = 7; k <= 10; k++) {
					if (trap[k].flag) {
						DrawGraph(trap[k].pos.x, trap[k].pos.y, block[0], true);				//trap[] = { 7 ~ 10}
					}
				}

				// �{�E�K���Ɩ�
				if (!trap[11].flag) {
					DrawGraph(25 * CHIP_SIZE_X, 18 * CHIP_SIZE_Y, bowgun[2], true);
				}
				else
				{
					DrawGraph(25 * CHIP_SIZE_X, 18 * CHIP_SIZE_Y, bowgun[0], true);
				}
				DrawGraph(trap[11].pos.x, trap[11].pos.y, arrow, true);				
				
			}
			// �ð��2
			if (stage == STAGE2)
			{
			// ��яo���j
				for (int k = 0; k < 9; k++)
				{
					if (trap[k].flag) {
						DrawGraph(trap[k].pos.x, trap[k].pos.y, block[0], true);
					}	
				}
				// �{�E�K���Ɩ�
				for (int h = 9; h < 12; h++)												
				{
					if (!trap[h].flag) {
						DrawGraph(25 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y + (h - 8) * CHIP_SIZE_Y, bowgun[2], true);
					}
					else {
						DrawGraph(25 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y + (h - 8) * CHIP_SIZE_Y, bowgun[0], true);
					}
					DrawGraph(trap[h].pos.x, trap[h].pos.y, arrow, true);
				}
				// ���
				DrawGraph(4 * CHIP_SIZE_X, 17 * CHIP_SIZE_Y, coin, false);
			}
			//�X�e�[�W3
			if (stage == STAGE3)
			{
				// �U�ް�
				DrawGraph(0, 10 * CHIP_SIZE_Y, block[9], true);


				// 1/6�j
				DrawGraph(trap[0].pos.x, trap[0].pos.y, block[3], true);
				DrawGraph(25 * CHIP_SIZE_X, 3 * CHIP_SIZE_Y, block[1], true);

				
				// ��яo���j
				for (int k = 1; k < 4; k++) {
					if (trap[k].flag) {
						DrawGraph(trap[k].pos.x, trap[k].pos.y, block[0], true);
					}
				}

				// ���
				DrawGraph(13 * CHIP_SIZE_X, 17 * CHIP_SIZE_Y, coin, false);
			}
			//�X�e�[�W4
			if (stage == STAGE4)
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
			//�X�e�[�W5
			if (stage == STAGE5)
			{
				for (int k = 0; k < 5; k++)
				{
 					DrawGraph(trap[k].pos.x, trap[k].pos.y, block[1], true);			// //trap[] = { 0 ~ 3}
				}
				for (int k = 5; k < 11; k++)
				{
					if (trap[k].flag) {
						DrawGraph(trap[k].pos.x, trap[k].pos.y, block[0], true);			// //trap[] = { 0 ~ 3}
					}
				}

//				DrawGraph(5 * CHIP_SIZE_X, 15 * CHIP_SIZE_Y, block[5], true);
////				DrawGraph(5 * CHIP_SIZE_X, 15 * CHIP_SIZE_Y, block[1], true);
//
//				DrawGraph(14 * CHIP_SIZE_X, 16 * CHIP_SIZE_Y, block[3], true);
////				DrawGraph(14 * CHIP_SIZE_X, 16 * CHIP_SIZE_Y, block[1], true);
//
//				DrawGraph(10 * CHIP_SIZE_X, 10 * CHIP_SIZE_Y, block[6], true);
//	//			DrawGraph(10 * CHIP_SIZE_X, 10 * CHIP_SIZE_Y, block[1], true);
//
//				DrawGraph(13 * CHIP_SIZE_X, 12 * CHIP_SIZE_Y, block[3], true);
//	//			DrawGraph(13 * CHIP_SIZE_X, 12 * CHIP_SIZE_Y, block[1], true);
//
//				DrawGraph(2 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y, block[3], true);
//	//			DrawGraph(2 * CHIP_SIZE_X, 6 * CHIP_SIZE_Y, block[1], true);
//
//				DrawGraph(12 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y, block[3], true);
//	//			DrawGraph(12 * CHIP_SIZE_X, 8 * CHIP_SIZE_Y, block[1], true);
//

				DrawGraph(12 * CHIP_SIZE_X, 10 * CHIP_SIZE_Y, coin, false);
				DrawGraph(CHIP_SIZE_X, 5 * CHIP_SIZE_Y, coin, false);
			}		}
	}
	DrawFormatString(0, 60, 0x000000, "trap.pos x %d, y %d" , trap[11].pos.x, trap[11].pos.y);
}

bool IsPass(XY pos)
{
	bool ret = true;
	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = nowStage[mapIndex.y][mapIndex.x];

	switch (mapNo) {
	case 1:		// ��
	case 7:		// �ެ�����ۯ�
	case 8:		// ��������ۯ�
		ret = false;
		break;
	}
	return ret;
}


// ���߰�
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

// �j
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

// �ް�
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

bool SaveIsPass(XY pos)
{
	bool ret = true;
	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = nowStage[mapIndex.y][mapIndex.x];

	switch (mapNo) {
	case 16:
		nowStage[mapIndex.y][mapIndex.x] = 17;
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
	if (velocity.x < 0) {			// ��
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
