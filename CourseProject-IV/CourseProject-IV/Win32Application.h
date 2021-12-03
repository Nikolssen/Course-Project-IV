#pragma once
#include <Windows.h>
#include "SkeletonCanvas.h"
#include "Toolbar.h"
#include "CalotteCanvas.h"
#include "resource.h"

#define MAX_LOADSTRING 100
class SkeletonCanvas;
class Win32Application
{
public:
    static Win32Application* Instance();

    int Run(HINSTANCE hInstance, int nCmdShow);
    int SetupToolBar();

private:
    Win32Application();

    //Toolbar* toolbar;
    //CalotteCanvas* calotteCanvas;

    SkeletonCanvas* skeletonCanvas;

    WCHAR szTitle[MAX_LOADSTRING];
    WCHAR szWindowClass[MAX_LOADSTRING];
};

