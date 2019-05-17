#pragma once
#include <DxLib.h>	

#ifdef _DEBUG
#define AST() {\
	CHAR ast_mes[256];\
	wsprintf(ast_mes, "%s %d行目\0", __FILE__, __LINE__);\
	MessageBox(0, ast_mes, "ｱｻｰﾄ表示", MB_OK);\
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
	int type;	// 当たり判定用 0: 矩形と矩形 1:矩形と線 2: ﾌﾞﾛｯｸ(死なない)
	MOVE_DIR moveDir;	// 向いている向き
	XY pos;				// 位置
	XY size;			// ｻｲｽﾞ
	XY offsetSize;		// ｷｬﾗ中央からの左上位置
	XY hitPosS;			// 当たり判定用左上
	XY hitPosE;			// 当たり判定用右下
	bool runFlag;
	bool jumpFlag;
	bool damageFlag;
	int moveSpeed;
	int animCnt;
	int cnt;
	XY_F velocity;		// 移動速度
	T_EVENT_MODE tEvent;
	int point;
	int jyuni;
	int skin;
}CHARACTER;

typedef struct {
	int PlusPt;
	int BonusPt;	//仮　特定の条件によるボーナス
	int EndPt;		//仮　ゲーム終了のための必要Pt
}PT;

typedef struct {
	int playerCnt;
	int DeathCnt;	//仮　プレイヤーの死亡回数
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