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
CHARACTER playerPt[4];
int Stage;
PT pt;
CNT cnt;
int stopCnt = 0;
int timeCnt;

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
			if (trgKey[START])gameMode = GMODE_RESULT;	// ���ޯ�ޗp
			break;
		case GMODE_RESULT:
			GameResult();
			break;
		case GMODE_F_RESULT:
			GameFinalResult();
			if (trgKey[START]) {
				SysInit();
				gameMode = GMODE_INIT;
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
	pt.CoinPt = 20;
	for (int i = 0; i < 4; i++)
	{
		playerPt[i].point = 0;
		playerPt[i].sortPt = 0;

		playerPt[i].skin = 1;
	}

	return true;
}

void GameInit(void)
{
	cnt.GoalCnt = 0;
	stageInit();
	PlayerInit();
	EffectInit();
	timeCnt = 0;
}

void GameTitle(void) 
{
	stageDraw();
	if (trgKey[START])
	{
		gameMode = GMODE_BATTLESELECT;
		PlayerInit();
		stageInit();
	}
	
	// ��ڲ԰���̌���
	if (trgKey[P1_UP])cnt.playerCnt -= 1;
	if (trgKey[P1_DOWN])cnt.playerCnt += 1;
	if (cnt.playerCnt < 1)cnt.playerCnt = 4;
	if (cnt.playerCnt > 4)cnt.playerCnt = 1;

	if (cnt.playerCnt == 1)DrawString(320, 480, "��", 0x000000);
	else if (cnt.playerCnt == 2)
	{
		DrawString(320, 500, "��", 0x000000);
	}
	else if (cnt.playerCnt == 3)
	{
		DrawString(320, 520, "��", 0x000000);
	}
	else if (cnt.playerCnt == 4)
	{
		DrawString(320, 540, "��", 0x000000);
	}
	DrawString(0, 0, "GameTitle", 0x000000);
	DrawString(350, 480, "1 PLAYER BATTLE", 0x000000);
	DrawString(350, 500, "2 PLAYER BATTLE", 0x000000);
	DrawString(350, 520, "3 PLAYER BATTLE", 0x000000);
	DrawString(350, 540, "4 PLAYER BATTLE", 0x000000);
	DrawFormatString(0, 24, 0x000000, "playerCnt = %d", cnt.playerCnt);
}


void GameSelect(void)
{
	stageDraw();
	DrawString(0, 0, "playerSelect", 0x000000);

	/*for (int i = 0; i < cnt.playerCnt; i++)
	{
		DrawFormatString(0, 100 + (12 * i), 0x000000, "player = %d", playerPt[i].skin);
	}
	if (trgKey[P1_UP])
	{
		playerPt[1].skin += 1;
	}
	if (trgKey[P1_DOWN])
	{
		playerPt[1].skin -= 1;
	}
	if (playerPt[1].skin > 4)
	{
		playerPt[1].skin = 1;
	}
	if (playerPt[1].skin < 1)
	{
		playerPt[1].skin = 4;
	}
*/
	if (GameCycleMax == 3)
	{
		DrawString(320, 500, "��", 0x000000);
	}
	else if (GameCycleMax == 5)
	{
		DrawString(320, 520, "��", 0x000000);
	}
	else if (GameCycleMax == 7)
	{
		DrawString(320, 540, "��", 0x000000);
	}

	DrawFormatString(0, 100, 0x000000, "MapNo.%d", Stage);

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
		Stage = STAGE_MAX;
	}
	else if (Stage > STAGE_MAX)
	{
		Stage = 1;
	}
	if (trgKey[START])
	{
		gameMode = GMODE_BATTLESELECT;
	
	}
	DrawString(350, 500, "3 ROUND BATTLE", 0x000000);
	DrawString(350, 520, "5 ROUND BATTLE", 0x000000);
	DrawString(350, 540, "7 ROUND BATTLE", 0x000000);
}

void GameBattleSelect(void)
{
	GameDraw();
	PlayerUpdate();
	EffectUpdate();
	HitCheck();
	for (int i = 0; i < cnt.playerCnt; i++)
	{
		CHARACTER tmp = GetPlayer(i);

		if (tmp.selectFlag) {
			if (trgKey[ENTER]) {
				gameMode = GMODE_GAME;
				PlayerInit();
				stageInit();
			}
		}

	}

	if (trgKey[START])
	{
		gameMode = GMODE_GAME;
		PlayerInit();
		stageInit();
	}

	DrawString(0, 0, "BattleSelect", 0xffffff);
	DrawFormatString(0, 24, 0xffffff, "Batlle = %d", GameCycleMax);

	
}

void GameMain(void)
{
	srand((unsigned)time(NULL));						//���@���������s���邽�тɃ����_����

	timeCnt++;
	for (int i = 0; i < cnt.playerCnt; i++)
	{
		CHARACTER tmp = GetPlayer(i);

		if (tmp.flag) {
			// ��ݔ���
			if (!CoinIsPass(tmp.pos)) {
				pt.BonusPt += pt.CoinPt;
			}
		}
		if (tmp.jyuni >= cnt.playerCnt) {
			stopCnt = 0;
			gameMode = GMODE_RESULT;
			playerPt[i].point += pt.PlusPt + pt.BonusPt;
		}
		//	playerPt[0].jyuni = rand() % 4 + 1;					//���@���ƂȂ鏇��
		//
		//for (int i = 0; i < 3; i++)
		//{
		//	playerPt[i + 1].jyuni = playerPt[i].jyuni + 1;				//���@���ʂ������_���Ɍ��߂� 1�`4�ʁ@��@player1�̏��ʂ�player1 - 1 �̏��ʂ�1�𑫂�������
		//	if (playerPt[i + 1].jyuni > 4) {
		//		playerPt[i + 1].jyuni = 1;
		//	}
		//}
		switch (tmp.jyuni)
		{
		case 0://if (player[i].jyuni == 1)pt.PlusPt = 25;			//�� ���ʂɂ����Pt��ς���
			pt.PlusPt = 0;
			break;
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
			playerPt[i].point += pt.PlusPt + pt.BonusPt;
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
		stopCnt = 0;
		/*HitCheck();*/

	}
	stopCnt++;
	if (stopCnt > 30) {
		stageMain();
		PlayerUpdate();
		EffectUpdate();
		timeCnt++;
	}
	GameDraw();

	
	
}


void GameResult(void)
{
	if (trgKey[START]) {
		if ((GameCycle < GameCycleMax) && (playerPt[1].point < pt.EndPt))
		{
			GameCycle += 1;
			PlayerSysInit();
			GameInit();
			gameMode = GMODE_GAME;
		}
		else if ((GameCycle == GameCycleMax) || (playerPt[1].point >= pt.EndPt))
		{
			gameMode = GMODE_F_RESULT/*GAMEOVER*/;
		}
	}
	
	// �`��
	for (int i = 0; i < cnt.playerCnt; i++)
	{
		CHARACTER tmp = GetPlayer(i);
		DrawString(0, 0, "GameResult", 0xFFFFFF);
		DrawFormatString(0, 60, 0xFFFFFF, "DeathCnt = %d", cnt.DeathCnt);
		DrawFormatString(0, 72, 0xFFFFFF, "BonusPt = %d", pt.BonusPt);
		DrawFormatString(0, 84, 0xFFFFFF, "EndPt = %d", pt.EndPt);
		DrawFormatString(0, 96, 0xFFFFFF, "�N���A�^�C�� %d�b", timeCnt / 60);
		for (int i = 0; i < cnt.playerCnt; i++) {
			DrawFormatString(0, 12 * (i + 1), 0xFFFFFF, "player%d.Pt = %d", i + 1, playerPt[i].point);
			DrawFormatString(0, 108 + 12 * (i + 1), 0xFFFFFF, "player%d jyuni = %d��", i + 1, tmp.jyuni); //���@���ʕ`��
		}
	}
}

void GameFinalResult(void) {
	
	

	for (int i = 0; i < cnt.playerCnt; i++) {
		playerPt[i].sortPt = playerPt[i].point;
	}

	RankSort();		// ���ʕt���\�[�g

	// �`��n
	DrawString(0, 0, "FinalResult", 0xFFFFFF);
	for (int i = 0; i < cnt.playerCnt; i++) {
		DrawFormatString(0, 12 * (i + 1), 0xFFFFFF, "player%d.Pt = %d", i + 1, playerPt[i].point);
		DrawFormatString(0, 108 + 12 * (i + 1), 0xFFFFFF, "player%d jyuni = %d��", i + 1, playerPt[i].jyuni); //���@���ʕ`��
		DrawFormatString(0, 180 + 12 * (i + 1), 0xFFFFFF, "player%d.sortPt = %d", i + 1, playerPt[i].sortPt);
	}
}

void HitCheck() {
	for (int i = 0; i < TRAP_MAX; i++) {
		TRAP tmp = GetTrap(i);
		if (tmp.flag) {
			PlayerHitCheck(tmp.pos, tmp.size, tmp.type);
		}
	}
}


void GameDraw(void)
{
	stageDraw();
	PlayerDraw();
	EffectDraw();
	DrawFormatString(0, 12, 0xFFFFFF, "GameCeycle = %d", GameCycle);
	DrawFormatString(0, 0, 0xFFFFFF, "GameMain", gameCounter);
}

int GetPlayerCnt(void) {
	return cnt.playerCnt;
}

void RankSort() {
	CHARACTER tmp;
	for (int j = 0; j < cnt.playerCnt - 1; j++) {
		for (int i = 0; i < cnt.playerCnt - 1; i++) {
			if (playerPt[i].sortPt < playerPt[i + 1].sortPt) {
				tmp.sortPt = playerPt[i].sortPt;
				playerPt[i].sortPt = playerPt[i + 1].sortPt;
				playerPt[i + 1].sortPt = tmp.sortPt;
			}
		}
	}
	for (int i = 0; i < cnt.playerCnt; i++) {
		for (int j = 0; j < cnt.playerCnt; j++) {
			if (playerPt[i].sortPt == playerPt[j].point)
			{
					playerPt[j].jyuni = i + 1;
				if (playerPt[i].sortPt == playerPt[i - 1].sortPt) 
				{
					playerPt[j].jyuni = i ;
					if (playerPt[i].sortPt == playerPt[i - 2].sortPt) 
					{
						playerPt[j].jyuni = i - 1;
						if (playerPt[i].sortPt == playerPt[i - 3].sortPt)
						{
							playerPt[j].jyuni = i - 2;
						}
					}
				}
			}
		}
	}
}

GAME_MODE GetGameMode() {
	return gameMode;
}