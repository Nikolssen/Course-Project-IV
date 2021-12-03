#pragma once
#include <Windows.h>

class Toolbar
{
public:
	Toolbar();
    HWND GetWindow() { return window; }
    void Configure();
private:
    HWND window;
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

