#pragma once
#include "KONORHYTHM.h"

KONORHYTHM::KONORHYTHM() 
{
	gstat = GAMESTATUS::GAMEHOME;
}

void KONORHYTHM::start()
{
    inopt.insert(std::make_pair(IDB_VGA, FALSE));
    inopt.insert(std::make_pair(IDB_BOARD, FALSE));
    inopt.insert(std::make_pair((int)collection.fastset[fastidx], FALSE));
    inopt.insert(std::make_pair((int)collection.noteset[noteidx], FALSE));
    inopt.insert(std::make_pair(IDB_OPTNM, FALSE));
}

void KONORHYTHM::main()
{
    gstat = GAMESTATUS::GAMEHOME;
}

LRESULT CALLBACK KONORHYTHM::MainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case VK_RETURN:
            if(gstat == GAMESTATUS::GAMEHOME)
                gstat = GAMESTATUS::GAMEMAIN;
            else if (gstat == GAMESTATUS::GAMEMAIN) {
                gstat = GAMESTATUS::GAMESTART;
                start();
            }
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case 0x31:
            if (gstat == GAMESTATUS::GAMEMAIN) {
                fastidx++;
                if (fastidx > 4)
                    fastidx = 0;
                RECT EFFECTRECT = { 0, 0, 210, 470 };
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (gstat == GAMESTATUS::GAMESTART) {
                RECT EFFECTRECT = { 0, 0, 700, 30 };
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        case 0x32:
            if (gstat == GAMESTATUS::GAMEMAIN) {
                noteidx++;
                if (noteidx > 1)
                    noteidx = 0;
                RECT EFFECTRECT = { 0, 0, 90, 470 };
                InvalidateRect(hWnd, NULL, TRUE);
            }
            else if (gstat == GAMESTATUS::GAMESTART) {
                RECT EFFECTRECT = { 0, 0, 730, 30 };
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;
        }
    }
    //윈도우 프로시저에서 처리되지 않은 나머지 메시지를 처리해준다
    return (DefWindowProc(hWnd, message, wParam, lParam));
}