#include <time.h>
#include <DxLib.h>		//Dxlibライブラリを使用する
#include <stdlib.h>
#include "main.h"
#include "stage.h"
#include "player.h"
#include "keycheck.h"
#include "effect.h"
#include "shot.h"

GAME_MODE gameMode;
int gameCounter;
//int pause;	// ﾎﾟｰｽﾞ用	0:ﾎﾟｰｽﾞしていない　1:ﾎﾟｰｽﾞ中
int GameCycle; //今のゲーム回数
int GameCycleMax;//終わるときのゲーム回数
CHARACTER playerPt[PLAYER_MAX];
int Stage;
PT pt[PLAYER_MAX];
CNT cnt;
int timeCnt;
int titleImage;
int numImage[11];
int playerBattle;
// ﾌｪｰﾄﾞ処理用
int bright;
int fadeCnt;
bool fadeIn;
bool fadeOut;

// ========= WinMain関数
int WINAPI WinMain(HINSTANCE hINSTANCE, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	SysInit();
	if (SysInit() == false) {
		return -1;
	}

	// --------ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen(); //画面消去
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
			if (trgKey[START])gameMode = GMODE_RESULT;	// ﾃﾞﾊﾞｯｸﾞ用
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

		
		ScreenFlip();	//裏画面を表画面に瞬間コピー
	}
	DxLib_End();	// DXライブラリの終了処理
	return 0;
}

bool SysInit(void)
{
	// ----------システム処理
	SetWindowText("Shooting_kadai");
	//システム処理
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);	// 640×480ドット65536色モードに設定
	ChangeWindowMode(true);							// true:window false:フルスクリーン
	if (DxLib_Init() == -1) return false;				//DXライブラリ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);					//ひとまずバックバッファに描画

	KeyCheckInit();
	PlayerSysInit();
	ShotSysinit();

	stageSysInit();
	EffectSysInit();


	// ｸﾞﾗﾌｨｯｸ
	titleImage = LoadGraph("png/タイトル.png", true);
	playerBattle = LoadGraph("png/player battle.png", true);
	if((LoadDivGraph("png/数字.png", 11, 11, 1, 24, 24, numImage, true))== -1) AST();

	// 乱数シード設定
	int randSeed = (unsigned int)time(NULL);	// 1970.1.1からの秒
	srand(randSeed);	// どこから読むか指定

	// 変数初期化
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
	
	// ﾌﾟﾚｲﾔｰ数の決定
	if (trgKey[P1_UP])cnt.playerCnt -= 1;
	if (trgKey[P1_DOWN])cnt.playerCnt += 1;
	if (cnt.playerCnt < 1)cnt.playerCnt = 4;
	if (cnt.playerCnt > 4)cnt.playerCnt = 1;

	// 描画
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
			// ｺｲﾝ判定
			if (!CoinIsPass(tmp.pos)) {
				pt[i].BonusPt += pt[i].CoinPt;
			}
		}
		// 順位
		if (!tmp.giveupFlag) {
			switch (tmp.jyuni)
			{
			case 0://if (player[i].jyuni == 0)pt.PlusPt = 25;			//仮 順位によってPtを変える
				pt[i].PlusPt = 200;
				break;
			case 1://if (player[i].jyuni == 1)pt.PlusPt = 20;			//仮 順位によってPtを変える
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

		//if (GameCycleMax == 1) {//仮　最大ゲーム回数で目標Ptが変わる
		//	pt.EndPt = 60;
		//}
		//else if (GameCycleMax == 2) {
		//	pt.EndPt = 120;
		//}
		//else {
		//	pt.EndPt = 170;
		//}

		//cnt.DeathCnt = rand() % 30 + 1;						//仮　死んだ回数を1～30回から決める	

		//if (GameCycleMax == 1) {
		//	cnt.DeathBonusCnt = 10;		//仮　最大ゲーム回数で死んだ回数ボーナスが変わる
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
	
	// 描画
	for (int i = 0; i < cnt.playerCnt; i++)
	{
		CHARACTER tmp = GetPlayer(i);
		DrawString(0, 0, "GameResult", 0xFFFFFF);
		DrawFormatString(0, 60, 0xFFFFFF, "DeathCnt = %d", cnt.DeathCnt);
		DrawFormatString(0, 72, 0xFFFFFF, "BonusPt = %d", pt[i].BonusPt);
		DrawFormatString(0, 84, 0xFFFFFF, "EndPt = %d", pt[i].EndPt);
		DrawFormatString(0, 96, 0xFFFFFF, "クリアタイム %d秒", timeCnt / 60);
		DrawFormatString(0, 12 * (i + 1), 0xFFFFFF, "player%d.Pt = %d", i + 1, playerPt[i].point);
		DrawFormatString(0, 108 + 12 * (i + 1), 0xFFFFFF, "player%d jyuni = %d位", i + 1, tmp.jyuni + 1); //仮　順位描写
		
	}
}

void GameFinalResult(void) {
	for (int i = 0; i < cnt.playerCnt; i++) {
		playerPt[i].sortPt = playerPt[i].point;
	}
	RankSort();		// 順位付けソート

	if (trgKey[START]) {
		SysInit();
		gameMode = GMODE_INIT;
	}

	

	// 描画系
	DrawString(0, 0, "FinalResult", 0xFFFFFF);
	for (int i = 0; i < cnt.playerCnt; i++) {
		DrawFormatString(0, 12 * (i + 1), 0xFFFFFF, "player%d.Pt = %d", i + 1, playerPt[i].point);
		DrawFormatString(0, 108 + 12 * (i + 1), 0xFFFFFF, "player%d jyuni = %d位", i + 1, playerPt[i].jyuni); //仮　順位描写
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
// ﾌｪｰﾄﾞｲﾝ処理
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

// ﾌｪｰﾄﾞｱｳﾄ処理
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
