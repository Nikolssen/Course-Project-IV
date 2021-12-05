#pragma once
#include <Windows.h>
#include "resource.h"
#include "Win32Application.h"
#include "State.h"
#include <vector>
#include <string>
#include "Vertex2D.h"
#include <windowsx.h>
class SkeletonCanvas
{
public:
    SkeletonCanvas();
    void MakeVisible(int nCmdShow);
    void LeftClick(int x, int y);
    void Paint(HDC dc, PAINTSTRUCT ps);
    void Configure(HINSTANCE hInstance, WCHAR* windowClass, WCHAR* title);
    HWND GetWindow() { return window; }
    State* GetState() { return &state; }
private:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    bool IsWithinSafeZone(int x, int y);
    void CalculateVertexPosition(int x0, int y0, int x, int y, int value, int& x1, int& y1);
    HWND window;
    State state;
    std::vector<Vertex2D> vertices;
    const int length = 50;
    const double pi = 3.14159265358979323846;
};

