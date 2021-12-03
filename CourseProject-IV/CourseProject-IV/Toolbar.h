#pragma once
#include <Windows.h>
#include "resource.h"

class Toolbar
{
public:
	Toolbar();
    HWND GetWindow() { return window; }
    void Configure(HWND parent, HINSTANCE hInst);
private:
    HWND window;
    LPCWSTR szToolsDialogClass = L"Tools Dialog";
    static LRESULT CALLBACK ToolsProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

