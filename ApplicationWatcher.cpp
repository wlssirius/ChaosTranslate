
#include "ApplicationWatcher.h"
#include "winuser.h"
#include "memory.h"

RECT ApplicationWatcher::getWindowSize()
{
    RECT window_rect = { 0 };
    GetWindowRect(m_appHandle, &window_rect);
    return window_rect;
}

void ApplicationWatcher::setApplication(HWND windowPtr)
{
    m_appHandle = windowPtr;
}

PIX* ApplicationWatcher::capture() 
{
    if (!appSelected() || !IsWindow(m_appHandle))
    {
        m_appHandle = nullptr;
        return nullptr;
    }

    if (IsIconic(m_appHandle))
    {
        return nullptr;
    }

    RECT window_rect = { 0 };
    GetWindowRect(m_appHandle, &window_rect);

    // get the device context of the screen
    HDC hScreenDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
    // and a device context to put it in
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    int width = window_rect.right - window_rect.left;
    int height = window_rect.bottom - window_rect.top;

    // maybe worth checking these are positive values
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

    // get a new bitmap
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, window_rect.left, window_rect.top, SRCCOPY);
    hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

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
    GetDIBits(hMemoryDC, hBitmap, 0, height, bmp_pixels, &bmp_info, DIB_RGB_COLORS);
    
    // clean up
    DeleteDC(hMemoryDC);
    DeleteDC(hScreenDC);
    PIX* pix = pixCreate(width, height, 32);
    //std::shared_ptr<PIX> pixd(pix);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixSetRGBPixel(pix, x, height - y - 1, bmp_pixels[(width * 3 + bmp_padding) * y + 3 * x+2], bmp_pixels[(width * 3 + bmp_padding) * y + 3 * x+1], bmp_pixels[(width * 3 + bmp_padding) * y + 3 * x]);
        }
    }
    delete[] bmp_pixels;
    return pix;
}


std::vector<ApplicationWatcher::appInfo> ApplicationWatcher::getAppInfoList()
{
    auto EnumWindowsProc = [](HWND hWnd, LPARAM lParam)
    {
        auto apps = (std::vector<appInfo>*)lParam;
        int length = GetWindowTextLengthA(hWnd);
        std::string title(length + 1, NULL);
        GetWindowTextA(hWnd, const_cast<char*>(title.c_str()), length + 1);
        HICON icon = reinterpret_cast<HICON>(::SendMessageW(hWnd, WM_GETICON, ICON_SMALL, 0));
        if (icon == nullptr)
        {
            icon = (HICON)GetClassLong(hWnd, GCLP_HICON);
        }
        if (length > 0 && icon!=NULL)  
        {
            int length = GetWindowTextLengthA(hWnd);
            appInfo info;
            info.icon = icon;
            info.name = title;
            info.ptr = hWnd;
            apps->push_back(info);
        }
        return TRUE;
    };

    std::vector<appInfo> apps;
    EnumWindows(EnumWindowsProc, (LPARAM)&apps);
    return apps;
}

bool ApplicationWatcher::appSelected()
{ 
    if (!m_appHandle || !IsWindow(m_appHandle))
    {
        m_appHandle = nullptr;
        return false;
    }
    return true;
}
