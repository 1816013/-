#pragma once
#include "DxLib.h"
#include <math.h>
#include "keycheck.h"

#ifdef _DEBUG
#define AST() {\
	CHAR ast_mes[256];\
	wsprintf(ast_mes, "%s %d�s��\0", __FILE__, __LINE__);\
	MessageBox(0, ast_mes, "���ĕ\��", MB_OK);\
	}
#else
#define AST()
#endif

// �萔
#define SCREEN_SIZE_X 1296
#define SCREEN_SIZE_Y 960
#define COLOR_MODE 16

#define PI 3.141592

#define G 9.8
#define ACC_G 10



// ��`
enum GAME_MODE {
	GMODE_INIT,
	GMODE_TITLE,
	GMODE_GAME,
	GMODE_CLEAR,
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
	int type;
	MOVE_DIR moveDir;	// �����Ă������
	XY pos;				// �ʒu
	XY size;			// ����
	XY offsetSize;		// ��ג�������̍���ʒu
	XY hitPosS;			// �����蔻��p����
	XY hitPosE;			// �����蔻��p�E��
	bool runFlag;
	bool jumpFlag;
	bool shotFlag;
	bool damageFlag;
	int moveSpeed;
	int life;
	int lifeMax;
	int animCnt;
	int imgLockCnt;		// ��ׂ̲Ұ�ތŒ�p���� 
	XY_F velocity;		// �ړ����x
}CHARACTER;

// �������ߐ錾
bool SysInit(void);
void GameInit(void);
void GameMain(void);
void GameTitle(void);
void GameDraw(void);
void GameClear(void);
void GameOver(void);
void HitCheck(void);
bool FadeInScreen(int);
bool FadeOutScreen(int);
void DrawWord(const char* title, int top, int start);
