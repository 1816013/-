#include "main.h"
#include "player.h"
#include "keycheck.h"
#include "stage.h"
#include "shot.h"
#include "effect.h"

CHARACTER player2;
//CHARACTER player2;

int p2StopImage[2];
int p2RunImage[2][4];
int p2JumpImage[2];
int p2DamageImage;

int p2lifeImage;

bool head2Flag;		// ジャンプして頭をぶつけたかのフラグ 
//int timeCnt;

void Player2SysInit(void)
{
	p2StopImage[0] = LoadGraph("image/stop2.png");
	p2StopImage[1] = LoadGraph("image/stop_shot2.png");

	p2JumpImage[0] = LoadGraph("image/jump2.png");
	p2JumpImage[1] = LoadGraph("image/jump_shot2.png");

	p2DamageImage = LoadGraph("image/damage2.png");

	LoadDivGraph("image/run2.png", 4, 4, 1, PLAYER_SIZE_X, PLAYER_SIZE_Y, p2RunImage[0], true);
	LoadDivGraph("image/run_shot2.png", 4, 4, 1, PLAYER_SIZE_X, PLAYER_SIZE_Y, p2RunImage[1], true);
	p2lifeImage = LoadGraph("image/life2.png");
}

void Player2Init(void)
{
	player2.moveDir = DIR_LEFT;

	player2.pos = { 18 * MAP_CHIP_SIZE_X , 10 * MAP_CHIP_SIZE_Y };

	player2.size = { PLAYER_SIZE_X, PLAYER_SIZE_Y };
	player2.offsetSize = { player2.size.x / 2, player2.size.y / 2 };
	player2.hitPosS = { 20,  16 };
	player2.hitPosE = { 20,  32 };
	player2.velocity = { 0,0 };
	//player2.moveSpeed = 4;
	player2.flag = true;
	player2.runFlag = false;
	player2.jumpFlag = false;
	player2.shotFlag = false;
	player2.damageFlag = false;
	player2.animCnt = 0;
	player2.imgLockCnt = 100;
	player2.lifeMax = 10;
	player2.life = player2.lifeMax;

	head2Flag = false;
	/*timeCnt = 0;*/
}

void Player2Update(void)
{
	bool player2Moved = false;

	XY movedPos = player2.pos;
	XY movedOffset = movedPos;
	XY movedOffset2 = movedPos;
	XY movedOffset3 = movedPos;
	XY tmpIndex;
	XY tmpPos;

	XY player2oldPos = player2.pos;

	player2.imgLockCnt++;
	player2.runFlag = false;
	player2.jumpFlag = true;
	head2Flag = false;

	// ﾌﾟﾚｲﾔｰの移動
	if (player2.flag) {
		if (newKey[P2_RIGHT]) {
			player2.moveDir = DIR_RIGHT;
			player2.velocity.x += ACC_RUN * 1;	// 速度の更新
			player2Moved = true;
			player2.runFlag = true;
			if (player2.velocity.x > VELOCITY_X_MAX) {
				player2.velocity.x = VELOCITY_X_MAX;
			}
		}

		if (newKey[P2_LEFT]) {
			player2.moveDir = DIR_LEFT;
			player2.velocity.x -= ACC_RUN * 1;	// 速度の更新
			player2Moved = true;
			player2.runFlag = true;
			if (player2.velocity.x < -VELOCITY_X_MAX) {
				player2.velocity.x = -VELOCITY_X_MAX;
			}
		}
		if (!player2Moved) {
			if (player2.velocity.x > 0) {
				player2.velocity.x -= ACC_STOP;
			}
			if (player2.velocity.x < 0) {
				player2.velocity.x += ACC_STOP;
			}
		}


		// 旧移動制限
		XY tmpMapPos = GetMapPos();
		if (player2.velocity.x != 0) {
			if (movedPos.x < tmpMapPos.x + 48)
			{
				player2.velocity.x = 0;
				movedPos.x = tmpMapPos.x + 48;
			}
			if (movedPos.x > tmpMapPos.x + SCREEN_SIZE_X - 48) {
				player2.velocity.x = 0;
				movedPos.x = tmpMapPos.x + SCREEN_SIZE_X - 48;
			}
		}

		/*movedPos = player2.pos;
		movedOffset = movedPos;*/
		player2.animCnt++;

		movedPos.x += player2.velocity.x * 1;	// 距離の更新

		if (player2.velocity.x > 0) {
			movedOffset.x = movedPos.x + player2.hitPosE.x;
		}
		if (player2.velocity.x < 0) {
			movedOffset.x = movedPos.x - player2.hitPosS.x;
		}
		movedOffset2 = movedOffset;
		movedOffset2.y = movedPos.y - player2.hitPosS.y;
		movedOffset3 = movedOffset;
		movedOffset3.y = movedPos.y + player2.hitPosE.y - 1;

		if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
			player2.pos = movedPos;
		}
		else {
			tmpPos = MapPosToMoveIndex(movedOffset, player2.jumpFlag, player2.velocity);
			/*tmpIndex = MapPosToIndex(movedOffset);
			//movedOffset.y /32;*/
			if (player2.velocity.x > 0) {			// 右
				/*tmpPos = MapIndexToPos(tmpIndex);
				//(movedOffset.y / 32) * 32;*/

				player2.pos.x = tmpPos.x - player2.hitPosE.x;
			}
			if (player2.velocity.x < 0) {			// 左
				/*tmpIndex.x++;
				tmpPos = MapIndexToPos(tmpIndex);
				//(movedOffset.y / 32) * 32;*/

				player2.pos.x = tmpPos.x + player2.hitPosS.x;
			}
			movedPos = player2.pos;
			player2.velocity.x = 0;
		}


		// ｼﾞｬﾝﾌﾟ
		if (player2.jumpFlag) {
			movedPos = player2.pos;
			movedOffset = movedPos;

			movedPos.y -= player2.velocity.y * SECOND_PER_FRAME;
			player2.velocity.y -= ACC_G * SECOND_PER_FRAME;

			// 頭上のﾌﾞﾛｯｸのﾁｪｯｸ
			movedOffset.y = movedPos.y - player2.hitPosS.y;
			movedOffset2 = movedOffset;							// 左上
			movedOffset2.x = movedPos.x - player2.hitPosS.x;
			movedOffset3 = movedOffset;							// 右上
			movedOffset3.x = movedPos.x + player2.hitPosE.x - 1;

			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
				player2.pos = movedPos;
			}
			else {
				head2Flag = true;
				tmpPos = MapPosToMoveIndex(movedOffset, head2Flag, player2.velocity);
				/*tmpIndex = MapPosToIndex(movedOffset);
				// movedOffset.y /32;
				tmpIndex.y++;
				tmpPos = MapIndexToPos(tmpIndex);
				// (movedOffset.y / 32) * 32*/
				player2.pos.y = tmpPos.y + player2.hitPosS.y;
				player2.velocity.y *= -1;
			}
			movedPos = player2.pos;

			// 足下のﾌﾞﾛｯｸのﾁｪｯｸ
			movedOffset.y = movedPos.y + player2.hitPosE.y;

			movedOffset2 = movedOffset;							// 左下
			movedOffset2.x = movedPos.x - player2.hitPosS.x;

			movedOffset3 = movedOffset;							// 右下
			movedOffset3.x = movedPos.x + player2.hitPosE.x - 1;

			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
				player2.pos = movedPos;
			}
			else {
				tmpPos = MapPosToMoveIndex(movedOffset, head2Flag, player2.velocity);
				/*tmpIndex = MapPosToIndex(movedOffset);
				// movedOffset.y /32;
				tmpPos = MapIndexToPos(tmpIndex);
				// (movedOffset.y / 32) * 32
				*/
				player2.pos.y = tmpPos.y - player2.hitPosE.y;
				player2.velocity.y = 0;
				player2.jumpFlag = false;
			}
			movedPos = player2.pos;
		}

		if (!player2.jumpFlag) {
			if (trgKey[P2_A]) {
				player2.jumpFlag = true;
				player2.velocity.y = INIT_VELOCITY;
			}
		}

		// ｼｮｯﾄ
		player2.imgLockCnt++;
		/*if (player2.imgLockCnt < 60) {
		shotStatus2 = 1;
		}*/
		if (trgKey[P2_SHOT]) {
			player2.shotFlag = true;
			player2.imgLockCnt = 0;
			XY shotPos = player2.pos;
			shotPos.y += 8;
			if (player2.jumpFlag) {
				shotPos.y -= 8;
			}
			if (player2.moveDir == DIR_RIGHT) {
				shotPos.x += player2.offsetSize.x;
			}
			else if (player2.moveDir == DIR_LEFT) {
				shotPos.x -= player2.offsetSize.x;
			}
			Shot(shotPos, player2.moveDir, SHOT_C_MAGENTA);
		}
	}

	// ｱﾆﾒｰｼｮﾝ用ｶｳﾝﾀ

	if (player2.animCnt > 300) {
		player2.animCnt = 0;
	}

	// スクロール
	/*int addX = abs(player2oldPos.x - player2.pos.x);
	if (player2.pos.x - mapPos.x < (SCREEN_SIZE_X / 2)) {
		mapPos.x -= addX;
	}
	if (player2.pos.x - mapPos.x >(SCREEN_SIZE_X / 2)) {
		mapPos.x += addX;
	}*/

	XY tmpMapPos = GetMapPos();
	CHARACTER tmp = GetPlayer();
	if (abs(player2.pos.x - tmp.pos.x) < SCREEN_SIZE_X - 48 * 2) {
		MapMove(player2.pos, player2oldPos);
	}
}

void Player2Draw(void)
{
	XY tmpMapPos = GetMapPos();
	if (player2.flag) {
		int shotStatus2 = (player2.imgLockCnt < 30) ? 1 : 0;
		/*if(player.imgLockCnt < 30){
		shotStatus = 1;
		}
		else {
		shotStatus = 1;
		}*/
		if (player2.imgLockCnt > 30) {
			player2.damageFlag = false;
		}
		int img = p2StopImage[shotStatus2];
		if (player2.runFlag) img = p2RunImage[shotStatus2][player2.animCnt / 10 % 4];
		if (player2.jumpFlag) img = p2JumpImage[shotStatus2];
		if (player2.damageFlag) img = p2DamageImage;

		if (player2.moveDir == DIR_RIGHT) {
			DrawGraph(player2.pos.x - player2.offsetSize.x - tmpMapPos.x, player2.pos.y - player2.offsetSize.y - tmpMapPos.y, img, true);
		}

		if (player2.moveDir == DIR_LEFT) {
			DrawTurnGraph(player2.pos.x - player2.offsetSize.x - tmpMapPos.x, player2.pos.y - player2.offsetSize.y - tmpMapPos.y, img, true);
		}

		// ﾌﾟﾚｲﾔｰﾗｲﾌ
		for (int i = 0; i < player2.life * 2; i++) {
			DrawGraph(MAP_CHIP_SIZE_X * 20, (MAP_CHIP_SIZE_Y * 6) - (i * 8), p2lifeImage, false);
		}

		// デバッグ用
		DrawBox(player2.pos.x - player2.offsetSize.x - tmpMapPos.x, player2.pos.y - player2.offsetSize.y - tmpMapPos.y, player2.pos.x - tmpMapPos.x, player2.pos.y - tmpMapPos.y, 0xff0000, false);
		DrawBox(player2.pos.x - tmpMapPos.x, player2.pos.y - player2.offsetSize.y - tmpMapPos.y, player2.pos.x + player2.offsetSize.x - tmpMapPos.x, player2.pos.y - tmpMapPos.y, 0xff0000, false);
		DrawBox(player2.pos.x - player2.offsetSize.x - tmpMapPos.x, player2.pos.y - tmpMapPos.y, player2.pos.x - tmpMapPos.x, player2.pos.y + player2.offsetSize.y - tmpMapPos.y, 0xff0000, false);
		DrawBox(player2.pos.x - tmpMapPos.x, player2.pos.y - tmpMapPos.y, player2.pos.x + player2.offsetSize.x - tmpMapPos.x, player2.pos.y + player2.offsetSize.y - tmpMapPos.y, 0xff0000, false);

		DrawBox(player2.pos.x - player2.hitPosS.x - tmpMapPos.x, player2.pos.y - player2.hitPosS.y - tmpMapPos.y, player2.pos.x + player2.hitPosE.x - tmpMapPos.x, player2.pos.y + player2.hitPosE.y - tmpMapPos.y, 0x00ff00, false);
	}
	DrawFormatString(0, 30, 0x000000, "player2Pos: %d , %d", player2.pos.x, player2.pos.y);
}

bool Player2HitCheck(XY pos, XY size)
{
	if (player2.flag) {
		if ((pos.x - size.x / 2 < player2.pos.x + player2.hitPosE.x)		//aの左とplayer2の右
			&& (pos.x + size.x / 2 > player2.pos.x - player2.hitPosS.x)		//aの右とplayer2の左
			&& (pos.y - size.y / 2 < player2.pos.y + player2.hitPosE.y)		//aの上とplayer2の下
			&& (pos.y + size.y / 2 > player2.pos.y - player2.hitPosS.y)		//aの下とplayer2の上) 
			) {
			player2.life--;
			player2.damageFlag = true;
			player2.imgLockCnt = 0;
			if (player2.life <= 0) {
				player2.flag = 0;
				TobichiriGenerator(player2.pos, EFFECT_C_MAGENTA);
				for (int i = 0; i < SHOT_MAX; i++) {
					DeleteShot(i);
				}
			}
			return true;
		}
	}
	return false;
}

CHARACTER GetPlayer2(void) {
	return player2;
}

void AImove(void) {
	CHARACTER tmp = GetPlayer();
	bool player2Moved = false;

	XY movedPos = player2.pos;
	XY movedOffset = movedPos;
	XY movedOffset2 = movedPos;
	XY movedOffset3 = movedPos;
	XY tmpIndex;
	XY tmpPos;

	XY player2oldPos = player2.pos;

	player2.imgLockCnt++;
	player2.runFlag = false;
	player2.jumpFlag = true;
	head2Flag = false;

	// AI 向き変更
	if (player2.pos.x < tmp.pos.x)
	{
		player2.moveDir = DIR_RIGHT;
	}
	else
	{
		player2.moveDir = DIR_LEFT;
	}

	// ﾌﾟﾚｲﾔｰの移動
	if (player2.flag) {
		if (player2.pos.x > tmp.pos.x && player2.pos.x  < tmp.pos.x + 400) {
			player2.moveDir = DIR_RIGHT;
			player2.velocity.x += ACC_RUN * 1;	// 速度の更新
			player2Moved = true;
			player2.runFlag = true;
			if (player2.velocity.x > VELOCITY_X_MAX) {
				player2.velocity.x = VELOCITY_X_MAX;
			}
		}

		if (newKey[P2_LEFT]) {
			player2.moveDir = DIR_LEFT;
			player2.velocity.x -= ACC_RUN * 1;	// 速度の更新
			player2Moved = true;
			player2.runFlag = true;
			if (player2.velocity.x < -VELOCITY_X_MAX) {
				player2.velocity.x = -VELOCITY_X_MAX;
			}
		}
		if (!player2Moved) {
			if (player2.velocity.x > 0) {
				player2.velocity.x -= ACC_STOP;
			}
			if (player2.velocity.x < 0) {
				player2.velocity.x += ACC_STOP;
			}
		}


		// 旧移動制限
		XY tmpMapPos = GetMapPos();
		if (player2.velocity.x != 0) {
			if (movedPos.x < tmpMapPos.x + 48)
			{
				player2.velocity.x = 0;
				movedPos.x = tmpMapPos.x + 48;
			}
			if (movedPos.x > tmpMapPos.x + SCREEN_SIZE_X - 48) {
				player2.velocity.x = 0;
				movedPos.x = tmpMapPos.x + SCREEN_SIZE_X - 48;
			}
		}

		/*movedPos = player2.pos;
		movedOffset = movedPos;*/
		player2.animCnt++;

		movedPos.x += player2.velocity.x * 1;	// 距離の更新

		if (player2.velocity.x > 0) {
			movedOffset.x = movedPos.x + player2.hitPosE.x;
		}
		if (player2.velocity.x < 0) {
			movedOffset.x = movedPos.x - player2.hitPosS.x;
		}
		movedOffset2 = movedOffset;
		movedOffset2.y = movedPos.y - player2.hitPosS.y;
		movedOffset3 = movedOffset;
		movedOffset3.y = movedPos.y + player2.hitPosE.y - 1;

		if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
			player2.pos = movedPos;
		}
		else {
			tmpPos = MapPosToMoveIndex(movedOffset, player2.jumpFlag, player2.velocity);
			/*tmpIndex = MapPosToIndex(movedOffset);
			//movedOffset.y /32;*/
			if (player2.velocity.x > 0) {			// 右
				/*tmpPos = MapIndexToPos(tmpIndex);
				//(movedOffset.y / 32) * 32;*/

				player2.pos.x = tmpPos.x - player2.hitPosE.x;
			}
			if (player2.velocity.x < 0) {			// 左
				/*tmpIndex.x++;
				tmpPos = MapIndexToPos(tmpIndex);
				//(movedOffset.y / 32) * 32;*/

				player2.pos.x = tmpPos.x + player2.hitPosS.x;
			}
			movedPos = player2.pos;
			player2.velocity.x = 0;
		}


		// ｼﾞｬﾝﾌﾟ
		if (player2.jumpFlag) {
			movedPos = player2.pos;
			movedOffset = movedPos;

			movedPos.y -= player2.velocity.y * SECOND_PER_FRAME;
			player2.velocity.y -= ACC_G * SECOND_PER_FRAME;

			// 頭上のﾌﾞﾛｯｸのﾁｪｯｸ
			movedOffset.y = movedPos.y - player2.hitPosS.y;
			movedOffset2 = movedOffset;							// 左上
			movedOffset2.x = movedPos.x - player2.hitPosS.x;
			movedOffset3 = movedOffset;							// 右上
			movedOffset3.x = movedPos.x + player2.hitPosE.x - 1;

			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
				player2.pos = movedPos;
			}
			else {
				head2Flag = true;
				tmpPos = MapPosToMoveIndex(movedOffset, head2Flag, player2.velocity);
				/*tmpIndex = MapPosToIndex(movedOffset);
				// movedOffset.y /32;
				tmpIndex.y++;
				tmpPos = MapIndexToPos(tmpIndex);
				// (movedOffset.y / 32) * 32*/
				player2.pos.y = tmpPos.y + player2.hitPosS.y;
				player2.velocity.y *= -1;
			}
			movedPos = player2.pos;

			// 足下のﾌﾞﾛｯｸのﾁｪｯｸ
			movedOffset.y = movedPos.y + player2.hitPosE.y;

			movedOffset2 = movedOffset;							// 左下
			movedOffset2.x = movedPos.x - player2.hitPosS.x;

			movedOffset3 = movedOffset;							// 右下
			movedOffset3.x = movedPos.x + player2.hitPosE.x - 1;

			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
				player2.pos = movedPos;
			}
			else {
				tmpPos = MapPosToMoveIndex(movedOffset, head2Flag, player2.velocity);
				/*tmpIndex = MapPosToIndex(movedOffset);
				// movedOffset.y /32;
				tmpPos = MapIndexToPos(tmpIndex);
				// (movedOffset.y / 32) * 32
				*/
				player2.pos.y = tmpPos.y - player2.hitPosE.y;
				player2.velocity.y = 0;
				player2.jumpFlag = false;
			}
			movedPos = player2.pos;
		}

		if (!player2.jumpFlag) {
			if (trgKey[P2_A]) {
				player2.jumpFlag = true;
				player2.velocity.y = INIT_VELOCITY;
			}
		}

		// ｼｮｯﾄ
		player2.imgLockCnt++;
		/*if (player2.imgLockCnt < 60) {
		shotStatus2 = 1;
		}*/
		if (player2.pos.x > tmp.pos.x && player2.pos.x  > tmp.pos.x + 400) {
			player2.shotFlag = true;
			player2.imgLockCnt = 0;
			XY shotPos = player2.pos;
			shotPos.y += 8;
			if (player2.jumpFlag) {
				shotPos.y -= 8;
			}
			if (player2.moveDir == DIR_RIGHT) {
				shotPos.x += player2.offsetSize.x;
			}
			else if (player2.moveDir == DIR_LEFT) {
				shotPos.x -= player2.offsetSize.x;
			}
			Shot(shotPos, player2.moveDir, SHOT_C_MAGENTA);
		}
	}

	// ｱﾆﾒｰｼｮﾝ用ｶｳﾝﾀ

	if (player2.animCnt > 300) {
		player2.animCnt = 0;
	}

	// スクロール
	/*int addX = abs(player2oldPos.x - player2.pos.x);
	if (player2.pos.x - mapPos.x < (SCREEN_SIZE_X / 2)) {
		mapPos.x -= addX;
	}
	if (player2.pos.x - mapPos.x >(SCREEN_SIZE_X / 2)) {
		mapPos.x += addX;
	}*/

	XY tmpMapPos = GetMapPos();
	
	if (abs(player2.pos.x - tmp.pos.x) < SCREEN_SIZE_X - 48 * 2) {
		MapMove(player2.pos, player2oldPos);
	}
}