#include "main.h"
#include "stage.h"
#include "player.h"

int mapImage[13];
XY mapPos;

int mapData[MAP_CHIP_Y][MAP_CHIP_X] = {
{ 8, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 8 },
{ 8, 1, 1, 1, 2, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 2, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 2, 1, 1,  1, 1, 1, 1, 1, 8 },
{ 8, 1, 1, 1, 1, 1,  1, 1, 2, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 2, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 2, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 2, 1, 1, 8 },
{ 8, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 2,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 2,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 8 },
{ 8, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 2,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 8 },

{ 8, 1, 1, 2, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 2, 1,  1, 1, 1, 2, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 2, 8 },
{ 8, 1, 1, 1, 1, 1,  1, 1, 1, 2, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 2, 1, 1,  1, 1, 1, 1, 2, 1,  1, 1, 1, 1, 1, 8 },
{ 8, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 8 },
{ 8, 1, 1, 1, 1, 2,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 2, 1, 1, 1, 1,  1, 1, 1, 1, 1, 2,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 2, 1, 1, 1, 8 },
{ 8, 1, 1, 1, 1, 1,  1, 8, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 2, 1, 1, 1, 1,  1, 1, 1, 1, 1, 8 },

{ 8, 1, 2, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 2, 1,  1, 1, 1, 1, 1, 1,  1, 1, 2, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 2, 1,  1, 1, 1, 1, 1, 8 },
{ 8, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 3, 1, 1, 1,  1, 1, 2, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 2, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 2, 1, 1, 8 },
{ 8, 1, 1, 1, 1, 1,  1, 2, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 8 },
{ 8, 8, 8, 8, 8, 8,  8, 8, 8, 8, 8, 8,  8, 8, 8, 8, 8, 8,  8, 8, 8, 8, 8, 8,  8, 8, 8, 8, 8, 8,  8, 8, 8, 8, 8, 8,  8, 8, 8, 8, 8, 8,  8, 8, 8, 8, 8, 8 },
{ 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 0,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 0 },
};

void StageSysInit(void) 
{

}

void StageInit(void)
{
	mapPos = { 0, 0 };
}

void StageUpdate(void)
{
	CHARACTER tmp = GetPlayer();

	// œØÃﬂÇÃΩ∏€∞Ÿêßå¿
	if (mapPos.y <= 0) {		// è„
		mapPos.y = 0;
	}

	if (mapPos.x >= MAP_CHIP_X * MAP_CHIP_SIZE_X - SCREEN_SIZE_X)	// âE
	{
		mapPos.x = MAP_CHIP_X * MAP_CHIP_SIZE_X - SCREEN_SIZE_X;
	}

	if (mapPos.y >= MAP_CHIP_Y * MAP_CHIP_SIZE_Y - SCREEN_SIZE_Y) {	// â∫
		mapPos.y = MAP_CHIP_Y * MAP_CHIP_SIZE_Y - SCREEN_SIZE_Y;
	}

	if (mapPos.x <= 0) {	// ç∂
		mapPos.x = 0;
	}
}

void StageDraw(void)
{
	DrawLine(0, MAP_CHIP_SIZE_Y * 18,SCREEN_SIZE_X, MAP_CHIP_SIZE_Y * 18, 0x00ff00, true);
	DrawBox(MAP_CHIP_SIZE_X * 7, MAP_CHIP_SIZE_Y * 15 - 48, MAP_CHIP_SIZE_X * 7 + 48, MAP_CHIP_SIZE_Y * 15, 0x00ff00, true);
	DrawBox(MAP_CHIP_SIZE_X * 7, MAP_CHIP_SIZE_Y * 18 - 48, MAP_CHIP_SIZE_X * 7 + 48, MAP_CHIP_SIZE_Y * 18, 0x00ff00, true);
	DrawFormatString(0, 48, 0x000000, "mapPos x:%d  y:%d", mapPos);
	
}

bool IsPass(XY pos)
{
	bool ret = true;
	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = mapData[mapIndex.y][mapIndex.x];

	switch (mapNo) {
	case 8:		// ï«
	case 10:	
		ret = false;
		break;
	}
	return ret;
}

bool HitIsPass(XY pos)
{
	bool ret = true;
	int mapNo;
	XY mapIndex;

	mapIndex = MapPosToIndex(pos);
	mapNo = mapData[mapIndex.y][mapIndex.x];

	switch (mapNo) {
	case 2:
		ret = false;
		break;
	}
	return ret;
}

XY MapPosToIndex(XY pos)
{
	XY mapIndex;
	mapIndex = { pos.x / MAP_CHIP_SIZE_X,  pos.y / MAP_CHIP_SIZE_Y };

	return mapIndex;
}

XY MapIndexToPos(XY index) {
	XY mapPos;
	mapPos = { index.x * MAP_CHIP_SIZE_X, index.y * MAP_CHIP_SIZE_Y };
	return mapPos;
}

XY MapPosToMoveIndex(XY pos, bool flag, XY_F velocity) {

	XY tmpIndex;
	XY tmpPos;
	tmpIndex = MapPosToIndex(pos);
	
	if (flag) {
		tmpIndex.y++;
	}
	if (velocity.x < 0) {			// ç∂
		tmpIndex.x++;
	}
	tmpPos = MapIndexToPos(tmpIndex);

	return tmpPos;
}

XY GetMapPos() {
	return mapPos;
}

void MapMove(XY pos, XY oldPos)
{
	int addX = abs(oldPos.x - pos.x);
	if (pos.x - mapPos.x < 48) {
		mapPos.x -= addX;
	}
	if (pos.x - mapPos.x > (SCREEN_SIZE_X - 48)) {
		mapPos.x += addX;
	}
}