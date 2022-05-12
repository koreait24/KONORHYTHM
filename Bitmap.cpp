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

void Bitmap::INITBITMAP(HDC& hdc, std::vector<std::pair<int, int>> pos, bool isTran) 
{
    this->hdc = hdc;

    int index = 0;
    BITMAPINFO bitinfo;
    for (const auto& it : res) {
        MemDC.push_back(CreateCompatibleDC(hdc));
        SelectObject(MemDC[index], MyBitmap[index]);

        bit.push_back(iBit);
        GetObject(MyBitmap[index], sizeof(BITMAP), &bit[index]);
        bitinfo.bx = bit[index].bmWidth;
        bitinfo.by = bit[index].bmHeight;
        bitinfo.x = pos[index].first;
        bitinfo.y = pos[index].second;

        BITINFO.push_back(bitinfo);
        index++;
    }
    this->pos = pos;
}

void Bitmap::DRAWBITMAPALL(HDC &hdc, bool isTran)
{
    {
        std::unique_lock<std::mutex> EffectLock(EffectMutex);
        this->hdc = hdc;

        CopyDC = CreateCompatibleDC(hdc);
        hCopy = CreateCompatibleBitmap(hdc, bit[0].bmWidth, bit[0].bmHeight);
        SelectObject(CopyDC, hCopy);

        int index = 0;
        for (const auto& it : res) {
            int x = BITINFO[index].x;
            int y = BITINFO[index].y;
            int bx = BITINFO[index].bx;
            int by = BITINFO[index].by;
            if (it.second) {
                TransparentBlt(CopyDC, x, y, bx, by, MemDC[index], 0, 0, bx, by, RGB(255, 242, 204));
            }
            else
                BitBlt(CopyDC, x, y, bx, by, MemDC[index], 0, 0, SRCCOPY);
            index++;
        }
        BitBlt(hdc, 0, 0, bit[0].bmWidth, bit[0].bmHeight, CopyDC, 0, 0, SRCCOPY);
    }
}

void Bitmap::DRAWBITMAP(HWND hWnd,HDC& hdc, int residx)
{
    {
        std::unique_lock<std::mutex> EffectLock(EffectMutex);
        int x = BITINFO[residx].x;
        int y = BITINFO[residx].y;
        int bx = BITINFO[residx].bx;
        int by = BITINFO[residx].by;
        RECT EFFECTRECT = { x, y, bx, by };
        if (BITINFO[residx].isTran)
            TransparentBlt(hdc, x, y, bx, by, MemDC[residx], 0, 0, bx, by, RGB(255, 242, 204));
        else
            BitBlt(hdc, x, y, bx, by, MemDC[residx], 0, 0, SRCCOPY);
        InvalidateRect(hWnd, &EFFECTRECT, FALSE);
    }
}

void Bitmap::EffectBlink(HWND hWnd)
{
    {
        std::unique_lock<std::mutex> EffectLock(EffectMutex);
        BLENDFUNCTION bf;

        bf.AlphaFormat = 0; // 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
        bf.BlendFlags = 0; // 무조건 0이어야 한다
        bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미

        CopyDC = CreateCompatibleDC(hdc);
        hCopy = CreateCompatibleBitmap(hdc, bit[0].bmWidth, bit[0].bmHeight);
        SelectObject(CopyDC, hCopy);

        RECT EFFECTRECT;

        for (unsigned int i = 255; i >= 50; i -= 20) {
            for (unsigned int j = 0; j < MyBitmap.size(); j++) {
                int x = BITINFO[j].x;
                int y = BITINFO[j].y;
                int bx = BITINFO[j].bx;
                int by = BITINFO[j].by;
                EFFECTRECT = { x, y, bx, by };
                bf.SourceConstantAlpha = i; // 투명도(투명 0 - 불투명 255)
                BitBlt(CopyDC, 0, 0, bx, by, MemDC[0], x, y, SRCCOPY);
                TransparentBlt(CopyDC, 0, 0, bx, by, MemDC[j], 0, 0, bx, by, RGB(255, 242, 204));
                BitBlt(hdc, 0, 0, bit[0].bmWidth, bit[0].bmHeight, MemDC[0], 0, 0, SRCCOPY);
                AlphaBlend(hdc, x, y, bx, by, CopyDC, 0, 0, bx, by, bf);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }
        DeleteObject(CopyDC);
    }
}

void Bitmap::EffectFadeout(HWND hWnd)
{
    {
        std::unique_lock<std::mutex> EffectLock(EffectMutex);
        BLENDFUNCTION bf;

        bf.AlphaFormat = 0; // 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
        bf.BlendFlags = 0; // 무조건 0이어야 한다
        bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미

        BaseDC = CreateCompatibleDC(hdc);
        hBase = CreateCompatibleBitmap(hdc, bit[0].bmWidth, bit[0].bmHeight);
        SelectObject(BaseDC, hBase);

        RECT EFFECTRECT = { 0, 0, bit[0].bmWidth, bit[0].bmHeight };

        for (unsigned int i = 255; i >= 50; i -= 20) {
            BitBlt(BaseDC, 0, 0, bit[0].bmWidth, bit[0].bmHeight, MemDC[0], 0, 0, SRCCOPY);
            for (unsigned int j = 1; j < MyBitmap.size(); j++) {
                int x = BITINFO[j].x;
                int y = BITINFO[j].y;
                int bx = BITINFO[j].bx;
                int by = BITINFO[j].by;
                TransparentBlt(BaseDC, x, y, bx, by, MemDC[j], 0, 0, bx, by, RGB(255, 242, 204));
            }
            bf.SourceConstantAlpha = i;
            AlphaBlend(hdc, 0, 0, bit[0].bmWidth, bit[0].bmHeight, BaseDC, 0, 0, bit[0].bmWidth, bit[0].bmHeight, bf);
            InvalidateRect(hWnd, NULL, FALSE);
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
        DeleteObject(BaseDC);
    }
}

void Bitmap::EffectFadein(HWND hWnd, int end)
{
    {
        std::unique_lock<std::mutex> EffectLock(EffectMutex);
        BLENDFUNCTION bf;

        bf.AlphaFormat = 0; // 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
        bf.BlendFlags = 0; // 무조건 0이어야 한다
        bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미

        BaseDC = CreateCompatibleDC(hdc);
        hBase = CreateCompatibleBitmap(hdc, bit[0].bmWidth, bit[0].bmHeight);
        SelectObject(BaseDC, hBase);

        RECT EFFECTRECT = { 0, 0, bit[0].bmWidth, bit[0].bmHeight };

        for (unsigned int i = 55; i < 255; i += 20) {
            BitBlt(BaseDC, 0, 0, bit[0].bmWidth, bit[0].bmHeight, MemDC[0], 0, 0, SRCCOPY);
            for (int j = 0; j < end; j++) {
                int x = BITINFO[j].x;
                int y = BITINFO[j].y;
                int bx = BITINFO[j].bx;
                int by = BITINFO[j].by;
                TransparentBlt(BaseDC, x, y, bx, by, MemDC[j], 0, 0, bx, by, RGB(255, 242, 204));
            }
            bf.SourceConstantAlpha = i;
            AlphaBlend(hdc, 0, 0, bit[0].bmWidth, bit[0].bmHeight, BaseDC, 0, 0, bit[0].bmWidth, bit[0].bmHeight, bf);
            InvalidateRect(hWnd, NULL, FALSE);
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }
        DeleteObject(BaseDC);
    }
}

void Bitmap::EffectFadeinBlack(HWND hWnd)
{
    {
        std::unique_lock<std::mutex> EffectLock(EffectMutex);
        BLENDFUNCTION bf;

        bf.AlphaFormat = 0; // 비트맵 종류로 일반 비트맵의 경우 0, 32비트 비트맵의 경우 AC_SRC_ALPHA
        bf.BlendFlags = 0; // 무조건 0이어야 한다
        bf.BlendOp = AC_SRC_OVER; // 무조건 AC_SRC_OVER이어야 하고 원본과 대상 이미지를 합친다는 의미

        BaseDC = CreateCompatibleDC(hdc);
        hBase = CreateCompatibleBitmap(hdc, bit[0].bmWidth, bit[0].bmHeight);
        SelectObject(BaseDC, hBase);

        RECT EFFECTRECT = { 0, 0, bit[0].bmWidth, bit[0].bmHeight };
        HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
        HBRUSH oldBrush = (HBRUSH)SelectObject(BaseDC, myBrush);

        for (unsigned int i = 55; i < 255; i += 5) {
            BitBlt(hdc, 0, 0, bit[0].bmWidth, bit[0].bmHeight, MemDC[0], 0, 0, SRCCOPY);
            for (unsigned int j = 1; j < MyBitmap.size(); j++) {
                int x = BITINFO[j].x;
                int y = BITINFO[j].y;
                int bx = BITINFO[j].bx;
                int by = BITINFO[j].by;
                TransparentBlt(hdc, x, y, bx, by, MemDC[j], 0, 0, bx, by, RGB(255, 242, 204));
            }
            bf.SourceConstantAlpha = i;
            Rectangle(BaseDC, 0, 0, bit[0].bmWidth, bit[0].bmHeight);
            AlphaBlend(hdc, 0, 0, bit[0].bmWidth, bit[0].bmHeight, BaseDC, 0, 0, bit[0].bmWidth, bit[0].bmHeight, bf);
            InvalidateRect(hWnd, NULL, FALSE);
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }
        DeleteObject(BaseDC);
    }
}

void Bitmap::end()
{
    delete this;
}