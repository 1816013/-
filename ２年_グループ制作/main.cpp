#include "main.h"
#include "keycheck.h"
#include "stage.h"
#include "player.h"


// �ϐ�
GAME_MODE gameMode;
int gameCounter;

// ̪��ޏ����p
int bright;
int fadeCnt;
bool fadeIn;
bool fadeOut;

// �����p
int numberImage[11];
int fontImage[27];

int pause;	// �߰�ޗp	0:�߰�ނ��Ă��Ȃ��@1:�߰�ޒ�

int titleImage;
int startKeyImage;
int gameoverImage;
int winImage[2];
int enterKeyImage;

int waitCnt = 0; //�������삳��Ă��Ȃ��Ƃ��̑J�ڗp


// ========Winmain�֐�
int WINAPI WinMain(HINSTANCE hINSTANCE, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	SysInit();
	// �V�X�e���̏�����
	if (SysInit() == false) {
		return -1;
	}
	gameMode = GMODE_INIT;
	// ------�ް�ٰ��
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen();	// ��ʏ���
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
	// �V�X�e������
	SetWindowText("AsoRockMan");
	// ���я���
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, COLOR_MODE);	// 640�~480�ޯ�65536�FӰ�ނɐݒ�
	ChangeWindowMode(false);							// true:window�@false:�ٽ�ذ�
	if (DxLib_Init() == -1)return false;				// DXײ���؏���������
	SetDrawScreen(DX_SCREEN_BACK);					// �ЂƂ܂��ޯ��ޯ̧�ɕ`��

	KeyCheckInit();

	// �摜�̓ǂݍ���
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

// �ްт̏�����
void GameInit(void)
{
	// --------- �֐��̏�����
	// ̪��ފ֐�
	fadeIn = false;
	fadeOut = false;
	fadeCnt = 0;

	// �߰��
	pause = 0;

	StageInit();
	PlayerInit();

}

// ����
void GameTitle(void)
{
	DrawString(0, 0, "GameTitle", 0xFFFFFF);
}

// �ްђ�
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

// �ްт̕`��
void GameDraw(void)
{
	StageDraw();
	PlayerDraw();


	//DrawWord("PLAYER", MAP_CHIP_SIZE_Y - 16 , 0);

	DrawFormatString(0, 0, 0xFFFFFF, "GameMain : %d", gameCounter);

}

// �ްѸر
void GameClear(void)
{
	DrawString(0, 0, "GameClear", 0xFFFFFF);

	if (trgKey[ENTER] == true || waitCnt > 300) {
		gameMode = GMODE_GAMEOVER;
		waitCnt = 0;
	}
}

// �ްѵ��ް
void GameOver(void)
{
	DrawString(0, 0, "GameOver", 0xFFFFFF);
	DrawGraph(150, 100, gameoverImage, true);
	DrawGraph(220, 400, startKeyImage, true);
}

// �����蔻��
void HitCheck(void)
{
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

// ��������
//void DrawStatus(const char* title, int value, int top)
//{
//	// ����
//	for (int i = 0; i < strlen(title); i++) {
//		DrawGraph( + 16 * (i + 1), top, fontImage[*(title + i) - 64], true);
//	}
//
//	// ���
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
	// ����
	for (int p = 0; p < 2; p++) {
		for (int i = 0; i < strlen(title); i++) {
			DrawGraph(MAP_CHIP_SIZE_X + (16 * (i + start)) + (MAP_CHIP_SIZE_X * 17 * p), top, fontImage[*(title + i) - 64], true);
		}

		DrawGraph(MAP_CHIP_SIZE_X * 4 + (MAP_CHIP_SIZE_X * 17 * p), top, numberImage[(p + 2)], true);
	}
}
