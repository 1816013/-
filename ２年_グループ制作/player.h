#pragma once

#define PLAYER_SIZE_X 48
#define PLAYER_SIZE_Y 48

#define INIT_VELOCITY 50	//60
#define SECOND_PER_FRAME 0.45	// 1�t���[��������̕b��	//0.2

#define ACC_RUN 0.5		// ��������x
#define ACC_STOP 0.30		// ��~�����x
#define VELOCITY_X_MAX 9	// ��ڲ԰�ő呬�x


// �������ߐ錾
// 1p
void PlayerSysInit(void);
void PlayerInit(void);
void PlayerUpdate(void);
void PlayerDraw(void);
bool PlayerHitCheck(XY pos, XY size, int shape);
void SetOffset(void);


CHARACTER GetPlayer(void);
bool GetSaveFlag();
