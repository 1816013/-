#pragma once

#define EFFECT_SIZE_X 6
#define EFFECT_SIZE_Y 6




enum EFFECT_COLOR {
	EFFECT_C_YELLOW,
	EFFECT_C_MAGENTA,
	EFFECT_C_CYAN,
	EFFECT_C_RED,
	EFFECT_C_BLACK,
	EFFECT_C_MAX
};
// Ãﬂ€ƒ¿≤ÃﬂêÈåæ

void EffectSysInit(void);
void EffectInit(void);
void EffectUpdate(void);
void EffectDraw(void);
void TobichiriGenerator(XY pos);