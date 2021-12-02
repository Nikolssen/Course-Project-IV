#pragma once
#include <Windows.h>

class CalotteCanvas
{
public:
    CalotteCanvas();
    HWND GetHwnd() { return m_hwnd; }

private:
    HWND m_hwnd;
    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

