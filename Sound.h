#pragma once

#include "framework.h"

class Sound
{
public:
	Sound(HWND hWnd, LPCTSTR lpszWave);
	void play();
	void end();

private:
	DWORD LoadWAV(HWND hWnd, LPCTSTR lpszWave);

public:
	MCI_OPEN_PARMS       mciOpen; //������ �ε�
	MCI_PLAY_PARMS       mciPlay; //������ ���
	MCI_STATUS_PARMS     mciStatus; //������ ����

	UINT wDeviceID = 0;

	DWORD BGM = 0;
	DWORD Result;
};

