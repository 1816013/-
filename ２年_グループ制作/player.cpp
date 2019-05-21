#include <DxLib.h>
#include "main.h"
#include "player.h"
#include "keycheck.h"
#include "stage.h"
#include "effect.h"



CHARACTER player[PLAYER_MAX];
int pImage[PLAYER_MAX][4];

int jumpCnt[PLAYER_MAX];
int Gflag;
int jumpFrame[PLAYER_MAX];

bool saveFlag;

XY savePos[PLAYER_MAX];	

XY movedPos;
// offset�͍�����ڲ԰��1Ͻ���Ƃ̍��W
XY movedOffset;
XY movedOffset2;
XY movedOffset3;
int offsetType;					

void PlayerSysInit(void) 
{
	LoadDivGraph("png/�v���C���[1.png", 4, 4, 1, 48, 48, pImage[0]);
	LoadDivGraph("png/�v���C���[2.png", 4, 4, 1, 48, 48, pImage[1]);
	LoadDivGraph("png/�v���C���[3.png", 4, 4, 1, 48, 48, pImage[2]);
	LoadDivGraph("png/�v���C���[4.png", 4, 4, 1, 48, 48, pImage[3]);
	for (int i = 0; i < PLAYER_MAX; i++) {
		savePos[i] = { 13 * CHIP_SIZE_X - PLAYER_SIZE_X / 2, 18 * CHIP_SIZE_Y - PLAYER_SIZE_Y / 2 };		// �v���C���[���X�n������
	}
}

void PlayerInit(void)
{
	for (int i = 0; i < PLAYER_MAX; i++) {
		player[i].moveDir = DIR_RIGHT;
		player[i].pos = savePos[i];
		//player.pos = { 4 * CHIP_SIZE_X - PLAYER_SIZE_X / 2, 3 * CHIP_SIZE_Y };
		player[i].size = { PLAYER_SIZE_X, PLAYER_SIZE_Y };
		player[i].offsetSize = { player[i].size.x / 2, player[i].size.y / 2 };
		player[i].hitPosS = { 13,  24 };									// ��ڲ԰�̍���
		player[i].hitPosE = { 13,  24 };									// ��ڲ԰�̉E��
		player[i].velocity = { 0,0 };
		player[i].flag = true;
		player[i].animCnt = 0;
		jumpFrame[i] = 15;
		jumpCnt[i] = 0;
	}


}

void PlayerUpdate(void)
{
	for (int i = 0; i < PLAYER_MAX; i++) {
		//�@1ٰ�ߖ��ɏ���������ϐ�
		XY tmpPos;
		//XY playeroldPos = player.pos;
		movedPos = player[i].pos;

		// offset�͍�����ڲ԰��1Ͻ���Ƃ̍��W
		movedOffset = movedPos;
		movedOffset2 = movedPos;
		movedOffset3 = movedPos;

		player[i].runFlag = false;
		player[i].headFlag = false;
		Gflag = true;

		// ��ڲ԰�̈ړ�	(�L�[����)
		if (player[i].flag) {
			if (PlayerKeyCheck(i)) {
				if (player[i].moveDir == DIR_RIGHT) {								// �E
					player[i].velocity.x += ACC_RUN;					// ���x�̍X�V
					player[i].runFlag = true;
					if (player[i].velocity.x > VELOCITY_X_MAX) {
						player[i].velocity.x = VELOCITY_X_MAX;
					}
				}

				if (player[i].moveDir == DIR_LEFT) {								// ��			
					player[i].velocity.x -= ACC_RUN * 1;				// ���x�̍X�V
					player[i].runFlag = true;
					if (player[i].velocity.x < -VELOCITY_X_MAX) {
						player[i].velocity.x = -VELOCITY_X_MAX;
					}
				}
			}
			// �~�܂������̌�������
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

			// ��ڲ԰�̈ړ�(���E)
			movedPos.x += player[i].velocity.x * 1;	// �����̍X�V

			//if (player.velocity.x > 0) {							// �E
			//	movedOffset.x = movedPos.x + player.hitPosE.x;
			//}
			//if (player.velocity.x < 0) {							// ��
			//	movedOffset.x = movedPos.x - player.hitPosS.x;
			//}
			////��ڲ԰�̓���̒��S�̵̾��
			//movedOffset2 = movedOffset;
			//movedOffset2.y = movedPos.y - player.hitPosS.y;
			////��ڲ԰�̑����̒��S�̵̾��
			//movedOffset3 = movedOffset;
			//movedOffset3.y = movedPos.y + player.hitPosE.y - 1;			// �]1�͒���

			SetOffset(OFFSET_LEFT_RIGHT);

			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {		// movedOffset����ڲ԰�̒��SmovedOffset2����ڲ԰�̓���̒��SmovedOffset3����ڲ԰�̑����̒��S
				player[i].pos = movedPos;													// 
			}
			else {
				tmpPos = MapPosToMoveIndex(movedOffset, player[i].jumpFlag, player[i].velocity);
				if (player[i].velocity.x > 0) {			// �E
					player[i].pos.x = tmpPos.x - player[i].hitPosE.x - 1;
				}
				if (player[i].velocity.x < 0) {			// ��
					player[i].pos.x = tmpPos.x + player[i].hitPosS.x + 1;
				}
				movedPos = player[i].pos;
				player[i].velocity.x = 0;
			}

			// �d��
			movedPos = player[i].pos;
			movedPos.y -= player[i].velocity.y * SECOND_PER_FRAME;
			player[i].velocity.y -= ACC_G * SECOND_PER_FRAME;

			// ��ۯ�������
			// �������ۯ�������

			//movedOffset = movedPos;
			//movedOffset.y = movedPos.y - player.hitPosS.y;		
			//movedOffset2 = movedOffset;							// ����
			//movedOffset2.x = movedPos.x - player.hitPosS.x;
			//movedOffset3 = movedOffset;							// �E��
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


			// ��������ۯ�������

			//movedOffset = movedPos;
			//movedOffset.y = movedPos.y + player.hitPosE.y;
			//movedOffset.y = movedPos.y + player.hitPosE.y;
			//movedOffset2 = movedOffset;							// ����
			//movedOffset2.x = movedPos.x - player.hitPosS.x;
			//movedOffset3 = movedOffset;							// �E��
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

			//�@�ެ���
		/*	movedOffset2 = movedOffset;							// ����
			movedOffset2.x = movedPos.x - player.hitPosS.x;
			movedOffset3 = movedOffset;							// �E��
			movedOffset3.x = movedPos.x + player.hitPosE.x - 1;*/

			// 1Ͻ�̂Ƃ���ł̓W�����v���Ȃ�
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

			// �ׯ�߂̔���
			// ���߰�
			if (!TelIsPass(player[i].pos)) {
				player[i].pos = { 100000 , 0 };
			}

			// �j�u���b�N
			if (!NeedleIsPass(player[i].pos)) {
				TobichiriGenerator(player[i].pos);
				player[i].flag = false;
			}

			// �ެ�����ۯ�
			// 1Ͻ�̂Ƃ���ł̓W�����v���Ȃ�
			SetOffset(OFFSET_NOTJUMP);
			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
				movedOffset.y = movedPos.y + player[i].hitPosE.y;		// ����
				if (!JumpIsPass(movedOffset)) {
					player[i].velocity.y = 80;
				}
			}
			movedPos = player[i].pos;

			// �Z�[�u
			if (!SaveIsPass(player[i].pos)) {
				savePos[i] = player[i].pos;
			}


			// ��ʊO����ڲ԰���o����
			if (player[i].pos.y > SCREEN_SIZE_Y)
			{
				player[i].flag = false;
			}


		}
		else {	// ��ڰ԰�����񂾂Ƃ�

			if (trgKey[ENTER]) {
				player[i].pos = savePos[i];
				player[i].velocity = { 0,0 };
				player[i].flag = true;
			}
		}
		player[i].animCnt++;
	}
}

void PlayerDraw(void)
{
	for (int i = 0; i < PLAYER_MAX; i++) {
		XY tmpMapPos = GetMapPos();

		if (player[i].flag) {
			if (player[i].runFlag) {
				if (player[i].moveDir == DIR_RIGHT) {
					DrawGraph(player[i].pos.x - tmpMapPos.x - player[i].offsetSize.x, player[i].pos.y - tmpMapPos.y - player[i].offsetSize.y, pImage[i][player[i].animCnt / 15 % 4], true);
				}
				if (player[i].moveDir == DIR_LEFT) {
					DrawTurnGraph(player[i].pos.x - tmpMapPos.x - player[i].offsetSize.x, player[i].pos.y - tmpMapPos.y - player[i].offsetSize.y, pImage[i][player[i].animCnt / 15 % 4], true);
				}
			}
			else {
				if (player[i].moveDir == DIR_RIGHT) {
					DrawGraph(player[i].pos.x - tmpMapPos.x - player[i].offsetSize.x, player[i].pos.y - tmpMapPos.y - player[i].offsetSize.y, pImage[i][0], true);
				}
				if (player[i].moveDir == DIR_LEFT) {
					DrawTurnGraph(player[i].pos.x - tmpMapPos.x - player[i].offsetSize.x, player[i].pos.y - tmpMapPos.y - player[i].offsetSize.y, pImage[i][0], true);
				}
			}
		}
		DrawFormatString(0, 48, 0x000000, "playerPos: %d , %d", player[i].pos.x, player[i].pos.y);
		//DrawCircle(player.pos.x, player.pos.y, 5,  0xff0000, true);
	}
}

CHARACTER GetPlayer(int i) {
	return player[i];
}

bool PlayerHitCheck(XY pos, XY size, int type)
{
	for (int i = 0; i < PLAYER_MAX; i++) {
		//��`�Ƌ�`
		if (player[i].flag) {
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
			// �c���Ƌ�`
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
			// �����Ƌ�`
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
			
		}
	}
	return false;
}

void SetOffset(OFFSET_TYPE type) {			
	for (int i = 0; i < PLAYER_MAX; i++) {
		switch (type) {
		case OFFSET_LEFT_RIGHT:
			if (player[i].velocity.x > 0) {							// �E
				movedOffset.x = movedPos.x + player[i].hitPosE.x + 1;
			}
			if (player[i].velocity.x < 0) {							// ��
				movedOffset.x = movedPos.x - player[i].hitPosS.x - 1;
			}
			//��ڲ԰�̓���̒��S�̵̾��
			movedOffset2 = movedOffset;
			movedOffset2.y = movedPos.y - player[i].hitPosS.y;
			//��ڲ԰�̑����̒��S�̵̾��
			movedOffset3 = movedOffset;
			movedOffset3.y = movedPos.y + player[i].hitPosE.y - 1;			// �]1�͒����p
			break;
		case OFFSET_HEAD:
			movedOffset = movedPos;
			movedOffset.y = movedPos.y - player[i].hitPosS.y;
			movedOffset2 = movedOffset;							// ����
			movedOffset2.x = movedPos.x - player[i].hitPosS.x;
			movedOffset3 = movedOffset;							// �E��
			movedOffset3.x = movedPos.x + player[i].hitPosE.x - 1;
			break;
		case OFFSET_FOOT:
			movedOffset = movedPos;
			movedOffset.y = movedPos.y + player[i].hitPosE.y;
			movedOffset2 = movedOffset;							// ����
			movedOffset2.x = movedPos.x - player[i].hitPosS.x;
			movedOffset3 = movedOffset;							// �E��
			movedOffset3.x = movedPos.x + player[i].hitPosE.x - 1;
			break;
		case OFFSET_NOTJUMP:
			movedOffset = movedPos;
			movedOffset.y = movedPos.y - player[i].hitPosS.y - 1;
			movedOffset2 = movedOffset;							// ����
			movedOffset2.x = movedPos.x - player[i].hitPosS.x - 1;
			movedOffset3 = movedOffset;							// �E��
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
