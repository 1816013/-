//#pragma once

#define MAP_CHIP_X 48
#define MAP_CHIP_Y 15

#define MAP_CHIP_SIZE_X 48
#define MAP_CHIP_SIZE_Y 48

extern XY mapPos;
// Ãﬂ€ƒ¿≤ÃﬂêÈåæ
void StageSysInit(void);
void StageInit(void);
void StageUpdate(void);
void StageDraw(void);
bool IsPass(XY pos);
XY MapPosToIndex(XY pos);
XY MapIndexToPos(XY index);
XY MapPosToMoveIndex(XY pos, bool flag, XY_F velocity);
XY GetMapPos();
void MapMove(XY pos, XY oldPos);
bool HitIsPass(XY pos);