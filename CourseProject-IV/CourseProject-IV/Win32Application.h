#pragma once
#include <Windows.h>
#include "SkeletonCanvas.h"
#include "Toolbar.h"
#include "CalotteCanvas.h"
#include "resource.h"

class SkeletonCanvas;
class Win32Application
{
public:
    static Win32Application* Instance();

    int Run(HINSTANCE hInstance, int nCmdShow);
    int SetupToolBar(HWND hwnd);

private:
    Win32Application();

    Toolbar* toolbar;
    //CalotteCanvas* calotteCanvas;

    SkeletonCanvas* skeletonCanvas;

    HINSTANCE hInst;
    WCHAR szTitle[MAX_LOADSTRING];
    WCHAR szWindowClass[MAX_LOADSTRING];
};

