#include "Bitmap.h"

Bitmap::Bitmap(HINSTANCE& hInst, int res)
{
    MyBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(res));
}

void Bitmap::DRAWBITMAP(HDC &hdc, int x, int y, bool isTran)
{
    MemDC           = CreateCompatibleDC(hdc);
    CopyDC          = CreateCompatibleDC(hdc);
    ObjDC           = CreateCompatibleDC(hdc);
    MemOldBitmap    = (HBITMAP)SelectObject(MemDC, MyBitmap);
    CopyOldBitmap   = (HBITMAP)SelectObject(CopyDC, MyBitmap);
    ObjOldBitmap    = (HBITMAP)SelectObject(ObjDC, MyBitmap);
    GetObject(MyBitmap, sizeof(BITMAP), &bit);

    bx          = bit.bmWidth;
    by          = bit.bmHeight;
    this->x     = x;
    this->y     = y;
    this->hdc   = hdc;

    if (isTran) {
        TransparentBlt(hdc, x, y, bx, by, MemDC, 0, 0, bx, by, RGB(255, 242, 204));
    }
    else
        BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);
}

void Bitmap::EffectBlink(bool isTran)
{
    BLENDFUNCTION bf;

    isContinue = true;
    bf.AlphaFormat = 0; // ��Ʈ�� ������ �Ϲ� ��Ʈ���� ��� 0, 32��Ʈ ��Ʈ���� ��� AC_SRC_ALPHA
    bf.BlendFlags = 0; // ������ 0�̾�� �Ѵ�
    bf.BlendOp = AC_SRC_OVER; // ������ AC_SRC_OVER�̾�� �ϰ� ������ ��� �̹����� ��ģ�ٴ� �ǹ�
    if (isTran) {
        TransparentBlt(CopyDC, x, y, bx, by, MemDC, 0, 0, bx, by, RGB(255, 242, 204));
    }
    else
        BitBlt(CopyDC, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

    while (isContinue) {
        for (int i = 255; i >= 50; i -= 30) {
            bx = bit.bmWidth;
            by = bit.bmHeight;
            bf.SourceConstantAlpha = i; // ����(���� 0 - ������ 255)
            AlphaBlend(hdc, i, y, bx, by, CopyDC, 0, 0, bx, by, bf);
            Sleep(1000);
        }
        for (int i = 50; i <= 230; i += 30) {
            bx = bit.bmWidth;
            by = bit.bmHeight;
            bf.SourceConstantAlpha = i;
            AlphaBlend(hdc, i, y, bx, by, CopyDC, 0, 0, bx, by, bf);
            Sleep(1000);
        }
    }
    DeleteObject(MyBitmap);
}

void Bitmap::end()
{
    SelectObject(MemDC, MemOldBitmap);
    DeleteDC(MemDC);
    SelectObject(CopyDC, CopyOldBitmap);
    DeleteDC(CopyDC);
    SelectObject(ObjDC, ObjOldBitmap);
    DeleteDC(ObjDC);
    isContinue = FALSE;
    delete this;
}