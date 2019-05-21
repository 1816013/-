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

// ----------- ¸ŞÛ°ÊŞÙ•Ï”
extern bool newKey[KEY_MAX];
extern bool trgKey[KEY_MAX];
extern bool oldKey[KEY_MAX];
extern bool upKey[KEY_MAX];

// ----------- ÌßÛÄÀ²ÌßéŒ¾


// keycheck.cpp

void KeyCheckInit(void);	// ·°Áª¯¸‚Ì‰Šú‰»
void KeyCheck(void); //·°“ü—Íó‘Ô‚ÌÁª¯¸