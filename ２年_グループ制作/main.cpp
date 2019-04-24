#include "main.h"
#include "keycheck.h"
#include "stage.h"
#include "player.h"


// 変数
GAME_MODE gameMode;
int gameCounter;

// ﾌｪｰﾄﾞ処理用
int bright;
int fadeCnt;
bool fadeIn;
bool fadeOut;

// 文字用
int numberImage[11];
int fontImage[27];

int pause;	// ﾎﾟｰｽﾞ用	0:ﾎﾟｰｽﾞしていない　1:ﾎﾟｰｽﾞ中

int titleImage;
int startKeyImage;
int gameoverImage;
int winImage[2];
int enterKeyImage;

int waitCnt = 0; //長く操作されていないときの遷移用


// ========Winmain関数
int WINAPI WinMain(HINSTANCE hINSTANCE, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	SysInit();
	// システムの初期化
	if (SysInit() == false) {
		return -1;
	}
	gameMode = GMODE_INIT;
	// ------ｹﾞｰﾑﾙｰﾌﾟ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen();	// 画面消去
		KeyCheck();

		switch (gameMode) {
		case GMODE_INIT:
			GameInit();
			gameMode = GMODE_TITLE;
			break;
		case GMODE_TITLE:
			if (fadeIn) {
				if (!FadeInScreen(5)) fadeIn = false;
			}
			else if (fadeOut) {
				if (!FadeOutScreen(5)) {
					gameMode = GMODE_GAME;
					fadeOut = false;
					fadeIn = true;
				}
			}
			else {
				if (trgKey[ENTER]) fadeOut = true;
			}
			GameTitle();
			break;

		case GMODE_GAME:
			if (fadeIn)
			{
				if (!FadeInScreen(5)) fadeIn = false;
			}
			GameMain();
			break;

		case GMODE_CLEAR:
			GameClear();
			break;

		case GMODE_GAMEOVER:
			if (fadeIn)
			{
				if (!FadeInScreen(5)) fadeIn = false;
			}
			else if (fadeOut) {
				if (!FadeOutScreen(5)) {
					gameMode = GMODE_TITLE;
					GameInit();
					fadeOut = false;
					fadeIn = true;
				}
			}
			else {
				if (trgKey[ENTER]) fadeOut = true;
			}
			GameOver();
			break;

		default:
			AST();
			break;
		}
		if (pause == 0) {
			gameCounter++;
		}
		ScreenFlip();
	}
	DxLib_End();
	return 0;
}

bool SysInit(void)
{
	// システム処理
	SetWindowText("AsoRockMan");
	// ｼｽﾃﾑ処理
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, COLOR_MODE);	// 640×480ﾄﾞｯﾄ65536色ﾓｰﾄﾞに設定
	ChangeWindowMode(false);							// true:window　false:ﾌﾙｽｸﾘｰﾝ
	if (DxLib_Init() == -1)return false;				// DXﾗｲﾌﾞﾗﾘ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);					// ひとまずﾊﾞｯｸﾊﾞｯﾌｧに描画

	KeyCheckInit();

	// 画像の読み込み
	startKeyImage = LoadGraph("image/hitstartkey.png");
	gameoverImage = LoadGraph("image/gameover.png");
	enterKeyImage = LoadGraph("image/button.png");
	winImage[0] = LoadGraph("image/player1win.png");
	winImage[1] = LoadGraph("image/player2win.png");

	LoadDivGraph("image/number16pix.png", 11, 11, 1, 16, 16, numberImage, true);
	LoadDivGraph("image/font16pix.png", 27, 27, 1, 16, 16, fontImage, true);

	StageSysInit();
	PlayerSysInit();



	return true;
}

// ｹﾞｰﾑの初期化
void GameInit(void)
{
	// --------- 関数の初期化
	// ﾌｪｰﾄﾞ関数
	fadeIn = false;
	fadeOut = false;
	fadeCnt = 0;

	// ﾎﾟｰｽﾞ
	pause = 0;

	StageInit();
	PlayerInit();

}

// ﾀｲﾄﾙ
void GameTitle(void)
{
	DrawString(0, 0, "GameTitle", 0xFFFFFF);
}

// ｹﾞｰﾑ中
void GameMain(void)
{
	if (trgKey[PAUSE]) {
		pause = !pause;
	}

	if (pause) {
		SetDrawBright(128, 128, 128);
	}
	else {
		HitCheck();
		PlayerUpdate();
		StageUpdate();


	}

	GameDraw();

	if (pause) {
		SetDrawBright(255, 255, 255);
		DrawString(SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("PAUSE") / 2, SCREEN_SIZE_Y / 2, "PAUSE", 0xffffff, true);
	}

}

// ｹﾞｰﾑの描写
void GameDraw(void)
{
	StageDraw();
	PlayerDraw();


	//DrawWord("PLAYER", MAP_CHIP_SIZE_Y - 16 , 0);

	DrawFormatString(0, 0, 0xFFFFFF, "GameMain : %d", gameCounter);

}

// ｹﾞｰﾑｸﾘｱ
void GameClear(void)
{
	DrawString(0, 0, "GameClear", 0xFFFFFF);

	if (trgKey[ENTER] == true || waitCnt > 300) {
		gameMode = GMODE_GAMEOVER;
		waitCnt = 0;
	}
}

// ｹﾞｰﾑｵｰﾊﾞｰ
void GameOver(void)
{
	DrawString(0, 0, "GameOver", 0xFFFFFF);
	DrawGraph(150, 100, gameoverImage, true);
	DrawGraph(220, 400, startKeyImage, true);
}

// 当たり判定
void HitCheck(void)
{
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

// 文字入力
//void DrawStatus(const char* title, int value, int top)
//{
//	// 文字
//	for (int i = 0; i < strlen(title); i++) {
//		DrawGraph( + 16 * (i + 1), top, fontImage[*(title + i) - 64], true);
//	}
//
//	// ｽｺｱ
//	int numLen = 0;
//	int sc = value;
//
//	DrawGraph(SCREEN_SIZE_X - 16 * 2, top + 30, numberImage[1], true);
//	DrawGraph(SCREEN_SIZE_X - 16, top + 30, numberImage[1], true);
//
//	while (sc > 0)
//	{
//		DrawGraph(SCREEN_SIZE_X - (numLen + 3) * 16, top + 30, numberImage[sc % 10 + 1], true);
//
//		sc /= 10;
//		numLen++;
//	}
//}

void DrawWord(const char* title, int top, int start) {
	// 文字
	for (int p = 0; p < 2; p++) {
		for (int i = 0; i < strlen(title); i++) {
			DrawGraph(MAP_CHIP_SIZE_X + (16 * (i + start)) + (MAP_CHIP_SIZE_X * 17 * p), top, fontImage[*(title + i) - 64], true);
		}

		DrawGraph(MAP_CHIP_SIZE_X * 4 + (MAP_CHIP_SIZE_X * 17 * p), top, numberImage[(p + 2)], true);
	}
}
