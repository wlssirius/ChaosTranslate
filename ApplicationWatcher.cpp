#include "ApplicationWatcher.h"

RECT ApplicationWatcher::getWindowSize()
{
    RECT window_rect = { 0 };
    GetClientRect(m_appHandle, &window_rect);
    return window_rect;
}

PIX* ApplicationWatcher::capture(RECT roi) {
    RECT client_rect = { 0 };
    GetClientRect(m_appHandle, &client_rect);
    int windowWidth = client_rect.right - client_rect.left;
    int windowHeight = client_rect.bottom - client_rect.top;

    HDC hdcScreen = GetDC(m_appHandle);
    HDC hdc = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen, windowWidth, windowHeight);
    SelectObject(hdc, hbmp);

    int width = windowWidth;
    int height = windowHeight;

    BitBlt(hdc, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    BITMAPINFO bmp_info = { 0 };
    bmp_info.bmiHeader.biSize = sizeof(bmp_info.bmiHeader);
    bmp_info.bmiHeader.biWidth = width;
    bmp_info.bmiHeader.biHeight = height;
    bmp_info.bmiHeader.biPlanes = 1;
    bmp_info.bmiHeader.biBitCount = 24;
    bmp_info.bmiHeader.biCompression = BI_RGB;

    int bmp_padding = (width * 3) % 4;
    if (bmp_padding != 0) bmp_padding = 4 - bmp_padding;

    BYTE* bmp_pixels = new BYTE[(width * 3 + bmp_padding) * height];
    GetDIBits(hdc, hbmp, 0, height, bmp_pixels, &bmp_info, DIB_RGB_COLORS);

    Pix* pixd = pixCreate(width, height, 32);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixSetRGBPixel(pixd, x, height - y - 1, bmp_pixels[(width * 3 + bmp_padding) * y + 3 * x+2], bmp_pixels[(width * 3 + bmp_padding) * y + 3 * x+1], bmp_pixels[(width * 3 + bmp_padding) * y + 3 * x]);
        }
    }
    PIX* pix1 = pixRead("D:\\TestFile1.bmp");
    pixWrite("D:/pixd.png", pixd, IFF_PNG);

    delete[] bmp_pixels;
    return pixd;
}

