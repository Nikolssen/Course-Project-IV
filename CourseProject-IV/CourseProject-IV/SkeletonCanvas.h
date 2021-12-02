#pragma once
#include <Windows.h>

class SkeletonCanvas
{
    SkeletonCanvas();
    HWND GetHwnd() { return m_hwnd; }
    void Configure();
private:
    HWND m_hwnd;
    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

