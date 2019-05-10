#include <DxLib.h>
#include "main.h"
#include "player.h"
#include "keycheck.h"
#include "stage.h"

CHARACTER player;
int player1[4];

int headFlag;
int jumpCnt;
int Gflag;
int jumpFrame;

XY movedPos = player.pos;
// offsetは今のﾌﾟﾚｲﾔｰの1ﾏｽあとの座標
XY movedOffset = movedPos;
XY movedOffset2 = movedPos;
XY movedOffset3 = movedPos;



void PlayerSysInit(void) 
{
	LoadDivGraph("png/プレイヤー１サイズ調整済み.png", 4, 4, 1, 48, 48, player1);
}

void PlayerInit(void)
{
	player.moveDir = DIR_RIGHT;
	player.pos = { 2 * CHIP_SIZE_X - PLAYER_SIZE_X / 2, 18 * CHIP_SIZE_Y };
	//player.pos = { 4 * CHIP_SIZE_X - PLAYER_SIZE_X / 2, 3 * CHIP_SIZE_Y };
	player.size = { PLAYER_SIZE_X, PLAYER_SIZE_Y };
	player.offsetSize = { player.size.x / 2, player.size.y / 2};
	player.hitPosS = { 13,  24 };									// ﾌﾟﾚｲﾔｰの左上
	player.hitPosE = { 13,  24 };									// ﾌﾟﾚｲﾔｰの右下
	player.velocity = { 0,0 };
	player.flag = true;
	player.animCnt = 0;
	jumpFrame = 15;
	jumpCnt = 0;

}

void PlayerUpdate(void)
{
	bool playerMoved = false;
	XY tmpPos;
	XY playeroldPos = player.pos;
	movedPos = player.pos;

	// offsetは今のﾌﾟﾚｲﾔｰの1ﾏｽあとの座標
	// offsetは1ﾙｰﾌﾟ毎に初期化

	movedOffset = movedPos;
	movedOffset2 = movedPos;
	movedOffset3 = movedPos;

	player.runFlag = false;
	headFlag = false;
	Gflag = true;

	// ﾌﾟﾚｲﾔｰの移動
	if (player.flag) {
		if (newKey[P1_RIGHT]) {								// 右
			player.moveDir = DIR_RIGHT;
			player.velocity.x += ACC_RUN;					// 速度の更新
			playerMoved = true;
			player.runFlag = true;
			if (player.velocity.x > VELOCITY_X_MAX) {
				player.velocity.x = VELOCITY_X_MAX;
			}
		}

		if (newKey[P1_LEFT]) {								// 左			
			player.moveDir = DIR_LEFT;
			player.velocity.x -= ACC_RUN * 1;				// 速度の更新
			playerMoved = true;
			player.runFlag = true;
			if (player.velocity.x < -VELOCITY_X_MAX) {	
				player.velocity.x = -VELOCITY_X_MAX;		
			}
		}		
		// 止まった時の減速処理
		if (!player.runFlag) {								
			if (player.velocity.x > 0) {
				player.velocity.x -= ACC_STOP;
				if (player.velocity.x < 0) {
					player.velocity.x = 0;
				}
			}
			if (player.velocity.x < 0) {
				player.velocity.x += ACC_STOP;
				if (player.velocity.x > 0) {
					player.velocity.x = 0;
				}
			}
		}
		
		// 移動制限(ﾏｯﾌﾟをｽｸﾛｰﾙさせるときのみ使用)
		/*XY tmpMapPos = GetMapPos();
		if (player.velocity.x != 0) {
			if (movedPos.x < tmpMapPos.x + 48)
			{
				player.velocity.x = 0;
				movedPos.x = tmpMapPos.x + 48;
			}

			if (movedPos.x > tmpMapPos.x + SCREEN_SIZE_X - 48) {
				player.velocity.x = 0;
				movedPos.x = tmpMapPos.x + SCREEN_SIZE_X - 48;
			}
		}*/

		movedPos.x += player.velocity.x * 1;	// 距離の更新

		if (player.velocity.x > 0) {							// 右
			movedOffset.x = movedPos.x + player.hitPosE.x;
		}
		if (player.velocity.x < 0) {							// 左
			movedOffset.x = movedPos.x - player.hitPosS.x;
		}

		// 左下
		movedOffset2 = movedOffset;
		movedOffset2.y = movedPos.y - player.hitPosS.y;
		// 右下
		movedOffset3 = movedOffset;
		movedOffset3.y = movedPos.y + player.hitPosE.y - 1;			// ‐1は調整用
		/*SetOffset();
		movedOffset3.x -= 1;*/

		if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
			player.pos = movedPos;													// 
		}
		else {
			tmpPos = MapPosToMoveIndex(movedOffset, player.jumpFlag, player.velocity);
			if (player.velocity.x > 0) {			// 右
				player.pos.x = tmpPos.x - player.hitPosE.x;
			}
			if (player.velocity.x < 0) {			// 左
				player.pos.x = tmpPos.x + player.hitPosS.x;
			}
			movedPos = player.pos;
			player.velocity.x = 0;
		}
		
		// 重力
		movedPos = player.pos;
		

		movedPos.y -= player.velocity.y * SECOND_PER_FRAME;
		player.velocity.y -= ACC_G * SECOND_PER_FRAME;

		// ﾌﾞﾛｯｸのﾁｪｯｸ
		// 頭上のﾌﾞﾛｯｸのﾁｪｯｸ
		movedOffset = movedPos;
		movedOffset.y = movedPos.y - player.hitPosS.y;
		movedOffset2 = movedOffset;							// 左上
		movedOffset2.x = movedPos.x - player.hitPosS.x;
		movedOffset3 = movedOffset;							// 右上
		movedOffset3.x = movedPos.x + player.hitPosE.x - 1;
		/*SetOffset();
		movedOffset3.x -= 1;*/


		if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
			player.pos = movedPos;
		}
		else {
			headFlag = true;
			tmpPos = MapPosToMoveIndex(movedOffset, headFlag, player.velocity);
			player.pos.y = tmpPos.y + player.hitPosS.y;
			player.velocity.y -= 1;
			player.velocity.y *= -1;
		}
		movedPos = player.pos;

		// 足下のﾌﾞﾛｯｸのﾁｪｯｸ

		movedOffset.y = movedPos.y + player.hitPosE.y;
		movedOffset2 = movedOffset;							// 左上
		movedOffset2.x = movedPos.x - player.hitPosS.x;
		movedOffset3 = movedOffset;							// 右上
		movedOffset3.x = movedPos.x + player.hitPosE.x - 1;
		/*movedOffset.y = movedPos.y + player.hitPosE.y;
		SetOffset();;
		movedOffset3.x -= 1;*/
		if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
			player.pos = movedPos;
		}
		else {
			tmpPos = MapPosToMoveIndex(movedOffset, headFlag, player.velocity);
			player.pos.y = tmpPos.y - player.hitPosE.y;
			player.velocity.y = 0;
			player.jumpFlag = false;
		}
		movedPos = player.pos;
	
		//　ｼﾞｬﾝﾌﾟ
		movedOffset.y = movedPos.y - player.hitPosS.y - 1;
	/*	movedOffset2 = movedOffset;							// 左上
		movedOffset2.x = movedPos.x - player.hitPosS.x;
		movedOffset3 = movedOffset;							// 右上
		movedOffset3.x = movedPos.x + player.hitPosE.x - 1;*/
		SetOffset();;
		movedOffset3.x -= 1;

		if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
			if (!player.jumpFlag) {
				if (newKey[P1_A]) {
					jumpCnt++;
					if (jumpCnt < jumpFrame) {
						player.velocity.y = 25;
					}
					if (headFlag) {
						jumpCnt = jumpFrame;
					}
				}
				else {
					jumpCnt = 0;
					player.jumpFlag = true;
				}
			}
		}
		else {
			jumpCnt = 15;
		}
		movedPos = player.pos;

		// ﾄﾗｯﾌﾟの判定
		// ﾃﾚﾎﾟｰﾄ
		if (!TelIsPass(player.pos)) {
			player.pos = { SCREEN_SIZE_X , 10000 };
		}

		// 針ブロック
		if (!NeedleIsPass(player.pos)) {
			player.flag = false;
		}

		// ｼﾞｬﾝﾌﾟﾌﾞﾛｯｸ
		movedOffset.y = movedPos.y - player.hitPosS.y - 1;	// 頭上
		movedOffset2 = movedOffset;							// 左上
		movedOffset2.x = movedPos.x - player.hitPosS.x ;
		movedOffset3 = movedOffset;							// 右上
		movedOffset3.x = movedPos.x + player.hitPosE.x - 1;
		if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {			// 1ﾏｽのところではジャンプしない
			movedOffset.y = movedPos.y + player.hitPosE.y;
			if (!JumpIsPass(movedOffset)) {
				player.velocity.y = 80;
			}
		}


		// 画面外にﾌﾟﾚｲﾔｰが出たら
		if (player.pos.x > SCREEN_SIZE_X || player.pos.y > SCREEN_SIZE_Y)
		{
			player.flag = false;
		}
		
		
	}
	else {	// ﾌﾟﾚｰﾔｰが死んだとき
		if (trgKey[ENTER]) {
			PlayerInit();
		}
	}
	player.animCnt++;
}

void PlayerDraw(void)
{
	XY tmpMapPos = GetMapPos();
	
	if (player.flag) {
		if (player.runFlag) {
			if (player.moveDir == DIR_RIGHT) {
				DrawGraph(player.pos.x - tmpMapPos.x - player.offsetSize.x, player.pos.y - tmpMapPos.y - player.offsetSize.y, player1[player.animCnt / 15 % 4], true);
			}
			if (player.moveDir == DIR_LEFT) {
				DrawTurnGraph(player.pos.x - tmpMapPos.x - player.offsetSize.x, player.pos.y - tmpMapPos.y - player.offsetSize.y, player1[player.animCnt / 15 % 4], true);
			}
		}
		else {
			if (player.moveDir == DIR_RIGHT) {
				DrawGraph(player.pos.x - tmpMapPos.x - player.offsetSize.x, player.pos.y - tmpMapPos.y - player.offsetSize.y, player1[0], true);
			}
			if (player.moveDir == DIR_LEFT) {
				DrawTurnGraph(player.pos.x - tmpMapPos.x - player.offsetSize.x, player.pos.y - tmpMapPos.y - player.offsetSize.y, player1[0], true);
			}
		}
	}
	DrawFormatString(0, 48, 0x000000, "playerPos: %d , %d", player.pos.x, player.pos.y);
	DrawCircle(player.pos.x, player.pos.y, 5,  0xff0000, true);
}

CHARACTER GetPlayer(void) {
	return player;
}

bool PlayerHitCheck(XY pos, XY size, int type)
{
	//矩形と矩形
	if (player.flag) {
		if (type == 0) {
			if ((pos.x < player.pos.x + player.hitPosE.x)
				&& (pos.x + size.x > player.pos.x - player.hitPosS.x)
				&& (pos.y < player.pos.y + player.hitPosE.y)
				&& (pos.y + size.y > player.pos.y - player.hitPosS.y)
				) {
				player.flag = false;
				return true;
			}
		}
	// 線と矩形
		if (type == 1) {
			if ((pos.x + size.x / 2 < player.pos.x + player.hitPosE.x)
				&& (pos.x + size.x / 2 > player.pos.x - player.hitPosS.x)
				&& (pos.y < player.pos.y + player.hitPosE.y)
				&& (pos.y + size.y > player.pos.y - player.hitPosS.y)
				) {
				player.flag = false;
				return true;
			}
		}
	}
	return false;
}

void SetOffset(void) {									// ﾌﾟﾚｲﾔｰの右端と左端のｵﾌｾｯﾄ 
	movedOffset2 = movedOffset;							// 左上
	movedOffset2.x = movedPos.x - player.hitPosS.x;
	movedOffset3 = movedOffset;							// 右上
	movedOffset3.x = movedPos.x + player.hitPosE.x;
}