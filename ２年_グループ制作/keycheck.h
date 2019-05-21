#pragma once
#include "main.h"

enum KEY_MODE {
	P1_UP,
	P1_RIGHT,
	P1_DOWN,
	P1_LEFT,
	P1_A,

	P2_UP,
	P2_RIGHT,
	P2_DOWN,
	P2_LEFT,
	P2_A,

	P3_UP,
	P3_RIGHT,
	P3_DOWN,
	P3_LEFT,
	P3_A,

	P4_UP,
	P4_RIGHT,
	P4_DOWN,
	P4_LEFT,
	P4_A,

	ENTER,
	PAUSE,
	START,

	KEY_MAX
};

// ----------- ��۰��ٕϐ�
extern bool newKey[KEY_MAX];
extern bool trgKey[KEY_MAX];
extern bool oldKey[KEY_MAX];
extern bool upKey[KEY_MAX];

// ----------- �������ߐ錾


// keycheck.cpp

void KeyCheckInit(void);	// �������̏�����
void KeyCheck(void); //�����͏�Ԃ�����