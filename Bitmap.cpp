#include "Bitmap.h"
#include "resource.h"

Bitmap::Bitmap(HINSTANCE& hInst, std::unordered_map<int, bool> res)
{
    for (const auto &it : res)
    {
        MyBitmap.push_back(LoadBitmap(hInst, MAKEINTRESOURCE(it.first)));
    }
    this->res.insert(res.begin(), res.end());
}

void Bitmap::INITBITMAP(HDC& hdc, int x, int y, bool isTran) 
{
    this->hdc = hdc;

    int index = 0;
    for (const auto& it : res) {
        MemDC.push_back(CreateCompatibleDC(hdc));
        SelectObject(MemDC[index], MyBitmap[index]);

        bit.push_back(iBit);
        GetObject(MyBitmap[index], sizeof(BITMAP), &bit[index]);
        bx = bit[index].bmWidth;
        by = bit[index].bmHeight;
        this->x = x;
        this->y = y;
        index++;
    }
}

void Bitmap::DRAWBITMAP(HDC &hdc, int x, int y, bool isTran)
{
    this->hdc = hdc;

    int index = 0;
    for (const auto &it : res) 
    {
        if (index == 0) {
            if (it.second) {
                TransparentBlt(hdc, 0, 0, bx, by, MemDC[index], 0, 0, bx, by, RGB(255, 242, 204));
            }
            else
                BitBlt(hdc, 0, 0, bx, by, MemDC[index], 0, 0, SRCCOPY);
        }

        else {
            if (it.second) {
                TransparentBlt(hdc, x, y, bx, by, MemDC[index], 0, 0, bx, by, RGB(255, 242, 204));
            }
            else
                BitBlt(hdc, x, y, bx, by, MemDC[index], 0, 0, SRCCOPY);
        }
        index++;
    }
}

void Bitmap::EffectBlink(HWND hWnd)
{
    // std::lock_guard<std::mutex> EffectLock (EffectMutex);
    BLENDFUNCTION bf;

    bf.AlphaFormat = 0; // 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
    bf.BlendFlags = 0; // 무조건 0이어야 한다
    bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미

    CopyDC = CreateCompatibleDC(hdc);
    hCopy = CreateCompatibleBitmap(hdc, bx, by);
    SelectObject(CopyDC, hCopy);

    RECT EFFECTRECT;

    for (unsigned int i = 255; i >= 50; i -= 10) {
        for (unsigned int j = 0; j < MyBitmap.size(); j++) {
            bx = bit[j].bmWidth;
            by = bit[j].bmHeight;
            EFFECTRECT = { x, y, bx, by };
            bf.SourceConstantAlpha = i; // 투명도(투명 0 - 불투명 255)
            BitBlt(CopyDC, 0, 0, bx, by, MemDC[0], x, y, SRCCOPY);
            TransparentBlt(CopyDC, 0, 0, bx, by, MemDC[j], 0, 0, bx, by, RGB(255, 242, 204));
            BitBlt(hdc, 0, 0, bit[0].bmWidth, bit[0].bmHeight, MemDC[0], 0, 0, SRCCOPY);
            AlphaBlend(hdc, x, y, bx, by, CopyDC, 0, 0, bx, by, bf);
            InvalidateRect(hWnd, &EFFECTRECT, FALSE);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Bitmap::EffectFadeout(HWND hWnd)
{
    // std::lock_guard<std::mutex> EffectLock (EffectMutex);
    BLENDFUNCTION bf;

    bf.AlphaFormat = 0; // 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
    bf.BlendFlags = 0; // 무조건 0이어야 한다
    bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미

    BaseDC = CreateCompatibleDC(hdc);
    hBase = CreateCompatibleBitmap(hdc, bit[0].bmWidth, bit[0].bmHeight);
    SelectObject(BaseDC, hBase);

    RECT EFFECTRECT = { 0, 0, bit[0].bmWidth, bit[0].bmHeight };

    for (unsigned int i = 255; i >= 50; i -= 10) {
        BitBlt(BaseDC, 0, 0, bit[0].bmWidth, bit[0].bmHeight, MemDC[0], 0, 0, SRCCOPY);
        for (unsigned int j = 1; j < MyBitmap.size(); j++) {
            bx = bit[j].bmWidth;
            by = bit[j].bmHeight;
            TransparentBlt(BaseDC, x, y, bx, by, MemDC[j], 0, 0, bx, by, RGB(255, 242, 204));
        }
        bf.SourceConstantAlpha = i;
        AlphaBlend(hdc, 0, 0, bit[0].bmWidth, bit[0].bmHeight, BaseDC, 0, 0, bit[0].bmWidth, bit[0].bmHeight, bf);
        InvalidateRect(hWnd, &EFFECTRECT, FALSE);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Bitmap::EffectFadein(HWND hWnd)
{
    // std::lock_guard<std::mutex> EffectLock (EffectMutex);
    BLENDFUNCTION bf;

    bf.AlphaFormat = 0; // 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
    bf.BlendFlags = 0; // 무조건 0이어야 한다
    bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미

    BaseDC = CreateCompatibleDC(hdc);
    hBase = CreateCompatibleBitmap(hdc, bit[0].bmWidth, bit[0].bmHeight);
    SelectObject(BaseDC, hBase);

    RECT EFFECTRECT = { 0, 0, bit[0].bmWidth, bit[0].bmHeight };

    for (unsigned int i = 55; i < 255; i += 10) {
        BitBlt(BaseDC, 0, 0, bit[0].bmWidth, bit[0].bmHeight, MemDC[0], 0, 0, SRCCOPY);
            for (unsigned int j = 1; j < MyBitmap.size(); j++) {
                bx = bit[j].bmWidth;
                by = bit[j].bmHeight;
                TransparentBlt(BaseDC, x, y, bx, by, MemDC[j], 0, 0, bx, by, RGB(255, 242, 204));
            }
            bf.SourceConstantAlpha = i;
            AlphaBlend(hdc, 0, 0, bit[0].bmWidth, bit[0].bmHeight, BaseDC, 0, 0, bit[0].bmWidth, bit[0].bmHeight, bf);
            InvalidateRect(hWnd, &EFFECTRECT, FALSE);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void Bitmap::end()
{
    delete this;
}