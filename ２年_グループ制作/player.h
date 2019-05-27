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

enum OFFSET_TYPE {
	OFFSET_LEFT_RIGHT,			// 0: movedOffsetはﾌﾟﾚｲﾔｰの移動方向の端の中心movedOffset2はﾌﾟﾚｲﾔｰの頭上の中心movedOffset3はﾌﾟﾚｲﾔｰの足下の中心				
	OFFSET_HEAD,				// 1: movedOffsetはﾌﾟﾚｲﾔｰの頭上movedOffset2はﾌﾟﾚｲﾔｰの頭上の左端movedOffset3はﾌﾟﾚｲﾔｰの足下の右端
	OFFSET_FOOT,				// 2: movedOffsetはﾌﾟﾚｲﾔｰの足下movedOffset2はﾌﾟﾚｲﾔｰの足下の左端movedOffset3はﾌﾟﾚｲﾔｰの足下の右端
	OFFSET_NOTJUMP
};

// ﾌﾟﾛﾄﾀｲﾌﾟ宣言
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