#include "main.h"
#include "keycheck.h"

bool newKey[KEY_MAX];	// new��(�����ڰт̷����͏�)
bool trgKey[KEY_MAX];	// trg��(�O�񉟂��ĂȂ��č��񉟂������̏�)
bool upKey[KEY_MAX];	// up��(�O�񉟂��Ă��č��񉟂��Ă��Ȃ����̏�)
bool oldKey[KEY_MAX];	// old��(����̓��͏�Ԃ�����̔��f�p�ɕۑ�)

void KeyCheckInit()
{
	// ----������������
	for (int i = 0; i < KEY_MAX; i++) {
		newKey[i] = false;		// �S�������Ă��Ȃ����Ƃɂ���
		trgKey[i] = false;
		upKey[i] = false;
		oldKey[i] = false;
	}

}


void KeyCheck()
{
	// ---- new��
	for (int i = 0; i < KEY_MAX; i++) {
		newKey[i] = false;		// �S�������Ă��Ȃ����Ƃɂ���
		trgKey[i] = false;
		upKey[i] = false;
	}
	if (CheckHitKey(KEY_INPUT_UP)) newKey[P1_UP] = true;
	if (CheckHitKey(KEY_INPUT_RIGHT)) newKey[P1_RIGHT] = true;
	if (CheckHitKey(KEY_INPUT_DOWN)) newKey[P1_DOWN] = true;
	if (CheckHitKey(KEY_INPUT_LEFT)) newKey[P1_LEFT] = true;
	if (CheckHitKey(KEY_INPUT_SPACE)) newKey[P1_SHOT] = true;
	if (CheckHitKey(KEY_INPUT_RSHIFT)) newKey[P1_A] = true;
	if (CheckHitKey(KEY_INPUT_RCONTROL)) newKey[P1_B] = true;


	if (CheckHitKey(KEY_INPUT_W)) newKey[P2_UP] = true;
	if (CheckHitKey(KEY_INPUT_D)) newKey[P2_RIGHT] = true;
	if (CheckHitKey(KEY_INPUT_S)) newKey[P2_DOWN] = true;
	if (CheckHitKey(KEY_INPUT_A)) newKey[P2_LEFT] = true;
	if (CheckHitKey(KEY_INPUT_LSHIFT)) newKey[P2_SHOT] = true;	// ���۰ٷ�
	if (CheckHitKey(KEY_INPUT_LCONTROL)) newKey[P2_A] = true;	// �����
	if (CheckHitKey(KEY_INPUT_LALT)) newKey[P2_B] = true;

	if (CheckHitKey(KEY_INPUT_RETURN)) newKey[ENTER] = true;
	if (CheckHitKey(KEY_INPUT_P)) newKey[PAUSE] = true;

	// ----- trgKey upKey oldKey
	for (int i = 0; i < KEY_MAX; i++) {
		trgKey[i] = newKey[i] & ~oldKey[i];	// trgKey
		upKey[i] = ~newKey[i] & oldKey[i];	// upKey
		oldKey[i] = newKey[i];						// oldKey
	}

	//// p2
	//trgKey[P2_SHOT] = newKey[P2_SHOT] & ~oldKey[P2_SHOT];	// trgKey
	//upKey[P2_SHOT] = ~newKey[P2_SHOT] & oldKey[P2_SHOT];	// upKey
	//oldKey[P2_SHOT] = newKey[P2_SHOT];						// oldKey

	////�J�ڗp
	//trgKey[ENTER] = newKey[ENTER] & ~oldKey[ENTER];	// trgKey
	//oldKey[ENTER] = newKey[ENTER];						// oldKey

	//trgKey[PAUSE] = newKey[PAUSE] & ~oldKey[PAUSE];	// trgKey
	//oldKey[PAUSE] = newKey[PAUSE];						// oldKey
}