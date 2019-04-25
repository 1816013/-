#pragma once

#define CHIP_SIZE_X 48
#define CHIP_SIZE_Y 48

extern XY mapPos;
// Ãﬂ€ƒ¿≤ÃﬂêÈåæ
//void StageSysInit(void);
//void StageInit(void);
//void StageUpdate(void);
//void StageDraw(void);
bool IsPass(XY pos);
XY MapPosToIndex(XY pos);
XY MapIndexToPos(XY index);
XY MapPosToMoveIndex(XY pos, bool flag, XY_F velocity);
XY GetMapPos();
bool TelIsPass(XY pos);
bool HitIsPass(XY pos);


void stageInit(void);
void stageMain(void);
void stageDraw(void);