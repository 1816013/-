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
	MOVE_DIR moveDir;	// 向いている向き
	XY pos;				// 位置
	XY size;			// ｻｲｽﾞ
	XY offsetSize;		// ｷｬﾗ中央からの左上位置
	XY hitPosS;			// 当たり判定用左上
	XY hitPosE;			// 当たり判定用右下
	bool runFlag;		// 走っているか
	bool jumpFlag;		// ｼﾞｬﾝﾌﾟ中か
	bool headFlag;
	bool goalFlag;
	int selectFlag;
	int animCnt;		// ｱﾆﾒｰｼｮﾝ用
	XY_F velocity;		// 移動速度
	int point;
	int sortPt;
	int jyuni;
	int skin;
}CHARACTER;

typedef struct {
	int PlusPt;
	int BonusPt;	//仮　特定の条件によるボーナス
	int EndPt;		//仮　ゲーム終了のための必要Pt
	int CoinPt;
}PT;

typedef struct {
	int playerCnt;
	int DeathCnt;	//仮　プレイヤーの死亡回数
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