#pragma once
#include "framework.h"
class Bitmap
{
public:
	Bitmap(HINSTANCE& hInst, int res);
	void DRAWBITMAP(HDC& hdc, int x, int y, bool isTran = false);
	void end();
	void EffectBlink(bool isTran = TRUE);

public:
	HDC hdc, CopyDC, MemDC, ObjDC; // Ahdc는 Transeparents 저장 용
	PAINTSTRUCT ps;
	HBITMAP MyBitmap, CopyOldBitmap, MemOldBitmap, ObjOldBitmap;
	int x, y;			// 이미지 그릴 좌표
	bool isContinue;
	int bx, by;
	BITMAP bit;
};

