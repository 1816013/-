#pragma once

#define CHIP_SIZE_X 48
#define CHIP_SIZE_Y 48





enum STAGE_NUM {
	STAGE1,
	STAGE2,
	STAGE3,
	STAGE4,
	STAGE5,
	EX_STAGE1,
	STAGE_MAX
};


// �������ߐ錾
void stageSysInit(void);
bool IsPass(XY pos);
XY MapPosToIndex(XY pos);
XY MapIndexToPos(XY index);
XY MapPosToMoveIndex(XY pos, bool flag, XY_F velocity);
XY GetMapPos();
bool TelIsPass(XY pos);
bool NeedleIsPass(XY pos);
bool JumpIsPass(XY pos);
bool GoalIsPass(XY pos);
bool SaveIsPass(XY pos);
CHARACTER GetTrap(int idx);

void stageInit(void);
void trapInit(int);
void stageMain(void);
void stageDraw(void);
