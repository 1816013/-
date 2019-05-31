#include <DxLib.h>
#include "main.h"
#include "player.h"
#include "keycheck.h"
#include "stage.h"
#include "effect.h"
#include "shot.h"



CHARACTER player[PLAYER_MAX];
// ���̨�������
int pImage[PLAYER_MAX][PLAYER_IMAGE_MAX];
int pIcon[PLAYER_MAX];
int pIconOK;

// �ϐ�
int jumpCnt[PLAYER_MAX];
int jumpFrame[PLAYER_MAX];
int selectCnt;
int goalCnt;
int giveupCnt;
int cycleCnt[4];
int cycleType;
int playerMax;
int intervalCnt;

XY savePos[PLAYER_MAX];	

//XY movedPos;
//// offset�͍�����ڲ԰��1Ͻ���Ƃ̍��W
//XY movedOffset;
//XY movedOffset2;
//XY movedOffset3;
int offsetType;					

void PlayerSysInit(void) 
{
	LoadDivGraph("png/�v���C���[1.png", 4, 4, 1, 48, 48, pImage[0]);
	LoadDivGraph("png/�v���C���[2.png", 4, 4, 1, 48, 48, pImage[1]);
	LoadDivGraph("png/�v���C���[3.png", 4, 4, 1, 48, 48, pImage[2]);
	LoadDivGraph("png/�v���C���[4.png", 4, 4, 1, 48, 48, pImage[3]);
	pIcon[0] = LoadGraph("png/�l�[��1.png");
	pIcon[1] = LoadGraph("png/�l�[��2.png");
	pIcon[2] = LoadGraph("png/�l�[��3.png");
	pIcon[3] = LoadGraph("png/�l�[��4.png");
	pIconOK = LoadGraph("png/ok.png");
	for (int i = 0; i < PLAYER_MAX; i++) {
		savePos[i] = { 2 * CHIP_SIZE_X - PLAYER_SIZE_X / 2, 19 * CHIP_SIZE_Y - PLAYER_SIZE_Y / 2 };		// �v���C���[���X�n������
	}
}

void PlayerInit(void)
{
	for (int i = 0; i < PLAYER_MAX; i++) {
		player[i].moveDir = DIR_RIGHT;
		//player.pos = { 4 * CHIP_SIZE_X - PLAYER_SIZE_X / 2, 3 * CHIP_SIZE_Y };
		player[i].size = { PLAYER_SIZE_X, PLAYER_SIZE_Y };
		player[i].offsetSize = { player[i].size.x / 2, player[i].size.y / 2 };
		player[i].hitPosS = { 17,  24 };									// ��ڲ԰�̍���
		player[i].hitPosE = { 17,  24 };									// ��ڲ԰�̉E��
		player[i].velocity = { 0,0 };
		player[i].animCnt = 0;
		jumpFrame[i] = 15;
		jumpCnt[i] = 0;
		player[i].flag = false;
		player[i].goalFlag = false;
		player[i].selectFlag = false;
		player[i].giveupFlag = false;
		player[i].shotInterval = false;
		player[i].jyuni = 3;
		player[i].selectType = 0;
		player[i].deathCnt = 0;

	}
	playerMax = GetPlayerCnt();
	for (int j = 0; j < playerMax; j++) {
		player[j].pos = savePos[j];
		player[j].flag = true;
	}
	goalCnt = 0;
	giveupCnt = 0;
	intervalCnt = 0;
}

void PlayerUpdate(void)
{
	for (int i = 0; i < playerMax; i++) {
		//�@1ٰ�ߖ��ɏ���������ϐ�
		XY tmpPos;
		//XY playeroldPos = player.pos;
		XY movedPos = player[i].pos;

		// offset�͍�����ڲ԰��1Ͻ���Ƃ̍��W
		XY movedOffset = movedPos;
		XY movedOffset2 = movedPos;
		XY movedOffset3 = movedPos;

		player[i].runFlag = false;
		player[i].headFlag = false;

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
					player[i].velocity.x -= ACC_RUN ;				// ���x�̍X�V
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

			if (player[i].velocity.x > 0) {							// �E
				movedOffset.x = movedPos.x + player[i].hitPosE.x;
			}
			if (player[i].velocity.x < 0) {							// ��
				movedOffset.x = movedPos.x - player[i].hitPosS.x;
			}
			//��ڲ԰�̓���̒��S�̵̾��
			movedOffset2 = movedOffset;
			movedOffset2.y = movedPos.y - player[i].hitPosS.y;
			//��ڲ԰�̑����̒��S�̵̾��
			movedOffset3 = movedOffset;
			movedOffset3.y = movedPos.y + player[i].hitPosE.y - 1;			// �]1�͒���

			//SetOffset(OFFSET_LEFT_RIGHT);

			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {		// movedOffset����ڲ԰�̒��SmovedOffset2����ڲ԰�̓���̒��SmovedOffset3����ڲ԰�̑����̒��S
				player[i].pos = movedPos;													// 
			}
			else {
				tmpPos = MapPosToMoveIndex(movedOffset, player[i].headFlag, player[i].velocity);
				if (player[i].velocity.x > 0) {			// �E
					player[i].pos.x = tmpPos.x - player[i].hitPosE.x;
				}
				if (player[i].velocity.x < 0) {			// ��
					player[i].pos.x = tmpPos.x + player[i].hitPosS.x;
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

			movedOffset = movedPos;
			movedOffset.y = movedPos.y - player[i].hitPosS.y;		
			movedOffset2 = movedOffset;							// ����
			movedOffset2.x = movedPos.x - player[i].hitPosS.x;
			movedOffset3 = movedOffset;							// �E��
			movedOffset3.x = movedPos.x + player[i].hitPosE.x - 1;
		//	SetOffset(OFFSET_HEAD);

			if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
				player[i].pos = movedPos;
			}
			else {
				player[i].headFlag = true; 
				tmpPos = MapPosToMoveIndex(movedOffset, player[i].headFlag, player[i].velocity);
				player[i].pos.y = tmpPos.y + player[i].hitPosS.y;
				player[i].velocity.y *= -1;
			}
			movedPos = player[i].pos;


			// ��������ۯ�������

			movedOffset = movedPos;
			movedOffset.y = movedPos.y + player[i].hitPosE.y;
			movedOffset.y = movedPos.y + player[i].hitPosE.y;
			movedOffset2 = movedOffset;							// ����
			movedOffset2.x = movedPos.x - player[i].hitPosS.x;
			movedOffset3 = movedOffset;							// �E��
			movedOffset3.x = movedPos.x + player[i].hitPosE.x - 1;

		//	SetOffset(OFFSET_FOOT);

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
			// 1Ͻ�̂Ƃ���ł̓W�����v���Ȃ�
			movedOffset = movedPos;
			movedOffset.y = movedPos.y - player[i].hitPosS.y;
			movedOffset2 = movedOffset;							// ����
			movedOffset2.x = movedPos.x - player[i].hitPosS.x;
			movedOffset3 = movedOffset;							// �E��
			movedOffset3.x = movedPos.x + player[i].hitPosE.x - 1;
			//SetOffset(OFFSET_NOTJUMP);
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
			movedOffset = movedPos;
			movedOffset.y = movedPos.y - player[i].hitPosS.y;
			movedOffset2 = movedOffset;							// ����
			movedOffset2.x = movedPos.x - player[i].hitPosS.x;
			movedOffset3 = movedOffset;							// �E��
			movedOffset3.x = movedPos.x + player[i].hitPosE.x - 1;
			//SetOffset(OFFSET_NOTJUMP);
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

			// �S�[��
			if (!GoalIsPass(player[i].pos)) {
				if(!player[i].goalFlag) {
					player[i].flag = false;
					player[i].goalFlag = true;
					player[i].jyuni = goalCnt;
					goalCnt++;
				}
			}
			

			// ��ʊO����ڲ԰���o����
			if (player[i].pos.y > SCREEN_SIZE_Y)
			{
				player[i].flag = false;
			}

			if (trgKey[BACK]) {
				player[i].selectFlag = false;
			}

			if (!player[i].shotInterval) {
				if (bulletKeyCheck(i)) {
					XY Shotpos = player[i].pos;
					if (player[i].jumpFlag == true)
					{
						Shotpos.y -= 5;
					}
					if (player[i].moveDir == DIR_RIGHT)
					{
						BulletFire({ Shotpos.x + player[i].hitPosE.x,Shotpos.y - 3 },
							player[i].moveDir);
					}
					else if (player[i].moveDir == DIR_LEFT)
					{
						BulletFire({ player[i].pos.x - player[i].hitPosS.x - 3, player[i].pos.y - 3 },
							player[i].moveDir);
					}
					player[i].shotInterval = true;
				}
			}
			if (player[i].shotInterval) {
				intervalCnt++;
				if (intervalCnt > 60) {
					player[i].shotInterval = false;
					intervalCnt = 0;
				}
			}

		}
		else {	// ��ڰ԰�����񂾂Ƃ�
			if (!player[i].goalFlag) {
				player[i].pos = savePos[i];
				player[i].velocity = { 0,0 };
				player[i].deathCnt += 2;
				player[i].flag = true;
			}
		}
		player[i].animCnt++;
	}
	selectCnt = 0;
	for (int j = 0; j < 4; j++) {
		cycleCnt[j] = 0;
	}
	for (int i = 0; i < playerMax; i++) {
		if (player[i].selectFlag) {
			selectCnt++;
			if (player[i].selectType == 1) {
				cycleCnt[0]++;
			}
			if (player[i].selectType == 2) {
				cycleCnt[1]++;
			}
			if (player[i].selectType == 3) {
				cycleCnt[2]++;
			}
			if (player[i].selectType == 4) {
				cycleCnt[3]++;
			}
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
		for (int j = 0; j < 4; j++) {
			DrawFormatString(0, 72 + 12 * j, 0x000000, "cycleCnt: %d", cycleCnt[j]);
			
		}
		DrawFormatString(0, 132 + 12 * i, 0x000000, "playerselectFlag %d: %d", i + 1, player[i].selectFlag);
		DrawFormatString(0, 180, 0x000000, "cycleType: %d", cycleType);
		DrawFormatString(0, 192 + 12 * i, 0x000000, "playerjyuni %d: %d", i + 1, player[i].jyuni + 1);

		//DrawCircle(player.pos.x, player.pos.y, 5,  0xff0000, true);selectCntplayer[i].selectFlag
	}
}

CHARACTER GetPlayer(int i) {
	return player[i];
}

bool PlayerHitCheck(XY pos, XY size, HIT_TYPE type, MOVE_DIR dir)
{
	for (int i = 0; i < PLAYER_MAX; i++) {
		if (player[i].flag) {
			//�j
			if (type == HIT_NEEDLE) {
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
			// 6����1�j
			if (type == HIT_S_NEEDLE) {
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
			// ��
			if (type == HIT_ARROW) {
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
			//��`�Ƌ�`(�Z���N�g�p)
			if (type == HIT_SELECT_1 || type == HIT_SELECT_2 || type == HIT_SELECT_3 || type == HIT_SELECT_EX) {
				if ((pos.x < player[i].pos.x + player[i].hitPosE.x)
					&& (pos.x + size.x > player[i].pos.x - player[i].hitPosS.x)
					&& (pos.y < player[i].pos.y + player[i].hitPosE.y)
					&& (pos.y + size.y > player[i].pos.y - player[i].hitPosS.y)
					) {
					if (trgKey[ENTER]) {
						if (!player[i].selectFlag) {
							if (type == HIT_SELECT_1) {
								cycleCnt[0]++;
								player[i].selectType = 1;
							}
							if (type == HIT_SELECT_2) {
								cycleCnt[1]++;
								player[i].selectType = 2;
							}
							if (type == HIT_SELECT_3) {
								cycleCnt[2]++;
								player[i].selectType = 3;
							}
							if (type == HIT_SELECT_EX) {
								cycleCnt[3]++;
								player[i].selectType = 4;
							}
							player[i].selectFlag = true;
						}
					}
				}
			}
			if (type == HIT_SHOT) {
				if (((player[i].pos.x - player[i].hitPosS.x) <= pos.x
				&& (player[i].pos.x + player[i].hitPosE.x) >= pos.x
				&& (player[i].pos.y + player[i].hitPosE.y) >= pos.y
				&& (player[i].pos.y - player[i].hitPosS.y) <= pos.y))
				{
					if (dir == DIR_LEFT)
					{
						XY movePos = { player[i].pos.x - CHIP_SIZE_X-13 , player[i].pos.y};
						if (IsPass(movePos)) {
							player[i].pos.x = player[i].pos.x - CHIP_SIZE_X;
						}
					}
					else if (dir == DIR_RIGHT)
					{
						XY movePos = { player[i].pos.x + CHIP_SIZE_X+13 , player[i].pos.y };
						if (IsPass(movePos)) {
							player[i].pos.x = player[i].pos.x + CHIP_SIZE_X;
						}
					}
					return true;
				}
			}
		}
	}
	return false;
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
	default:
		AST();
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

int GetCycleType()
{
	int max = 0;
	for (int j = 0; j < 4; j++) {
		if (cycleCnt[j] > max) {
			max = cycleCnt[j];
		}
	}
	int cnt = 0;
	for (int j = 0; j < 4; j++) {
		if (cycleCnt[j] == max) {
			cnt++;
			if (cnt > 1) {
				int num = (int)(rand() % 2);	// ����т�0��1�Ő��������߂�
				if (num == 0) {
					cycleType = j + 1;
				}
			}
			else {
				cycleType = j + 1;
			}
		}
	}
	return cycleType;
}

int GetGoalCnt() {	// �ر�������ͷ��ޱ��߂̐l��
	return goalCnt + giveupCnt;
}
bool bulletKeyCheck(int i) {
	switch (i) {
	case 0:
		if (trgKey[P1_B]) {
			return true;
		}
		break;
	case 1:
		if (trgKey[P2_B]) {
			return true;
		}
		break;
	case 2:
		if (trgKey[P3_B]) {
			return true;
		}
		break;
	case 3:
		if (trgKey[P4_B]) {
			return true;
		}
		break;
	}
	return false;
}

void GiveUp(int i) {
	// ���ޱ���
	if (!player[i].goalFlag) {
		player[i].flag = false;
		player[i].goalFlag = true;
		player[i].jyuni = playerMax - 1;
		player[i].giveupFlag = true;
		giveupCnt++;
	}
	
}
