#pragma once
#include <Windows.h>
#include "resource.h"
#include "Win32Application.h"
#include "State.h"

class SkeletonCanvas
{
public:
    SkeletonCanvas();
    HWND GetWindow() { return window; }
    void Configure(HINSTANCE hInstance, WCHAR* windowClass, WCHAR* title);
    void MakeVisible(int nCmdShow);
    State* getState() { return &state; }
private:
    HWND window;
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    State state;
};

