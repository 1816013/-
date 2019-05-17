#include <time.h>
#include <DxLib.h>		//Dxlib���C�u�������g�p����
#include <stdlib.h>
#include "main.h"
#include "stage.h"
#include "player.h"
#include "keycheck.h"
#include "effect.h"

GAME_MODE gameMode;
int gameCounter;
//int pause;	// �߰�ޗp	0:�߰�ނ��Ă��Ȃ��@1:�߰�ޒ�
int GameCycle; //���̃Q�[����
int GameCycleMax;//�I���Ƃ��̃Q�[����
int jyuni;		//���@�v���C���[�̏���
CHARACTER player[4];
int Stage;
PT pt;
CNT cnt;
int Arrow;
int EndPt;

// ========= WinMain�֐�
int WINAPI WinMain(HINSTANCE hINSTANCE, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	SysInit();
	if (SysInit() == false) {
		return -1;
	}

	// --------�Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen(); //��ʏ���
		KeyCheck();
		switch (gameMode) {
		case GMODE_INIT:
			GameInit();
			gameMode = GMODE_TITLE;
			break;
		case GMODE_TITLE:
			if (trgKey[START])
			{
				gameMode = GMODE_SELECT;
			}

			GameTitle();
			break;
		case GMODE_SELECT:
			GameSelect();
			break;
		case GMODE_BATTLESELECT:
			GameBattleSelect();
			break;
		case GMODE_GAME:
			GameMain();
			HitCheck();
			if (trgKey[START])gameMode = GMODE_RESULT;
			break;
		case GMODE_RESULT:
			GameResult();
			if (trgKey[START])
				if ((GameCycle < Arrow/*GameCycleMax*/) && (EndPt < 100))
				{
					GameCycle += 1;
					EndPt += 10;
					gameMode = GMODE_GAME;
					GameInit();

				}
				else if ((GameCycle == Arrow/*GameCycleMax*/) || (EndPt >= 100))
				{
					gameMode = GMODE_INIT/*GAMEOVER*/;
				}

			break;
		default:
			AST();
			break;
		}

		
		ScreenFlip();	//����ʂ�\��ʂɏu�ԃR�s�[
	}
	DxLib_End();	// DX���C�u�����̏I������
	return 0;
}

bool SysInit(void)
{
	// ----------�V�X�e������
	SetWindowText("Shooting_kadai");
	//�V�X�e������
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);	// 640�~480�h�b�g65536�F���[�h�ɐݒ�
	ChangeWindowMode(true);							// true:window false:�t���X�N���[��
	if (DxLib_Init() == -1) return false;				//DX���C�u��������������
	SetDrawScreen(DX_SCREEN_BACK);					//�ЂƂ܂��o�b�N�o�b�t�@�ɕ`��

	KeyCheckInit();
	PlayerSysInit();
	stageSysInit();
	EffectSysInit();
	gameMode = GMODE_INIT;

	// �����V�[�h�ݒ�
	int randSeed = (unsigned int)time(NULL);	// 1970.1.1����̕b
	srand(randSeed);	// �ǂ�����ǂނ��w��

	GameCycle = 1;
	GameCycleMax = 3;
	Stage = 1;
	cnt.playerCnt = 1;
	cnt.DeathCnt = 0;
	pt.EndPt = 0;
	for (int i = 0; i <= 4; i++)
	{
		player[i].point = 0;

		player[i].skin = 1;
	}

	return true;
}

void GameInit(void)
{
	
	stageInit();
	PlayerInit();
	EffectInit();
}

void GameTitle(void) 
{
	DrawString(0, 0, "GameTitle", 0xFFFFFF);
	DrawString(350, 480, "1 PLAYER BATTLE", 0xFFFFFF);
	DrawString(350, 500, "2 PLAYER BATTLE", 0xFFFFFF);
	DrawString(350, 520, "3 PLAYER BATTLE", 0xFFFFFF);
	DrawString(350, 540, "4 PLAYER BATTLE", 0xFFFFFF);

	if (trgKey[P1_UP])cnt.playerCnt -= 1;
	if (trgKey[P1_DOWN])cnt.playerCnt += 1;
	if (cnt.playerCnt < 1)cnt.playerCnt = 4;
	if (cnt.playerCnt > 4)cnt.playerCnt = 1;

	if (cnt.playerCnt == 1)DrawString(320, 480, "��", 0xFFFFFF);
	else if (cnt.playerCnt == 2)
	{
		DrawString(320, 500, "��", 0xFFFFFF);
	}
	else if (cnt.playerCnt == 3)
	{
		DrawString(320, 520, "��", 0xFFFFFF);
	}
	else if (cnt.playerCnt == 4)
	{
		DrawString(320, 540, "��", 0xFFFFFF);
	}
	DrawFormatString(0, 24, 0xFFFFFF, "playerCnt = %d", cnt.playerCnt);
}


void GameSelect(void)
{
	DrawString(0, 0, "playerSelect", 0xFFFFFF);

	for (int i = 1; i <= cnt.playerCnt; i++)
	{
		DrawFormatString(0, 100 + (12 * i), 0xFFFFFF, "player = %d", player[i].skin);
	}

	if (trgKey[P1_UP])
	{
		player[1].skin += 1;
	}
	if (trgKey[P1_DOWN])
	{
		player[1].skin -= 1;
	}
	if (player[1].skin > 4)
	{
		player[1].skin = 1;
	}
	if (player[1].skin < 1)
	{
		player[1].skin = 4;
	}

	if (trgKey[START])
	{
		gameMode = GMODE_BATTLESELECT;
	}
}

void GameBattleSelect(void)
{
	DrawString(0, 0, "BattleSelect", 0xFFFFFF);
	DrawFormatString(0, 24, 0xFFFFFF, "Batlle = %d", GameCycleMax);

	DrawString(350, 500, "3 ROUND BATTLE", 0xFFFFFF);
	DrawString(350, 520, "5 ROUND BATTLE", 0xFFFFFF);
	DrawString(350, 540, "7 ROUND BATTLE", 0xFFFFFF);

	if (GameCycleMax == 3)
	{
		DrawString(320, 500, "��", 0xFFFFFF);
	}
	else if (GameCycleMax == 5)
	{
		DrawString(320, 520, "��", 0xFFFFFF);
	}
	else if (GameCycleMax == 7)
	{
		DrawString(320, 540, "��", 0xFFFFFF);
	}

	DrawFormatString(0, 100, 0xFFFFFF, "MapNo.%d", Stage);

	if (trgKey[P1_UP])
	{
		GameCycleMax -= 2;
	}
	else if (trgKey[P1_DOWN])
	{
		GameCycleMax += 2;
	}

	if (GameCycleMax < 3)
	{
		GameCycleMax = 7;
	}
	else if (GameCycleMax > 7)
	{
		GameCycleMax = 3;
	}

	if (trgKey[P1_LEFT])
	{
		Stage -= 1;
	}
	else if (trgKey[P1_RIGHT])
	{
		Stage += 1;
	}

	if (Stage <= 0)
	{
		Stage = 3;
	}
	else if (Stage > 3)
	{
		Stage = 1;
	}
	if (trgKey[START])
	{
		gameMode = GMODE_GAME;

	}
}

void GameMain(void)
{
	srand((unsigned)time(NULL));						//���@���������s���邽�тɃ����_����

	player[0].jyuni = rand() % 4 + 1;					//���@���ƂȂ鏇��
	for (int i = 1; i <= 4; i++)
	{
		player[i].jyuni = player[i - 1].jyuni + 1;				//���@���ʂ������_���Ɍ��߂� 1�`4�ʁ@��@player1�̏��ʂ�player1 - 1 �̏��ʂ�1�𑫂�������
		if (player[i].jyuni == 5) {
			player[i].jyuni = 1;
		}
	}

	pt.BonusPt = 0;


	for (int i = 1; i <= 4; i++)
	{
		switch (player[i].jyuni)
		{
		case 1://if (player[i].jyuni == 1)pt.PlusPt = 25;			//�� ���ʂɂ����Pt��ς���
			pt.PlusPt = 25;
			break;
		case 2:	//else if (player[i].jyuni == 2)pt.PlusPt = 20;
			pt.PlusPt = 20;
			break;
		case 3:	//else if (player[i].jyuni == 3)pt.PlusPt = 15;
			pt.PlusPt = 15;
			break;
		case 4:	//else if (player[i].jyuni == 4) pt.PlusPt = 10;
			pt.PlusPt = 10;
			break;
		}
		if (trgKey[START]) {
			player[i].point += pt.PlusPt + pt.BonusPt;
		}
	}

	if (GameCycleMax == 3)pt.EndPt = 60;				//���@�ő�Q�[���񐔂ŖڕWPt���ς��
	else if (GameCycleMax == 5)pt.EndPt = 120;
	else pt.EndPt = 170;

	cnt.DeathCnt = rand() % 30 + 1;						//���@���񂾉񐔂�1�`30�񂩂猈�߂�	

	if (GameCycleMax == 3)cnt.DeathBonusCnt = 10;		//���@�ő�Q�[���񐔂Ŏ��񂾉񐔃{�[�i�X���ς��
	else if (GameCycleMax == 5)cnt.DeathBonusCnt = 15;
	else cnt.DeathBonusCnt = 20;

	if (cnt.DeathCnt < cnt.DeathBonusCnt)pt.BonusPt = 10;

	if (trgKey[START]) {
		gameMode = GMODE_RESULT;
		/*HitCheck();*/

	}
	stageMain();
	PlayerUpdate();
	EffectUpdate();
	GameDraw();

	CHARACTER tmp = GetPlayer();
	for (int i = 1; i <= 4; i++)
	{
		if (!GoalIsPass(tmp.pos)) {
			gameMode = GMODE_RESULT;
			player[i].point += pt.PlusPt + pt.BonusPt;
		}
	}
}


void GameResult(void)
{
	if (trgKey[START])
		if ((GameCycle < GameCycleMax) && (player[1].point < pt.EndPt))
		{
			GameCycle += 1;
			PlayerSysInit();
			GameInit();
			gameMode = GMODE_GAME;
		}
		else if ((GameCycle == GameCycleMax) || (player[1].point >= pt.EndPt))
		{
			SysInit();
			gameMode = GMODE_INIT/*GAMEOVER*/;
		}
	if ((GameCycle == GameCycleMax) || (player[1].point >= pt.EndPt))
	{
		DrawString(0, 0, "FinalResult", 0xFFFFFF);
	}
	else
	{
		DrawString(0, 0, "GameResult", 0xFFFFFF);
	}
	DrawFormatString(0, 12, 0xFFFFFF, "player1.Pt = %d", player[1].point);

	DrawFormatString(0, 60, 0xFFFFFF, "DeathCnt = %d", cnt.DeathCnt);
	DrawFormatString(0, 72, 0xFFFFFF, "BonusPt = %d", pt.BonusPt);
	DrawFormatString(0, 84, 0xFFFFFF, "EndPt = %d", pt.EndPt);



	DrawFormatString(0, 108, 0xFFFFFF, "player1 jyuni = %d��", player[1].jyuni); //���@���ʕ`��

	DrawFormatString(0, 24, 0xFFFFFF, "player2.Pt = %d", player[2].point);
	DrawFormatString(0, 36, 0xFFFFFF, "player3.Pt = %d", player[3].point);
	DrawFormatString(0, 48, 0xFFFFFF, "player4.Pt = %d", player[4].point);

	DrawFormatString(0, 120, 0xFFFFFF, "player2 jyuni = %d��", player[2].jyuni); //���@���ʕ`��
	DrawFormatString(0, 132, 0xFFFFFF, "player3 jyuni = %d��", player[3].jyuni); //���@���ʕ`��
	DrawFormatString(0, 144, 0xFFFFFF, "player4 jyuni = %d��", player[4].jyuni); //���@���ʕ`��
}
void HitCheck() {
	for (int i = 0; i < 20; i++) {
		CHARACTER tmp = GetTrap(i);
		if (tmp.flag) {
			PlayerHitCheck(tmp.pos, tmp.size, tmp.type);
		}
		else {
			if (tmp.type == 3) {
				PlayerHitCheck(tmp.pos, tmp.size, tmp.type);
			}
		}
	}
}


void GameDraw(void)
{
	stageDraw();
	PlayerDraw();
	EffectDraw();
	DrawFormatString(0, 12, 0xFFFFFF, "GameCeycle = %d", GameCycle);
	DrawFormatString(0, 24, 0xFFFFFF, "Arrow = %d", Arrow);
	DrawFormatString(0, 0, 0xFFFFFF, "GameMain", gameCounter);
}