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

enum OFFSET_TYPE {
	OFFSET_LEFT_RIGHT,			// 0: movedOffset����ڲ԰�̈ړ������̒[�̒��SmovedOffset2����ڲ԰�̓���̒��SmovedOffset3����ڲ԰�̑����̒��S				
	OFFSET_HEAD,				// 1: movedOffset����ڲ԰�̓���movedOffset2����ڲ԰�̓���̍��[movedOffset3����ڲ԰�̑����̉E�[
	OFFSET_FOOT,				// 2: movedOffset����ڲ԰�̑���movedOffset2����ڲ԰�̑����̍��[movedOffset3����ڲ԰�̑����̉E�[
	OFFSET_NOTJUMP
};

// �������ߐ錾
// 1p
void PlayerSysInit(void);
void PlayerInit(void);
void PlayerUpdate(void);
void PlayerDraw(void);
bool PlayerHitCheck(XY pos, XY size, int shape);
void SetOffset(OFFSET_TYPE);
bool PlayerKeyCheck(int i);
bool PlayerJumpKeyCheck(int i);

CHARACTER GetPlayer(int i);
int GetSelectCnt();