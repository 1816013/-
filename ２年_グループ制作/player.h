#pragma once

#define PLAYER_SIZE_X 48
#define PLAYER_SIZE_Y 48

#define INIT_VELOCITY 50		// �ެ��߂̏��������x
#define SECOND_PER_FRAME 0.45	// 1�t���[��������̕b��	

#define ACC_RUN 0.5				// ��������x
#define ACC_STOP 0.30			// ��~�����x
#define VELOCITY_X_MAX 9		// ��ڲ԰�ő呬�x


#define PLAYER_MAX 4
#define PLAYER_IMAGE_MAX 4



// �������ߐ錾
// 1p
void PlayerSysInit(void);
void PlayerInit(void);
void PlayerUpdate(void);
void PlayerDraw(void);
bool PlayerHitCheck(XY pos, XY size, HIT_TYPE type, MOVE_DIR);
//void SetOffset(OFFSET_TYPE type, XY pos, XY offset, XY offset2, XY offset3);
bool PlayerKeyCheck(int i);
bool PlayerJumpKeyCheck(int i);
bool bulletKeyCheck(int i);

CHARACTER GetPlayer(int i);
int GetSelectCnt();
int GetCycleType();
int GetGoalCnt();
void GiveUp(int i);