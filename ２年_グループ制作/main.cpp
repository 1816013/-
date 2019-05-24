#include <time.h>
#include <DxLib.h>		//Dxlibライブラリを使用する
#include <stdlib.h>
#include "main.h"
#include "stage.h"
#include "player.h"
#include "keycheck.h"
#include "effect.h"


GAME_MODE gameMode;
int gameCounter;
//int pause;	// ﾎﾟｰｽﾞ用	0:ﾎﾟｰｽﾞしていない　1:ﾎﾟｰｽﾞ中
int GameCycle; //今のゲーム回数
int GameCycleMax;//終わるときのゲーム回数
CHARACTER playerPt[4];
int Stage;
PT pt;
CNT cnt;
int stopCnt = 0;
int timeCnt;

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
		case GMODE_SELECT:
			GameSelect();
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
			if (trgKey[START]) {
				SysInit();
				gameMode = GMODE_INIT;
			}

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
	stageSysInit();
	EffectSysInit();
	gameMode = GMODE_INIT;

	// 乱数シード設定
	int randSeed = (unsigned int)time(NULL);	// 1970.1.1からの秒
	srand(randSeed);	// どこから読むか指定

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
	
	// ﾌﾟﾚｲﾔｰ数の決定
	if (trgKey[P1_UP])cnt.playerCnt -= 1;
	if (trgKey[P1_DOWN])cnt.playerCnt += 1;
	if (cnt.playerCnt < 1)cnt.playerCnt = 4;
	if (cnt.playerCnt > 4)cnt.playerCnt = 1;

	if (cnt.playerCnt == 1)DrawString(320, 480, "→", 0x000000);
	else if (cnt.playerCnt == 2)
	{
		DrawString(320, 500, "→", 0x000000);
	}
	else if (cnt.playerCnt == 3)
	{
		DrawString(320, 520, "→", 0x000000);
	}
	else if (cnt.playerCnt == 4)
	{
		DrawString(320, 540, "→", 0x000000);
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
		DrawString(320, 500, "→", 0x000000);
	}
	else if (GameCycleMax == 5)
	{
		DrawString(320, 520, "→", 0x000000);
	}
	else if (GameCycleMax == 7)
	{
		DrawString(320, 540, "→", 0x000000);
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
	srand((unsigned)time(NULL));						//仮　乱数を実行するたびにランダム化

	timeCnt++;
	for (int i = 0; i < cnt.playerCnt; i++)
	{
		CHARACTER tmp = GetPlayer(i);

		if (tmp.flag) {
			// ｺｲﾝ判定
			if (!CoinIsPass(tmp.pos)) {
				pt.BonusPt += pt.CoinPt;
			}
		}
		if (tmp.jyuni >= cnt.playerCnt) {
			stopCnt = 0;
			gameMode = GMODE_RESULT;
			playerPt[i].point += pt.PlusPt + pt.BonusPt;
		}
		//	playerPt[0].jyuni = rand() % 4 + 1;					//仮　元となる順位
		//
		//for (int i = 0; i < 3; i++)
		//{
		//	playerPt[i + 1].jyuni = playerPt[i].jyuni + 1;				//仮　順位をランダムに決める 1～4位　例　player1の順位はplayer1 - 1 の順位に1を足したもの
		//	if (playerPt[i + 1].jyuni > 4) {
		//		playerPt[i + 1].jyuni = 1;
		//	}
		//}
		switch (tmp.jyuni)
		{
		case 0://if (player[i].jyuni == 1)pt.PlusPt = 25;			//仮 順位によってPtを変える
			pt.PlusPt = 0;
			break;
		case 1://if (player[i].jyuni == 1)pt.PlusPt = 25;			//仮 順位によってPtを変える
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

	if (GameCycleMax == 3)pt.EndPt = 60;				//仮　最大ゲーム回数で目標Ptが変わる
	else if (GameCycleMax == 5)pt.EndPt = 120;
	else pt.EndPt = 170;

	cnt.DeathCnt = rand() % 30 + 1;						//仮　死んだ回数を1～30回から決める	

	if (GameCycleMax == 3)cnt.DeathBonusCnt = 10;		//仮　最大ゲーム回数で死んだ回数ボーナスが変わる
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
	
	// 描画
	for (int i = 0; i < cnt.playerCnt; i++)
	{
		CHARACTER tmp = GetPlayer(i);
		DrawString(0, 0, "GameResult", 0xFFFFFF);
		DrawFormatString(0, 60, 0xFFFFFF, "DeathCnt = %d", cnt.DeathCnt);
		DrawFormatString(0, 72, 0xFFFFFF, "BonusPt = %d", pt.BonusPt);
		DrawFormatString(0, 84, 0xFFFFFF, "EndPt = %d", pt.EndPt);
		DrawFormatString(0, 96, 0xFFFFFF, "クリアタイム %d秒", timeCnt / 60);
		for (int i = 0; i < cnt.playerCnt; i++) {
			DrawFormatString(0, 12 * (i + 1), 0xFFFFFF, "player%d.Pt = %d", i + 1, playerPt[i].point);
			DrawFormatString(0, 108 + 12 * (i + 1), 0xFFFFFF, "player%d jyuni = %d位", i + 1, tmp.jyuni); //仮　順位描写
		}
	}
}

void GameFinalResult(void) {
	
	

	for (int i = 0; i < cnt.playerCnt; i++) {
		playerPt[i].sortPt = playerPt[i].point;
	}

	RankSort();		// 順位付けソート

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