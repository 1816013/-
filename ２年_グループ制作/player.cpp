#include "main.h"
#include "player.h"
#include "keycheck.h"
#include "stage.h"

CHARACTER player;

int headFlag;
int jumpCnt;
int Gflag;
void PlayerSysInit(void) 
{
	
}

void PlayerInit(void)
{
	player.moveDir = DIR_RIGHT;
	player.pos = { 5 * MAP_CHIP_SIZE_X , 10 * MAP_CHIP_SIZE_Y };

	player.size = { PLAYER_SIZE_X, PLAYER_SIZE_Y };
	player.offsetSize = { player.size.x / 2, player.size.y / 2};
	player.hitPosS = { 24,  24 };									// ��ڲ԰�̍���
	player.hitPosE = { 24,  24 };
	player.velocity = { 0,0 };
	player.flag = true;
	player.animCnt = 0;
	jumpCnt = 0;

}

void PlayerUpdate(void)
{
	bool playerMoved = false;

	XY movedPos = player.pos;
	XY movedOffset = movedPos;
	XY movedOffset2 = movedPos;
	XY movedOffset3 = movedPos;
	XY tmpPos;

	XY playeroldPos = player.pos;

	player.imgLockCnt++;
	player.runFlag = false;
	headFlag = false;
	Gflag = true;

	// ��ڲ԰�̈ړ�
	if (player.flag) {
		if (newKey[P1_RIGHT]) {								// �E
			player.moveDir = DIR_RIGHT;
			player.velocity.x += ACC_RUN;					// ���x�̍X�V
			playerMoved = true;
			player.runFlag = true;
			if (player.velocity.x > VELOCITY_X_MAX) {
				player.velocity.x = VELOCITY_X_MAX;
			}
		}

		if (newKey[P1_LEFT]) {								// ��			
			player.moveDir = DIR_LEFT;
			player.velocity.x -= ACC_RUN * 1;				// ���x�̍X�V
			playerMoved = true;
			player.runFlag = true;
			if (player.velocity.x < -VELOCITY_X_MAX) {
				player.velocity.x = -VELOCITY_X_MAX;
			}
		}		
		if (!player.runFlag) {								// �~�܂������̌�������
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
		
		// �ړ�����(ϯ�߂�۰ق�����Ƃ��̂ݎg�p)
		XY tmpMapPos = GetMapPos();
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
		}

		movedPos.x += player.velocity.x * 1;	// �����̍X�V

		if (player.velocity.x > 0) {
			movedOffset.x = movedPos.x + player.hitPosE.x;
		}
		if (player.velocity.x < 0) {
			movedOffset.x = movedPos.x - player.hitPosS.x;
		}
		movedOffset2 = movedOffset;
		movedOffset2.y = movedPos.y - player.hitPosS.y;
		movedOffset3 = movedOffset;
		movedOffset3.y = movedPos.y + player.hitPosE.y - 1;

		if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
			player.pos = movedPos;
		}
		else {
			tmpPos = MapPosToMoveIndex(movedOffset, player.jumpFlag, player.velocity);
			if (player.velocity.x > 0) {			// �E
				player.pos.x = tmpPos.x - player.hitPosE.x;
			}
			if (player.velocity.x < 0) {			// ��
				player.pos.x = tmpPos.x + player.hitPosS.x;
			}
			movedPos = player.pos;
			player.velocity.x = 0;
		}
		
		// �d��
		movedPos = player.pos;
		movedOffset = movedPos;	

		movedPos.y -= player.velocity.y * SECOND_PER_FRAME;
		player.velocity.y -= ACC_G * SECOND_PER_FRAME;

		// ��ۯ�������
		// �������ۯ�������
		movedOffset.y = movedPos.y - player.hitPosS.y;
		movedOffset2 = movedOffset;							// ����
		movedOffset2.x = movedPos.x - player.hitPosS.x;
		movedOffset3 = movedOffset;							// �E��
		movedOffset3.x = movedPos.x + player.hitPosE.x - 1;

		if (IsPass(movedOffset) && IsPass(movedOffset2) && IsPass(movedOffset3)) {
			player.pos = movedPos;
		}
		else {
			headFlag = true;
			tmpPos = MapPosToMoveIndex(movedOffset, headFlag, player.velocity);
			player.pos.y = tmpPos.y + player.hitPosS.y;
			player.velocity.y *= -1;
		}
		movedPos = player.pos;

		// ��������ۯ�������
		movedOffset.y = movedPos.y + player.hitPosE.y;
		movedOffset2 = movedOffset;							// ����
		movedOffset2.x = movedPos.x - player.hitPosS.x;
		movedOffset3 = movedOffset;							// �E��
		movedOffset3.x = movedPos.x + player.hitPosE.x - 1;

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
	
		//�@�ެ���
		if (!player.jumpFlag) {
			if (newKey[P1_A]) {
				jumpCnt++;
				if (jumpCnt < 15) {
					player.velocity.y = 25;
				}
				if (headFlag) {
					jumpCnt = 15;
				}
			}
			else {
				jumpCnt = 0;
				player.jumpFlag = true;
			}
		}
		

		movedOffset2 = movedOffset;
		movedOffset2.y = movedPos.y - player.hitPosS.y;
		movedOffset3 = movedOffset;
		movedOffset3.y = movedPos.y + player.hitPosE.y - 1;

		if (HitIsPass(movedOffset) && HitIsPass(movedOffset2) && HitIsPass(movedOffset3)) {

		}
		else {
			DrawString(0, 30, "Hit", 0xffffff);
		}

		movedOffset.y = movedPos.y - player.hitPosS.y;
		movedOffset2 = movedOffset;							// ����
		movedOffset2.x = movedPos.x - player.hitPosS.x;
		movedOffset3 = movedOffset;							// �E��
		movedOffset3.x = movedPos.x + player.hitPosE.x - 1;

		if (HitIsPass(movedOffset) && HitIsPass(movedOffset2) && HitIsPass(movedOffset3)) {

		}
		else {
			DrawString(0, 30, "Hit", 0xffffff);
		}

		movedOffset.y = movedPos.y + player.hitPosE.y;
		movedOffset2 = movedOffset;							// ����
		movedOffset2.x = movedPos.x - player.hitPosS.x;
		movedOffset3 = movedOffset;							// �E��
		movedOffset3.x = movedPos.x + player.hitPosE.x - 1;
		if (HitIsPass(movedOffset) && HitIsPass(movedOffset2) && HitIsPass(movedOffset3)) {
			
		}
		else {
			DrawString(0, 30, "Hit", 0xffffff);
		}

	}
}

void PlayerDraw(void)
{
	XY tmpMapPos = GetMapPos();
	if (player.flag) {
		if (player.imgLockCnt > 30) {
			player.damageFlag = false;
		}
		DrawCircle(player.pos.x - tmpMapPos.x, player.pos.y  - tmpMapPos.y, 24, 0xff0000, true, true);
	
	}
	DrawFormatString(0, 15, 0x000000, "playerPos: %d , %d", player.pos.x, player.pos.y);
}

CHARACTER GetPlayer(void) {
	return player;
}