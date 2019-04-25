#include <DxLib.h>		//Dxlibライブラリを使用する
#include "main.h"
#include "stage.h"
#include "player.h"
#include "keycheck.h"

GAME_MODE gameMode;
int gameCounter;
int pause;	// ﾎﾟｰｽﾞ用	0:ﾎﾟｰｽﾞしていない　1:ﾎﾟｰｽﾞ中

// ========= WinMain関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	
	// ----------システム処理
	SetWindowText("Shooting_kadai");
	//システム処理
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);	// 640×480ドット65536色モードに設定
	ChangeWindowMode(true);							// true:window false:フルスクリーン
	if (DxLib_Init() == -1) return -1;				//DXライブラリ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);					//ひとまずバックバッファに描画
	SysInit();

	// --------ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen(); //画面消去
		KeyCheck();
		switch (gameMode) {
		case GMODE_INIT:
			GameInit();
			gameMode = GMODE_GAME;
			break;
		case GMODE_GAME:
			GameMain();
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

void SysInit(void)
{
	PlayerSysInit();
	gameMode = GMODE_INIT;
}

void GameInit(void)
{
	stageInit();
	PlayerInit();
}

void GameMain(void)
{
	stageMain();
	PlayerUpdate();
	GameDraw();
}

void GameDraw(void)
{
	stageDraw();
	PlayerDraw();
}