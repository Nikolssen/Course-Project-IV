#pragma once
#include <Windows.h>
#include "SkeletonCanvas.h"
#include "Toolbar.h"
#include "CalotteCanvas.h"

class Win32Application
{
public:
    static int Run(HINSTANCE hInstance, int nCmdShow);
private:
    Toolbar* toolbar;
    CalotteCanvas* calotteCanvas;
    SkeletonCanvas* skeletonCanvas;
};

