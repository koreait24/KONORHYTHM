#include "Bitmap.h"

Bitmap::Bitmap(HINSTANCE& hInst, int res)
{
    MyBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(res));
}

void Bitmap::DRAWBITMAP(HDC hdc, int x, int y)
{
    HDC MemDC;
    HBITMAP OldBitmap;
    int bx, by;
    BITMAP bit;

    MemDC = CreateCompatibleDC(hdc);
    OldBitmap = (HBITMAP)SelectObject(MemDC, MyBitmap);

    GetObject(MyBitmap, sizeof(BITMAP), &bit);
    bx = bit.bmWidth;
    by = bit.bmHeight;

    BitBlt(hdc, 0, 0, bx, by, MemDC, 0, 0, SRCCOPY);
    SelectObject(MemDC, OldBitmap);
    DeleteDC(MemDC);
}

void Bitmap::end()
{
    DeleteObject(MyBitmap);
}