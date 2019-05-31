#pragma once
#define SHOT_MAX 50

typedef struct {
	bool flag;
	MOVE_DIR moveDir;	// �����Ă������
	XY pos;				// �ʒu
	XY size;			// ����
	XY offsetSize;		// ��ג�������̍���ʒu
	HIT_TYPE type;
	int animCnt;		// ��Ұ��ݗp
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