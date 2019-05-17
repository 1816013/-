#pragma once
#include <DxLib.h>	

#ifdef _DEBUG
#define AST() {\
	CHAR ast_mes[256];\
	wsprintf(ast_mes, "%s %d�s��\0", __FILE__, __LINE__);\
	MessageBox(0, ast_mes, "���ĕ\��", MB_OK);\
	}
#else
#define AST()
#endif

#define SCREEN_SIZE_X 1296
#define SCREEN_SIZE_Y 960


#define PI 3.141592

#define G 9.8
#define ACC_G 10

enum GAME_MODE {
	GMODE_INIT,
	GMODE_TITLE,
	GMODE_SELECT,
	GMODE_BATTLESELECT,
	GMODE_GAME,
	GMODE_RESULT,
	GMODE_GAMEOVER,
	GMODE_MAX
};

enum MOVE_DIR {
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_MAX
};

enum T_EVENT_MODE {
	BLOCK_FALL,
	BLOCK_STOP,
	BLOCK_POP,
	BLOCK_MOVE,
	BLOCK_SAVE
};

typedef struct {
	float x;
	float y;
}XY_F;

typedef struct {
	int x;
	int y;
}XY;

typedef struct {
	bool flag;
	int type;	// �����蔻��p 0: ��`�Ƌ�` 1:��`�Ɛ� 2: ��ۯ�(���ȂȂ�)
	MOVE_DIR moveDir;	// �����Ă������
	XY pos;				// �ʒu
	XY size;			// ����
	XY offsetSize;		// ��ג�������̍���ʒu
	XY hitPosS;			// �����蔻��p����
	XY hitPosE;			// �����蔻��p�E��
	bool runFlag;
	bool jumpFlag;
	bool damageFlag;
	int moveSpeed;
	int animCnt;
	int cnt;
	XY_F velocity;		// �ړ����x
	T_EVENT_MODE tEvent;
	int point;
	int jyuni;
	int skin;
}CHARACTER;

typedef struct {
	int PlusPt;
	int BonusPt;	//���@����̏����ɂ��{�[�i�X
	int EndPt;		//���@�Q�[���I���̂��߂̕K�vPt
}PT;

typedef struct {
	int playerCnt;
	int DeathCnt;	//���@�v���C���[�̎��S��
	int DeathBonusCnt;
}CNT;

bool SysInit(void);
void GameInit(void);
void GameTitle(void);
void GameSelect(void);
void GameBattleSelect(void);
void GameMain(void);
void GameDraw(void);
void GameResult(void);
void HitCheck();