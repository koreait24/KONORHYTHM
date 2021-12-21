#pragma once

#include "resource.h"
#include "framework.h"
#include "Bitmap.h"
#include "Sound.h"

class KONORHYTHM 
{
public:
	KONORHYTHM();
	void start();
	void main();
	GAMESTATUS Getgstat() { return gstat; };
	LRESULT CALLBACK MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	GAMESTATUS gstat;

public:
	Sound* soundmain = new Sound(hWnd, _T("sound\\opening.mp3"), BGMID::GAMEHOME);
	Sound* soundstart = new Sound(hWnd, _T("sound\\start.mp3"), BGMID::GAMEMAIN);
	Sound* soundselect = new Sound(hWnd, _T("sound\\select.mp3"), BGMID::GAMESELECT);
	Sound* soundbutton = new Sound(hWnd, _T("sound\\button.mp3"), BGMID::GAMEBUTTON);
	Sound* soundmusicsel = new Sound(hWnd, _T("sound\\musicsel.mp3"), BGMID::GAMEMUSICSEL);
	Sound* sound1mmsymnor = new Sound(hWnd, _T("sound\\gamesong.mp3"), BGMID::INGAME_1MMSYM_NOR);

	HWND hWnd;
	Bitmap* bitmapMain;
	Bitmap* bitmapStart;
	BITMAPCOLLECTION collection;
	HINSTANCE hInst;                                // 현재 인스턴스입니다.

	std::unordered_map<int, bool> inopt;

	int fastidx = 0;
	int noteidx = 0;
};