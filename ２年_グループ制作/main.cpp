#include <DxLib.h>		//Dxlib���C�u�������g�p����
#include "main.h"
#include "stage.h"
#include "player.h"
#include "keycheck.h"

GAME_MODE gameMode;
int gameCounter;
int pause;	// �߰�ޗp	0:�߰�ނ��Ă��Ȃ��@1:�߰�ޒ�

// ========= WinMain�֐�
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	
	// ----------�V�X�e������
	SetWindowText("Shooting_kadai");
	//�V�X�e������
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);	// 640�~480�h�b�g65536�F���[�h�ɐݒ�
	ChangeWindowMode(true);							// true:window false:�t���X�N���[��
	if (DxLib_Init() == -1) return -1;				//DX���C�u��������������
	SetDrawScreen(DX_SCREEN_BACK);					//�ЂƂ܂��o�b�N�o�b�t�@�ɕ`��
	SysInit();

	// --------�Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClsDrawScreen(); //��ʏ���
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

		
		ScreenFlip();	//����ʂ�\��ʂɏu�ԃR�s�[
	}
	DxLib_End();	// DX���C�u�����̏I������
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