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
	HDC hdc, CopyDC, MemDC, ObjDC; // Ahdc�� Transeparents ���� ��
	PAINTSTRUCT ps;
	HBITMAP MyBitmap, CopyOldBitmap, MemOldBitmap, ObjOldBitmap;
	int x, y;			// �̹��� �׸� ��ǥ
	bool isContinue;
	int bx, by;
	BITMAP bit;
};

