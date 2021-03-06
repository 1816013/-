#pragma once
#define SHOT_MAX 50

typedef struct {
	bool flag;
	MOVE_DIR moveDir;	// 向いている向き
	XY pos;				// 位置
	XY size;			// ｻｲｽﾞ
	XY offsetSize;		// ｷｬﾗ中央からの左上位置
	HIT_TYPE type;
	int animCnt;		// ｱﾆﾒｰｼｮﾝ用
	int movespeed;
	int lifeMax;
	int life;
}SHOT;

void ShotSysinit(void);
void ShotInit(void);
void Shotcontrol(void);
void ShotDraw(void);
void BulletFire(XY pPos, MOVE_DIR pDir);
SHOT Getshot(int i);
void DeleteShot(int i);
//bool bulletKeyCheck(int i);