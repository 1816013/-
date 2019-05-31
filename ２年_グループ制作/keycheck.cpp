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
	if (CheckHitKey(KEY_INPUT_RSHIFT)) newKey[P1_A] = true;
	if (CheckHitKey(KEY_INPUT_Z)) newKey[P1_B] = true;


	if (CheckHitKey(KEY_INPUT_W)) newKey[P2_UP] = true;
	if (CheckHitKey(KEY_INPUT_D)) newKey[P2_RIGHT] = true;
	if (CheckHitKey(KEY_INPUT_S)) newKey[P2_DOWN] = true;
	if (CheckHitKey(KEY_INPUT_A)) newKey[P2_LEFT] = true;
	if (CheckHitKey(KEY_INPUT_LSHIFT)) newKey[P2_A] = true;
	if (CheckHitKey(KEY_INPUT_X)) newKey[P2_B] = true;

	if (CheckHitKey(KEY_INPUT_R)) newKey[P3_UP] = true;
	if (CheckHitKey(KEY_INPUT_H)) newKey[P3_RIGHT] = true;
	if (CheckHitKey(KEY_INPUT_G)) newKey[P3_DOWN] = true;
	if (CheckHitKey(KEY_INPUT_F)) newKey[P3_LEFT] = true;
	if (CheckHitKey(KEY_INPUT_T)) newKey[P3_A] = true;
	if (CheckHitKey(KEY_INPUT_C)) newKey[P3_B] = true;

	if (CheckHitKey(KEY_INPUT_U)) newKey[P4_UP] = true;
	if (CheckHitKey(KEY_INPUT_L)) newKey[P4_RIGHT] = true;
	if (CheckHitKey(KEY_INPUT_K)) newKey[P4_DOWN] = true;
	if (CheckHitKey(KEY_INPUT_J)) newKey[P4_LEFT] = true;
	if (CheckHitKey(KEY_INPUT_I)) newKey[P4_A] = true;
	if (CheckHitKey(KEY_INPUT_V)) newKey[P4_B] = true;

	if (CheckHitKey(KEY_INPUT_RETURN)) newKey[ENTER] = true;
	if (CheckHitKey(KEY_INPUT_P)) newKey[PAUSE] = true;
	if (CheckHitKey(KEY_INPUT_SPACE)) newKey[START] = true;
	if (CheckHitKey(KEY_INPUT_BACK)) newKey[BACK] = true;

	// ----- trgKey upKey oldKey
	for (int i = 0; i < KEY_MAX; i++) {
		trgKey[i] = newKey[i] & ~oldKey[i];	// trgKey
		upKey[i] = ~newKey[i] & oldKey[i];	// upKey
		oldKey[i] = newKey[i];						// oldKey
	}
}