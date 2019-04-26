#pragma once

#define CHIP_SIZE_X 48
#define CHIP_SIZE_Y 48

extern XY mapPos;

enum EVENT_MODE{
	E_WALL,
	E_NEEDLE
};



// Ãﬂ€ƒ¿≤ÃﬂêÈåæ
void stageSysInit(void);
bool IsPass(XY pos);
XY MapPosToIndex(XY pos);
XY MapIndexToPos(XY index);
XY MapPosToMoveIndex(XY pos, bool flag, XY_F velocity);
XY GetMapPos();
bool TelIsPass(XY pos);
bool HitIsPass(XY pos);
bool NeedleIsPass(XY pos);
bool JumpIsPass(XY pos);
bool GoalIsPass(XY pos);

void stageInit(void);
void stageMain(void);
void stageDraw(void);