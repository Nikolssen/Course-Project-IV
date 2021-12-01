#pragma once
#include <Windows.h>
#include "SkeletonCanvas.h"

class Win32Application
{
public:
    static int Run(SkeletonCanvas* canvas, HINSTANCE hInstance, int nCmdShow);
    static HWND GetHwnd() { return m_hwnd; }

protected:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    static HWND m_hwnd;
};

