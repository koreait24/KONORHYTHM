#pragma once
#include "framework.h"
class Bitmap
{
public:
	Bitmap(HINSTANCE& hInst, std::unordered_map<int, bool> res);
	void DRAWBITMAPALL(HDC& hdc, bool isTran = false);
	void DRAWBITMAP(HWND hWnd, HDC& hdc, int residx);
	void INITBITMAP(HDC& hdc, std::vector<std::pair<int, int>> pos, bool isTran = false);
	void end();

public:										// 비트맵 애니메이션 섹션
	void EffectBlink(HWND hWnd);
	void EffectFadein(HWND hWnd, int end);
	void EffectFadeinBlack(HWND hWnd);
	void EffectFadeout(HWND hWnd);

public:
	HDC						hdc, CopyDC, BaseDC; // Ahdc는 Transeparents 저장 용
	PAINTSTRUCT				ps;
	BITMAP					iBit;
	HBITMAP					hCopy, hBase;
	std::vector<HBITMAP>	MyBitmap;
	std::vector<BITMAP>		bit;
	std::vector<HDC>		MemDC;

	std::unordered_map<int, bool>	 res;
	std::vector<std::pair<int, int>> pos;
	std::thread					  Blink;
	std::mutex					  EffectMutex;

	struct BITMAPINFO {
		int		bx = 0, by = 0;
		int		x  = 0, y  = 0;
		int		isTran = FALSE;
	};

	std::vector<BITMAPINFO> BITINFO;
};
