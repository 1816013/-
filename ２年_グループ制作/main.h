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
	GMODE_F_RESULT,
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
	MOVE_DIR moveDir;	// �����Ă������
	XY pos;				// �ʒu
	XY size;			// ����
	XY offsetSize;		// ��ג�������̍���ʒu
	XY hitPosS;			// �����蔻��p����
	XY hitPosE;			// �����蔻��p�E��
	bool runFlag;		// �����Ă��邩
	bool jumpFlag;		// �ެ��ߒ���
	bool headFlag;
	bool goalFlag;
	int selectFlag;
	int animCnt;		// ��Ұ��ݗp
	XY_F velocity;		// �ړ����x
	int point;
	int sortPt;
	int jyuni;
	int skin;
}CHARACTER;

typedef struct {
	int PlusPt;
	int BonusPt;	//���@����̏����ɂ��{�[�i�X
	int EndPt;		//���@�Q�[���I���̂��߂̕K�vPt
	int CoinPt;
}PT;

typedef struct {
	int playerCnt;
	int DeathCnt;	//���@�v���C���[�̎��S��
	int DeathBonusCnt;
	int GoalCnt;
}CNT;

bool SysInit(void);
void GameInit(void);
void GameTitle(void);
void GameSelect(void);
void GameBattleSelect(void);
void GameMain(void);
void GameDraw(void);
void GameResult(void);
void GameFinalResult(void);
void HitCheck();
int GetPlayerCnt(void);
void RankSort();
GAME_MODE GetGameMode();