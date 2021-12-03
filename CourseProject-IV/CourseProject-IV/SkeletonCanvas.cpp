#include "SkeletonCanvas.h"
SkeletonCanvas::SkeletonCanvas() {
    this->window = nullptr;
}

void SkeletonCanvas::Configure(HINSTANCE hInstance, WCHAR* windowClass, WCHAR* title) {

    WNDCLASSEXW wcex = { 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COURSEPROJECTIV));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(255, 248, 233));
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_COURSEPROJECTIV);
    wcex.lpszClassName = windowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    this->window = CreateWindowW(windowClass, title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
}

void SkeletonCanvas::MakeVisible(int nCmdShow) {
    ShowWindow(window, nCmdShow);
    UpdateWindow(window);
}



LRESULT CALLBACK SkeletonCanvas::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Win32Application* delegate = Win32Application::Instance();
    switch (message)
    {
    case WM_CREATE:
    {
        delegate->SetupToolBar(hWnd);
        break;
    }

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}