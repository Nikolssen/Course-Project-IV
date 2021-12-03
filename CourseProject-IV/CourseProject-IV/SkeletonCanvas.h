#pragma once
#include <Windows.h>
#include "resource.h"
#include "Win32Application.h"
#include "State.h"
#include <vector>
#include "Vertex2D.h"
#include <windowsx.h>
class SkeletonCanvas
{
public:
    SkeletonCanvas();
    void MakeVisible(int nCmdShow);
    void leftClick(int x, int y);
    void Configure(HINSTANCE hInstance, WCHAR* windowClass, WCHAR* title);

    HWND GetWindow() { return window; }
    State* getState() { return &state; }
private:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    bool isWithinSafeZone(int x, int y);
    void getVertexPosition(int x0, int y0, int x, int y, int value, int& x1, int& y1);

    HWND window;
    State state;
    std::vector<Vertex2D> vertices;
    const int length = 30;
    const double pi = 3.14159265358979323846;
};

