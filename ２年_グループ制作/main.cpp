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
int jyuni;		//仮　プレイヤーの順位
CHARACTER player[4];
int Stage;
PT pt;
CNT cnt;
int Arrow;
int EndPt;

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

	if (cnt.playerCnt == 1)DrawString(320, 480, "→", 0xFFFFFF);
	else if (cnt.playerCnt == 2)
	{
		DrawString(320, 500, "→", 0xFFFFFF);
	}
	else if (cnt.playerCnt == 3)
	{
		DrawString(320, 520, "→", 0xFFFFFF);
	}
	else if (cnt.playerCnt == 4)
	{
		DrawString(320, 540, "→", 0xFFFFFF);
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
		DrawString(320, 500, "→", 0xFFFFFF);
	}
	else if (GameCycleMax == 5)
	{
		DrawString(320, 520, "→", 0xFFFFFF);
	}
	else if (GameCycleMax == 7)
	{
		DrawString(320, 540, "→", 0xFFFFFF);
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
	srand((unsigned)time(NULL));						//仮　乱数を実行するたびにランダム化

	player[0].jyuni = rand() % 4 + 1;					//仮　元となる順位
	for (int i = 1; i <= 4; i++)
	{
		player[i].jyuni = player[i - 1].jyuni + 1;				//仮　順位をランダムに決める 1～4位　例　player1の順位はplayer1 - 1 の順位に1を足したもの
		if (player[i].jyuni == 5) {
			player[i].jyuni = 1;
		}
	}

	pt.BonusPt = 0;


	for (int i = 1; i <= 4; i++)
	{
		switch (player[i].jyuni)
		{
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
			player[i].point += pt.PlusPt + pt.BonusPt;
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



	DrawFormatString(0, 108, 0xFFFFFF, "player1 jyuni = %d位", player[1].jyuni); //仮　順位描写

	DrawFormatString(0, 24, 0xFFFFFF, "player2.Pt = %d", player[2].point);
	DrawFormatString(0, 36, 0xFFFFFF, "player3.Pt = %d", player[3].point);
	DrawFormatString(0, 48, 0xFFFFFF, "player4.Pt = %d", player[4].point);

	DrawFormatString(0, 120, 0xFFFFFF, "player2 jyuni = %d位", player[2].jyuni); //仮　順位描写
	DrawFormatString(0, 132, 0xFFFFFF, "player3 jyuni = %d位", player[3].jyuni); //仮　順位描写
	DrawFormatString(0, 144, 0xFFFFFF, "player4 jyuni = %d位", player[4].jyuni); //仮　順位描写
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