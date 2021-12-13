#include "Sound.h"

#pragma once
Sound::Sound(HWND hWnd, LPCTSTR lpszWave)
{
        LoadWAV(hWnd, lpszWave);
}

DWORD Sound::LoadWAV(HWND hWnd, LPCTSTR lpszWave)
{
    mciOpen.lpstrDeviceType = _T("mpegvideo");
    mciOpen.lpstrElementName = lpszWave;

    Result = mciSendCommand(wDeviceID, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&mciOpen);

    if (Result)
        return Result;

    wDeviceID = mciOpen.wDeviceID;

    mciPlay.dwCallback = (DWORD)hWnd;

    if (Result)
        return Result;

    return 0;
}

void Sound::play() 
{
    BGM = mciSendCommand(1, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay);
}

void Sound::end()
{
    if (wDeviceID > 0)
    {
        BGM = mciSendCommand(1, MCI_CLOSE, 0, (DWORD)(LPVOID)NULL);
    }
}