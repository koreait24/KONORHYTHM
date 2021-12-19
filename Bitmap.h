#pragma once
#include "framework.h"
class Bitmap
{
public:
	Bitmap(HINSTANCE& hInst, std::unordered_map<int, bool> res);
	void DRAWBITMAP(HDC& hdc, int x, int y, bool isTran = false);
	void INITBITMAP(HDC& hdc, int x, int y, bool isTran = false);
	void end();

public:										// 비트맵 애니메이션 섹션
	bool GetEffectBlink();
	void EffectBlink(HWND hWnd);
	void EffectFadein(HWND hWnd);
	void EffectFadeout(HWND hWnd);

public:
	HDC						hdc, CopyDC, BaseDC; // Ahdc는 Transeparents 저장 용
	PAINTSTRUCT				ps;
	BITMAP					iBit;
	HBITMAP					hCopy, hBase;
	std::vector<HBITMAP>	MyBitmap;
	std::vector<BITMAP>		bit;
	std::vector<HDC>		MemDC;
	int						x, y;			// 이미지 그릴 좌표
	int						bx, by;

	std::unordered_map<int, bool> res;
	std::thread					  Blink;
	std::mutex					  EffectMutex;
private:
};
