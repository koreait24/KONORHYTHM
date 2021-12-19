#pragma once

#include "framework.h"

class Sound
{
public:
	Sound(HWND hWnd, LPCTSTR lpszWave, BGMID bgm);
	void stop();
	void play();
	void end();

private:
	DWORD LoadWAV(HWND hWnd, LPCTSTR lpszWave);

private:
	MCI_OPEN_PARMS       mciOpen; //파일을 로드
	MCI_PLAY_PARMS       mciPlay; //파일을 재생
	MCI_STATUS_PARMS     mciStatus; //파일의 상태

	int wDeviceID = 0;
	
	BGMID bgm;
	DWORD BGM = 0;
	DWORD Result;
};

