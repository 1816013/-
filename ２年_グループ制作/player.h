#pragma once

#define PLAYER_SIZE_X 48
#define PLAYER_SIZE_Y 48

#define INIT_VELOCITY 50		// ｼﾞｬﾝﾌﾟの初期加速度
#define SECOND_PER_FRAME 0.45	// 1フレーム当たりの秒数	

#define ACC_RUN 0.5				// 走る加速度
#define ACC_STOP 0.30			// 停止加速度
#define VELOCITY_X_MAX 9		// ﾌﾟﾚｲﾔｰ最大速度


#define PLAYER_MAX 4
#define PLAYER_IMAGE_MAX 4



// ﾌﾟﾛﾄﾀｲﾌﾟ宣言
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