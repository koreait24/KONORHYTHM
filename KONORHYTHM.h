#pragma once

#include "resource.h"
#include "framework.h"

class KONORHYTHM 
{
public:
	KONORHYTHM();
	void start();
	GAMESTATUS Getgstat() { return gstat; };
	LRESULT CALLBACK MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	GAMESTATUS gstat;
};