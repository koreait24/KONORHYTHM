#pragma once
#include "framework.h"
class Bitmap
{
public:
	Bitmap(HINSTANCE& hInst, int res);
	void DRAWBITMAP(HDC hdc, int x, int y);
	void end();

public:
	HDC hdc;
	PAINTSTRUCT ps;
	HBITMAP MyBitmap;
};

