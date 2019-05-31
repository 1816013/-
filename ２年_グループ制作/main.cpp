#include <time.h>
#include <DxLib.h>		//Dxlib���C�u�������g�p����
#include <stdlib.h>
#include "main.h"
#include "stage.h"
#include "player.h"
#include "keycheck.h"
#include "effect.h"
#include "shot.h"

GAME_MODE gameMode;
int gameCounter;
//int pause;	// �߰�ޗp	0:�߰�ނ��Ă��Ȃ��@1:�߰�ޒ�
int GameCycle; //���̃Q�[����
int GameCycleMax;//�I���Ƃ��̃Q�[����
CHARACTER playerPt[PLAYER_MAX];
int Stage;
PT pt[PLAYER_MAX];
CNT cnt;
int timeCnt;
int titleImage;
int numImage[11];
int playerBattle;
// ̪��ޏ����p
int bright;
int fadeCnt;
bool fadeIn;
bool fadeOut;

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
	ShotSysinit();

	stageSysInit();
	EffectSysInit();


	// ���̨��
	titleImage = LoadGraph("png/�^�C�g��.png", true);
	playerBattle = LoadGraph("png/player battle.png", true);
	if((LoadDivGraph("png/����.png", 11, 11, 1, 24, 24, numImage, true))== -1) AST();

	// �����V�[�h�ݒ�
	int randSeed = (unsigned int)time(NULL);	// 1970.1.1����̕b
	srand(randSeed);	// �ǂ�����ǂނ��w��

	// �ϐ�������
	GameCycle = 1;
	GameCycleMax = 0;
	cnt.playerCnt = 1;
	cnt.DeathCnt = 0;
	
	for (int i = 0; i < 4; i++)
	{
		playerPt[i].point = 0;
		playerPt[i].sortPt = 0;
		pt[i].CoinPt = 50;

	}
	gameMode = GMODE_INIT;
	return true;
}

void GameInit(void)
{

	stageInit();
	PlayerInit();
	EffectInit();
	ShotInit();
	timeCnt = 0;
	cnt.GoalCnt = 0;
}

void GameTitle(void) 
{
	stageDraw();
	if (fadeIn) {
		if (!FadeInScreen(5)) fadeIn = false;
	}
	else if (fadeOut) {
		if (!FadeOutScreen(5)) {
			gameMode = GMODE_BATTLESELECT;
			PlayerInit();
			stageInit();
			fadeOut = false;
			fadeIn = true;
		}
	}
	else {
		if (trgKey[START]) fadeOut = true;
	}
	//if (trgKey[START])
	//{
	//	gameMode = GMODE_BATTLESELECT;
	//	PlayerInit();
	//	stageInit();
	//}
	
	// ��ڲ԰���̌���
	if (trgKey[P1_UP])cnt.playerCnt -= 1;
	if (trgKey[P1_DOWN])cnt.playerCnt += 1;
	if (cnt.playerCnt < 1)cnt.playerCnt = 4;
	if (cnt.playerCnt > 4)cnt.playerCnt = 1;

	// �`��
	DrawGraph(308, 150, titleImage, true);
	DrawTriangleAA(406,600 + 34 * (cnt.playerCnt - 1),
				   436, 612 + 34 * (cnt.playerCnt - 1),
				   406, 624 + 34 * (cnt.playerCnt - 1),
		0x000000, true);
	for (int i = 0; i < PLAYER_MAX; i++) {
		DrawGraph(506, 600 + 34 * i, playerBattle, false);
		DrawGraph(456, 600 + 34 * i, numImage[i + 2], true);
	}
	DrawString(0, 0, "GameTitle", 0x000000);
	DrawFormatString(0, 24, 0x000000, "playerCnt = %d", cnt.playerCnt);
}

void GameBattleSelect(void)
{
	if (fadeIn)
	{
		if (!FadeInScreen(5)) {
	
			fadeIn = false;
		}
	}
	int tmpCnt = GetSelectCnt();
	if (tmpCnt == cnt.playerCnt) {
		if (fadeOut && !fadeIn) {
			if (!FadeOutScreen(5)) {
				if (GetCycleType() != 4) {
					GameCycleMax = GetCycleType();
				}
				else {
					GameCycleMax = 2;
				}
				gameMode = GMODE_GAME;
				PlayerInit();
				stageInit();
				fadeOut = false;
				fadeIn = true;
			}
		}
		else {
			if (trgKey[START]) fadeOut = true;
		}
	}
	GameDraw();
	HitCheck();
	if (!fadeIn) {
		PlayerUpdate();
		Shotcontrol();
	}

	/*int tmpCnt = GetSelectCnt();
	if (tmpCnt == cnt.playerCnt) {
		if (trgKey[START]) {
			if (GetCycleType() != 4) {
				GameCycleMax = GetCycleType();
			}
			else
			{
				GameCycleMax = 2;
			}
			gameMode = GMODE_GAME;
			PlayerInit();
			stageInit();
		}
	}*/

	

	/*if (trgKey[START])
	{
		GameCycleMax = 1;
		gameMode = GMODE_GAME;
		PlayerInit();
		stageInit();
	}*/

	DrawString(0, 0, "BattleSelect", 0x000000);
	DrawFormatString(0, 24, 0x000000, "Batlle = %d", GameCycleMax);

	
}

void GameMain(void)
{
	if (fadeIn)
	{
		if (!FadeInScreen(5)) fadeIn = false;
	}
	timeCnt++;
	for (int i = 0; i < cnt.playerCnt; i++)
	{
		CHARACTER tmp = GetPlayer(i);
		int gcnt = GetGoalCnt();

		if (tmp.flag) {
			// ��ݔ���
			if (!CoinIsPass(tmp.pos)) {
				pt[i].BonusPt += pt[i].CoinPt;
			}
		}
		// ����
		if (!tmp.giveupFlag) {
			switch (tmp.jyuni)
			{
			case 0://if (player[i].jyuni == 0)pt.PlusPt = 25;			//�� ���ʂɂ����Pt��ς���
				pt[i].PlusPt = 200;
				break;
			case 1://if (player[i].jyuni == 1)pt.PlusPt = 20;			//�� ���ʂɂ����Pt��ς���
				pt[i].PlusPt = 150;
				break;
			case 2:	//else if (player[i].jyuni == 2)pt.PlusPt = 15;
				pt[i].PlusPt = 100;
				break;
			case 3:	//else if (player[i].jyuni == 3)pt.PlusPt = 10;
				pt[i].PlusPt = 50;
				break;
			}
		}
		else {
			pt[i].PlusPt = 0;
		}

		//if (GameCycleMax == 1) {//���@�ő�Q�[���񐔂ŖڕWPt���ς��
		//	pt.EndPt = 60;
		//}
		//else if (GameCycleMax == 2) {
		//	pt.EndPt = 120;
		//}
		//else {
		//	pt.EndPt = 170;
		//}

		//cnt.DeathCnt = rand() % 30 + 1;						//���@���񂾉񐔂�1�`30�񂩂猈�߂�	

		//if (GameCycleMax == 1) {
		//	cnt.DeathBonusCnt = 10;		//���@�ő�Q�[���񐔂Ŏ��񂾉񐔃{�[�i�X���ς��
		//}
		//else if (GameCycleMax == 2) {
		//	cnt.DeathBonusCnt = 15;
		//}
		//else {
		//	cnt.DeathBonusCnt = 20;
		//}

		if (gcnt >= cnt.playerCnt) {
			gameMode = GMODE_RESULT;
			playerPt[i].point += pt[i].PlusPt + pt[i].BonusPt - tmp.deathCnt;
		}

	}

	if (trgKey[START]) {
		gameMode = GMODE_RESULT;
		/*HitCheck();*/
	}
	if (!fadeIn) {
		stageMain();
		PlayerUpdate();
		EffectUpdate();
		Shotcontrol();
		timeCnt++;
		if (CheckHitKey(KEY_INPUT_G)) {
			GiveUp(0);
		}
	}
	
	GameDraw();
	DrawFormatString(0, 24, 0x000000, "Batlle = %d", GameCycleMax);

	
	
}


void GameResult(void)
{
	if (trgKey[START]) {
		if (GameCycle < GameCycleMax)
		{
			GameCycle++;
			PlayerSysInit();
			GameInit();
			gameMode = GMODE_GAME;
		}
		else if (GameCycle >= GameCycleMax)
		{
			gameMode = GMODE_F_RESULT;
		}
	}
	
	// �`��
	for (int i = 0; i < cnt.playerCnt; i++)
	{
		CHARACTER tmp = GetPlayer(i);
		DrawString(0, 0, "GameResult", 0xFFFFFF);
		DrawFormatString(0, 60, 0xFFFFFF, "DeathCnt = %d", cnt.DeathCnt);
		DrawFormatString(0, 72, 0xFFFFFF, "BonusPt = %d", pt[i].BonusPt);
		DrawFormatString(0, 84, 0xFFFFFF, "EndPt = %d", pt[i].EndPt);
		DrawFormatString(0, 96, 0xFFFFFF, "�N���A�^�C�� %d�b", timeCnt / 60);
		DrawFormatString(0, 12 * (i + 1), 0xFFFFFF, "player%d.Pt = %d", i + 1, playerPt[i].point);
		DrawFormatString(0, 108 + 12 * (i + 1), 0xFFFFFF, "player%d jyuni = %d��", i + 1, tmp.jyuni + 1); //���@���ʕ`��
		
	}
}

void GameFinalResult(void) {
	for (int i = 0; i < cnt.playerCnt; i++) {
		playerPt[i].sortPt = playerPt[i].point;
	}
	RankSort();		// ���ʕt���\�[�g

	if (trgKey[START]) {
		SysInit();
		gameMode = GMODE_INIT;
	}

	

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
		TRAP trapTmp = GetTrap(i);
		if (trapTmp.flag) {
			PlayerHitCheck(trapTmp.pos, trapTmp.size, trapTmp.type, (MOVE_DIR)0);
		}
	}
	for (int j = 0; j < SHOT_MAX; j++) {
		SHOT shotTmp = Getshot(j);
		if (shotTmp.flag) {
			if (PlayerHitCheck(shotTmp.pos, shotTmp.size, shotTmp.type, shotTmp.moveDir)) {
				DeleteShot(j);
			}
		}
	}
}


void GameDraw(void)
{
	stageDraw();
	PlayerDraw();
	EffectDraw();
	ShotDraw();
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
// ̪��޲ݏ���
bool FadeInScreen(int fadeStep)
{
	fadeCnt += fadeStep;
	if (fadeCnt <= 255) {
		SetDrawBright(fadeCnt, fadeCnt, fadeCnt);
		return true;
	}
	else {
		fadeCnt = 0;
		SetDrawBright(255, 255, 255);
		return false;
	}
}

// ̪��ޱ�ď���
bool FadeOutScreen(int fadeStep)
{
	fadeCnt += fadeStep;
	if (fadeCnt <= 255) {
		SetDrawBright(255 - fadeCnt, 255 - fadeCnt, 255 - fadeCnt);
		return true;
	}
	else {
		fadeCnt = 0;
		SetDrawBright(0, 0, 0);
		return false;
	}
}
