#include <time.h>
#include <DxLib.h>		//Dxlib���C�u�������g�p����
#include "main.h"
#include "stage.h"
#include "player.h"
#include "keycheck.h"
#include "effect.h"

GAME_MODE gameMode;
int gameCounter;
//int pause;	// �߰�ޗp	0:�߰�ނ��Ă��Ȃ��@1:�߰�ޒ�

int GameCycle; //���̃Q�[����
//int GameCycleMax;//�I���Ƃ��̃Q�[����
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
			GameCycle = 1;
			Arrow = 3;
			EndPt = 15;
			/*GameCycleMax = 3;*/
			GameInit();
			gameMode = GMODE_TITLE;
			break;
		case GMODE_TITLE:
			if (trgKey[START])
			{
				gameMode = GMODE_GAME;
			}

			GameTitle();

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
	if (trgKey[P1_UP])
	{
		/*GameCycleMax*/Arrow -= 2;
	}
	else if (trgKey[P1_DOWN])
	{
		/*GameCycleMax*/ Arrow += 2;
	}

	if (Arrow <= 3)
	{
		Arrow = 3;
	}
	else if (Arrow >= 7)
	{
		Arrow = 7;
	}

	DrawString(0, 0, "GameTitle", 0xFFFFFF);
	DrawFormatString(0, 24, 0xFFFFFF, "Arrow = %d", Arrow);

	DrawString(350, 500, "3 ROUND BATTLE", 0xFFFFFF);
	DrawString(350, 520, "5 ROUND BATTLE", 0xFFFFFF);
	DrawString(350, 540, "7 ROUND BATTLE", 0xFFFFFF);


	if (/*GameCycleMax*/Arrow == 3)
	{
		DrawString(320, 500, "��", 0xFFFFFF);
	}
	else if (/*GameCycleMax*/Arrow == 5)
	{
		DrawString(320, 520, "��", 0xFFFFFF);
	}
	else if (/*GameCycleMax*/Arrow == 7)
	{
		DrawString(320, 540, "��", 0xFFFFFF);
	}

}

void GameMain(void)
{
	stageMain();
	PlayerUpdate();
	EffectUpdate();
	GameDraw();

	CHARACTER tmp = GetPlayer();
	if (!GoalIsPass(tmp.pos)) {
		gameMode = GMODE_RESULT;
	}
}


void GameResult(void)
{
	if ((GameCycle == Arrow/*GameCycleMax*/) || (EndPt >= 100))
	{
		DrawString(0, 0, "FinalResult", 0xFFFFFF);
	}
	else
	{
		DrawString(0, 0, "GameResult", 0xFFFFFF);
	}
	DrawFormatString(0, 24, 0xFFFFFF, "EndPt = %d", EndPt);
}
void HitCheck() {
	for (int i = 0; i < 20; i++) {
		CHARACTER tmp = GetTrap(i);
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
	DrawFormatString(0, 24, 0xFFFFFF, "Arrow = %d", Arrow);
	DrawFormatString(0, 0, 0xFFFFFF, "GameMain", gameCounter);
}