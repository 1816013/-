#include <DxLib.h>
#include "main.h"
#include "player.h"
#include "keycheck.h"
#include "stage.h"
#include "effect.h"



CHARACTER player[PLAYER_MAX];
int pImage[PLAYER_MAX][PLAYER_IMAGE_MAX];
int pIcon[PLAYER_MAX];
int pIconOK;
int jumpCnt[PLAYER_MAX];
int Gflag;
int jumpFrame[PLAYER_MAX];
int playerMax;
int selectCnt;
int goalCnt;
int cycleCnt[3];

XY savePos[PLAYER_MAX];	

XY movedPos;
// offsetは今のﾌﾟﾚｲﾔｰの1ﾏｽあとの座標
XY movedOffset;
XY movedOffset2;
XY movedOffset3;
int offsetType;					

void PlayerSysInit(void) 
{
	LoadDivGraph("png/プレイヤー1.png", 4, 4, 1, 48, 48, pImage[0]);
	LoadDivGraph("png/プレイヤー2.png", 4, 4, 1, 48, 48, pImage[1]);
	LoadDivGraph("png/プレイヤー3.png", 4, 4, 1, 48, 48, pImage[2]);
	LoadDivGraph("png/プレイヤー4.png", 4, 4, 1, 48, 48, pImage[3]);
	pIcon[0] = LoadGraph("png/ネーム1.png");
	pIcon[1] = LoadGraph("png/ネーム2.png");
	pIcon[2] = LoadGraph("png/ネーム3.png");
	pIcon[3] = LoadGraph("png/ネーム4.png");
	pIconOK = LoadGraph("png/ok.png");
	for (int i = 0; i < PLAYER_MAX; i++) {
		savePos[i] = { 2 * CHIP_SIZE_X - PLAYER_SIZE_X / 2, 19 * CHIP_SIZE_Y - PLAYER_SIZE_Y / 2 };		// プレイヤーリス地初期化
	}
}

void PlayerInit(void)
{
	for (int i = 0; i < PLAYER_MAX; i++) {
		player[i].moveDir = DIR_RIGHT;
		//player.pos = { 4 * CHIP_SIZE_X - PLAYER_SIZE_X / 2, 3 * CHIP_SIZE_Y };
		player[i].size = { PLAYER_SIZE_X, PLAYER_SIZE_Y };
		player[i].offsetSize = { player[i].size.x / 2, player[i].size.y / 2 };
		player[i].hitPosS = { 13,  24 };									// ﾌﾟﾚｲﾔｰの左上
		player[i].hitPosE = { 13,  24 };									// ﾌﾟﾚｲﾔｰの右下
		player[i].velocity = { 0,0 };
		player[i].animCnt = 0;
		jumpFrame[i] = 15;
		jumpCnt[i] = 0;
		player[i].flag = false;
		player[i].goalFlag = false;
		player[i].jyuni = 0;
		player[i].selectFlag = false;
		player[i].cycleType = 0;
	}
	playerMax = GetPlayerCnt();
	for (int j = 0; j < playerMax; j++) {
		player[j].pos = savePos[j];
		player[j].flag = true;
	}
	goalCnt = 0;
}

void PlayerUpdate(void)
{
	playerMax = GetPlayerCnt();
	for (int i = 0; i < playerMax; i++) {
		//　1ﾙｰﾌﾟ毎に初期化する変数
		XY tmpPos;
		//XY playeroldPos = player.pos;
		movedPos = player[i].pos;

		// offsetは今のﾌﾟﾚｲﾔｰの1ﾏｽあとの座標
		movedOffset = movedPos;
		movedOffset2 = movedPos;
		movedOffset3 = movedPos;

		player[i].runFlag = false;
		player[i].headFlag = false;
		Gflag = true;

		// ﾌﾟﾚｲﾔｰの移動	(キー操作)
		if (player[i].flag) {
			if (PlayerKeyCheck(i)) {
				if (player[i].moveDir == DIR_RIGHT) {								// 右
					player[i].velocity.x += ACC_RUN;					// 速度の更新
					player[i].runFlag = true;
					if (player[i].velocity.x > VELOCITY_X_MAX) {
						player[i].velocity.x = VELOCITY_X_MAX;
					}
				}

				if (player[i].moveDir == DIR_LEFT) {								// 左			
					player[i].velocity.x -= ACC_RUN * 1;				// 速度の更新
					player[i].runFlag = true;
					if (player[i].velocity.x < -VELOCITY_X_MAX) {
						player[i].velocity.x = -VELOCITY_X_MAX;
					}
				}
			}
			// 止まった時の減速処理
			if (!player[i].runFlag) {
				if (player[i].velocity.x > 0) {
					player[i].velocity.x -= ACC_STOP;
					if (player[i].velocity.x < 0) {
						player[i].velocity.x = 0;
					}
				}
				if (player[i].velocity.x < 0) {
					player[i].velocity.x += ACC_STOP;
					if (player[i].velocity.x > 0) {
						player[i].velocity.x = 0;
					}
				}
			}

			// ﾌﾟﾚｲﾔｰの移動(左右)
			movedPos.x += player[i].velocity.x * 1;	// 距離の更新

			//if (player.velocity.x > 0) {							// 右
			//	movedOffset.x = movedPos.x + player.hitPosE.x;
			//}
			//if (player.velocity.x < 0) {							// 左
			//	movedOffset.x = movedPos.x - player.hitPosS.x;
			//}
			////ﾌﾟﾚｲﾔｰの頭上の中心のｵﾌｾｯﾄ
			//movedOffset2 = movedOffset;
			//movedOffset2.y = movedPos.y - player.hitPosS.y;
			////ﾌﾟﾚｲﾔｰの足下の中心のｵﾌｾｯﾄ
			//movedOffset3 = movedOffset;
			//movedOffset3.y = movedPos.y + player.hitPosE.y - 1;			// ‐1は調整

			SetOffset(OFFSET_LEFT_RIGHT);

			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {		// movedOffsetはﾌﾟﾚｲﾔｰの中心movedOffset2はﾌﾟﾚｲﾔｰの頭上の中心movedOffset3はﾌﾟﾚｲﾔｰの足下の中心
				player[i].pos = movedPos;													// 
			}
			else {
				tmpPos = MapPosToMoveIndex(movedOffset, player[i].jumpFlag, player[i].velocity);
				if (player[i].velocity.x > 0) {			// 右
					player[i].pos.x = tmpPos.x - player[i].hitPosE.x - 2;
				}
				if (player[i].velocity.x < 0) {			// 左
					player[i].pos.x = tmpPos.x + player[i].hitPosS.x + 2;
				}
				movedPos = player[i].pos;
				player[i].velocity.x = 0;
			}

			// 重力
			movedPos = player[i].pos;
			movedPos.y -= player[i].velocity.y * SECOND_PER_FRAME;
			player[i].velocity.y -= ACC_G * SECOND_PER_FRAME;

			// ﾌﾞﾛｯｸのﾁｪｯｸ
			// 頭上のﾌﾞﾛｯｸのﾁｪｯｸ

			//movedOffset = movedPos;
			//movedOffset.y = movedPos.y - player.hitPosS.y;		
			//movedOffset2 = movedOffset;							// 左上
			//movedOffset2.x = movedPos.x - player.hitPosS.x;
			//movedOffset3 = movedOffset;							// 右上
			//movedOffset3.x = movedPos.x + player.hitPosE.x - 1;
			SetOffset(OFFSET_HEAD);

			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
				player[i].pos = movedPos;
			}
			else {
				player[i].headFlag = true; 
				tmpPos = MapPosToMoveIndex(movedOffset, player[i].headFlag, player[i].velocity);
				player[i].pos.y = tmpPos.y + player[i].hitPosS.y;
				player[i].velocity.y -= 1;
				player[i].velocity.y *= -1;
			}
			movedPos = player[i].pos;


			// 足下のﾌﾞﾛｯｸのﾁｪｯｸ

			//movedOffset = movedPos;
			//movedOffset.y = movedPos.y + player.hitPosE.y;
			//movedOffset.y = movedPos.y + player.hitPosE.y;
			//movedOffset2 = movedOffset;							// 左上
			//movedOffset2.x = movedPos.x - player.hitPosS.x;
			//movedOffset3 = movedOffset;							// 右上
			//movedOffset3.x = movedPos.x + player.hitPosE.x - 1
			SetOffset(OFFSET_FOOT);
			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
				player[i].pos = movedPos;
			}
			else {
				tmpPos = MapPosToMoveIndex(movedOffset, player[i].headFlag, player[i].velocity);
				player[i].pos.y = tmpPos.y - player[i].hitPosE.y;
				player[i].velocity.y = 0;
				player[i].jumpFlag = false;
			}
			movedPos = player[i].pos;

			//　ｼﾞｬﾝﾌﾟ
		/*	movedOffset2 = movedOffset;							// 左上
			movedOffset2.x = movedPos.x - player.hitPosS.x;
			movedOffset3 = movedOffset;							// 右上
			movedOffset3.x = movedPos.x + player.hitPosE.x - 1;*/

			// 1ﾏｽのところではジャンプしない
			SetOffset(OFFSET_NOTJUMP);
			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
				if (!player[i].jumpFlag) {
					if (PlayerJumpKeyCheck(i)) {
						jumpCnt[i]++;
						if (jumpCnt[i] < jumpFrame[i]) {
							player[i].velocity.y = 25;
						}
						if (player[i].headFlag) {
							jumpCnt[i] = jumpFrame[i];
						}
					}
					else {
						jumpCnt[i] = 0;
						player[i].jumpFlag = true;
					}
				}
			}
			else {
				jumpCnt[i] = 15;
			}
			movedPos = player[i].pos;

			// ﾄﾗｯﾌﾟの判定
			// ﾃﾚﾎﾟｰﾄ
			if (!TelIsPass(player[i].pos)) {
				player[i].pos = { 100000 , 0 };
			}

			// 針ブロック
			if (!NeedleIsPass(player[i].pos)) {
				TobichiriGenerator(player[i].pos);
				player[i].flag = false;
			}

			// ｼﾞｬﾝﾌﾟﾌﾞﾛｯｸ
			// 1ﾏｽのところではジャンプしない
			SetOffset(OFFSET_NOTJUMP);
			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
				movedOffset.y = movedPos.y + player[i].hitPosE.y;		// 足元
				if (!JumpIsPass(movedOffset)) {
					player[i].velocity.y = 80;
				}
			}
			movedPos = player[i].pos;

			// セーブ
			if (!SaveIsPass(player[i].pos)) {
				savePos[i] = player[i].pos;
			}

			if (!GoalIsPass(player[i].pos)) {
				player[i].pos = { 0 , 0 };
				player[i].flag = false;
				player[i].goalFlag = true;
				goalCnt++;
				player[i].jyuni = goalCnt;
			}

			// 画面外にﾌﾟﾚｲﾔｰが出たら
			if (player[i].pos.y > SCREEN_SIZE_Y)
			{
				player[i].flag = false;
			}

			if (trgKey[BACK]) {
				player[i].selectFlag = false;
			}

		}
		else {	// ﾌﾟﾚｰﾔｰが死んだとき
			if (!player[i].goalFlag) {
				player[i].pos = savePos[i];
				player[i].velocity = { 0,0 };
				if (trgKey[ENTER]) {
					player[i].flag = true;
				}
			}
		}
		player[i].animCnt++;
	}
	selectCnt = 0;
	for (int i = 0; i < playerMax; i++) {
		if (player[i].selectFlag) {
			selectCnt++;
		}
	}
	int max = 0;
	int num = 0;
	for (int j = 0; j < 3; j++) {
		if (cycleCnt[j] > max) {
			max = cycleCnt[j];
		}
	}
	int maxCnt = 0;
	for (int i = 0; i < playerMax; i++) {
		for (int j = 0; j < 3; j++) {
			if (cycleCnt[j] == max) {
				player[i].cycleType = j +1; 
			}
		//	player[i].cycleType = j + 1;
		}
	}
	
}

void PlayerDraw(void)
{
	for (int i = 0; i < PLAYER_MAX; i++) {
		XY tmpMapPos = GetMapPos();

		if (player[i].flag) {
			if (player[i].runFlag) {
				if (player[i].moveDir == DIR_RIGHT) {
					DrawGraph(player[i].pos.x - player[i].offsetSize.x, player[i].pos.y - player[i].offsetSize.y, pImage[i][player[i].animCnt / 15 % 4], true);
				}
				if (player[i].moveDir == DIR_LEFT) {
					DrawTurnGraph(player[i].pos.x  - player[i].offsetSize.x, player[i].pos.y - player[i].offsetSize.y, pImage[i][player[i].animCnt / 15 % 4], true);
				}
			}
			else {
				if (player[i].moveDir == DIR_RIGHT) {
					DrawGraph(player[i].pos.x - player[i].offsetSize.x, player[i].pos.y- player[i].offsetSize.y, pImage[i][0], true);
				}
				if (player[i].moveDir == DIR_LEFT) {
					DrawTurnGraph(player[i].pos.x - player[i].offsetSize.x, player[i].pos.y- player[i].offsetSize.y, pImage[i][0], true);
				}
			}
			DrawGraph(player[i].pos.x - player[i].offsetSize.x, player[i].pos.y - 60, pIcon[i], true);
			if (player[i].selectFlag) {
				DrawGraph(player[i].pos.x - player[i].offsetSize.x, player[i].pos.y - 84, pIconOK, false);
			}
		}
		DrawFormatString(0, 48, 0x000000, "playerPos 1: %d , %d", player[0].pos.x, player[0].pos.y);
		DrawFormatString(0, 60, 0x000000, "selectCnt: %d", selectCnt);
		for (int j = 0; j < 3; j++) {
			DrawFormatString(0, 72 + 12 * j, 0x000000, "cycleCnt: %d", cycleCnt[j]);
		}
		DrawFormatString(0, 120 + 12 * i, 0x000000, "playerselectFlag %d: %d", i + 1, player[i].selectFlag);
		//DrawCircle(player.pos.x, player.pos.y, 5,  0xff0000, true);selectCntplayer[i].selectFlag
	}
}

CHARACTER GetPlayer(int i) {
	return player[i];
}

bool PlayerHitCheck(XY pos, XY size, int type)
{
	for (int i = 0; i < PLAYER_MAX; i++) {
		if (player[i].flag) {
			//矩形と矩形(ﾌﾟﾚｲﾔｰ死亡)
			if (type == 0) {
				if ((pos.x < player[i].pos.x + player[i].hitPosE.x)
					&& (pos.x + size.x > player[i].pos.x - player[i].hitPosS.x)
					&& (pos.y < player[i].pos.y + player[i].hitPosE.y)
					&& (pos.y + size.y > player[i].pos.y - player[i].hitPosS.y)
					) {
					player[i].flag = false;
					TobichiriGenerator(player[i].pos);
					return true;
				}
			}
			// 縦線と矩形
			if (type == 1) {
				if ((pos.x + size.x / 2 < player[i].pos.x + player[i].hitPosE.x)
					&& (pos.x + size.x / 2 > player[i].pos.x - player[i].hitPosS.x)
					&& (pos.y < player[i].pos.y + player[i].hitPosE.y)
					&& (pos.y + size.y > player[i].pos.y - player[i].hitPosS.y)
					) {
					player[i].flag = false;
					TobichiriGenerator(player[i].pos);
					return true;
				}
			}
			// 横線と矩形
			if (type == 2) {
				if ((pos.x < player[i].pos.x + player[i].hitPosE.x)
					&& (pos.x + size.x / 2 > player[i].pos.x - player[i].hitPosS.x)
					&& (pos.y + size.y / 2 < player[i].pos.y + player[i].hitPosE.y)
					&& (pos.y + size.y / 2 > player[i].pos.y - player[i].hitPosS.y)
					) {
					player[i].flag = false;
					TobichiriGenerator(player[i].pos);
					return true;
				}
			}
			//矩形と矩形(セレクト用)
			if (type == 3 || type == 4 || type == 5) {
				if ((pos.x < player[i].pos.x + player[i].hitPosE.x)
					&& (pos.x + size.x > player[i].pos.x - player[i].hitPosS.x)
					&& (pos.y < player[i].pos.y + player[i].hitPosE.y)
					&& (pos.y + size.y > player[i].pos.y - player[i].hitPosS.y)
					) {
					if (trgKey[ENTER]) {
						if (!player[i].selectFlag) {
							if (type == 3) {
								cycleCnt[0]++;
							}
							if (type == 4) {
								cycleCnt[1]++;
							}
							if (type == 5) {
								cycleCnt[2]++;
							}

							player[i].selectFlag = true;
							
						}
					}
				}
			}
		}
	}
	return false;
}

void SetOffset(OFFSET_TYPE type) {			

	for (int i = 0; i < PLAYER_MAX; i++) {
		switch (type) {
		case OFFSET_LEFT_RIGHT:
			if (player[i].velocity.x > 0) {							// 右
				movedOffset.x = movedPos.x + player[i].hitPosE.x + 1;
			}
			if (player[i].velocity.x < 0) {							// 左
				movedOffset.x = movedPos.x - player[i].hitPosS.x -2 ;
			}
			//ﾌﾟﾚｲﾔｰの頭上の中心のｵﾌｾｯﾄ
			movedOffset2 = movedOffset;
			movedOffset2.y = movedPos.y - player[i].hitPosS.y;
			//ﾌﾟﾚｲﾔｰの足下の中心のｵﾌｾｯﾄ
			movedOffset3 = movedOffset;
			movedOffset3.y = movedPos.y + player[i].hitPosE.y - 1;			// ‐1は調整用
			break;
		case OFFSET_HEAD:
			movedOffset = movedPos;
			movedOffset.y = movedPos.y - player[i].hitPosS.y;
			movedOffset2 = movedOffset;							// 左上
			movedOffset2.x = movedPos.x - player[i].hitPosS.x;
			movedOffset3 = movedOffset;							// 右上
			movedOffset3.x = movedPos.x + player[i].hitPosE.x - 1;
			break;
		case OFFSET_FOOT:
			movedOffset = movedPos;
			movedOffset.y = movedPos.y + player[i].hitPosE.y;
			movedOffset2 = movedOffset;							// 左上
			movedOffset2.x = movedPos.x - player[i].hitPosS.x;
			movedOffset3 = movedOffset;							// 右上
			movedOffset3.x = movedPos.x + player[i].hitPosE.x - 1;
			break;
		case OFFSET_NOTJUMP:
			movedOffset = movedPos;
			movedOffset.y = movedPos.y - player[i].hitPosS.y - 1;
			movedOffset2 = movedOffset;							// 左上
			movedOffset2.x = movedPos.x - player[i].hitPosS.x - 1;
			movedOffset3 = movedOffset;							// 右上
			movedOffset3.x = movedPos.x + player[i].hitPosE.x - 1;
			break;
		default:
			AST();
			break;
		}
	}
	
}

bool PlayerKeyCheck(int i){
	switch (i) {
	case 0:
		if (newKey[P1_RIGHT]) {
			player[i].moveDir = DIR_RIGHT;
			return true;
		}

		if (newKey[P1_LEFT]) {
			player[i].moveDir = DIR_LEFT;
			return true;
		}
		break;
	case 1:
		if (newKey[P2_RIGHT]) {
			player[i].moveDir = DIR_RIGHT;
			return true;
		}

		if (newKey[P2_LEFT]) {
			player[i].moveDir = DIR_LEFT;
			return true;
		}
		break;
	case 2:
		if (newKey[P3_RIGHT]) {
			player[i].moveDir = DIR_RIGHT;
			return true;
		}

		if (newKey[P3_LEFT]) {
			player[i].moveDir = DIR_LEFT;
			return true;
		}
		break;
	case 3:
		if (newKey[P4_RIGHT]) {
			player[i].moveDir = DIR_RIGHT;
			return true;
		}

		if (newKey[P4_LEFT]) {
			player[i].moveDir = DIR_LEFT;
			return true;
		}
		break;
	}	
	return false;
}

bool PlayerJumpKeyCheck(int i) {
	switch (i) {
	case 0:
		if (newKey[P1_A]) {
			return true;
		}
		break;
	case 1:
		if (newKey[P2_A]) {
			return true;
		}
		break;
	case 2:
		if (newKey[P3_A]) {
			return true;
		}
		break;
	case 3:
		if (newKey[P4_A]) {
			return true;
		}
		break;
	}
	return false;
}
int GetSelectCnt()
{
	return selectCnt;
}