// CourseProject-IV.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "main.h"
#include "Win32Application.h"
                        

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    return Win32Application::Instance()->Run(hInstance, nCmdShow);
}
