#pragma once

#define PLAYER_SIZE_X 48
#define PLAYER_SIZE_Y 48

#define INIT_VELOCITY 50	//60
#define SECOND_PER_FRAME 0.45	// 1フレーム当たりの秒数	//0.2

#define ACC_RUN 0.30		// 走る加速度
#define ACC_STOP 0.30		// 停止加速度
#define VELOCITY_X_MAX 6	// ﾌﾟﾚｲﾔｰ最大速度

// ﾌﾟﾛﾄﾀｲﾌﾟ宣言
// 1p
void PlayerSysInit(void);
void PlayerInit(void);
void PlayerUpdate(void);
void PlayerDraw(void);


CHARACTER GetPlayer(void);
