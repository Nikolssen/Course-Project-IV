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

void SkeletonCanvas::leftClick(int x, int y) {
    Vertex2D currentVertex;
    if (isWithinSafeZone(x, y))
    {
        int counter = vertices.size() - 1;
        switch (counter)
        {
        case -1:
            currentVertex.x = x;
            currentVertex.y = y;
            currentVertex.prevx = x + round(length * cos(pi / 6));
            currentVertex.prevy = y - round(length * sin(pi / 6));
            currentVertex.shortFront = (state.getElement() != Element::Carbon);
            currentVertex.shortRear = false;
            currentVertex.atom.element = state.getElement();
            currentVertex.atom.freeBonds = State::getMaxBonds(state.getElement());
            currentVertex.atom.bond = 0;
            vertices.push_back(currentVertex);
            break;
        case 0:
            if ((vertices[0].x - 10 < x) && (vertices[0].x + 10 > x) && (vertices[0].y - 10 < y) && (vertices[0].y + 10 > y) && (vertices[0].atom.freeBonds - state.getBond() >= 0))
            {
                currentVertex.x = vertices[0].prevx;
                currentVertex.y = vertices[0].prevy;
                currentVertex.prevx = vertices[0].x;
                currentVertex.prevy = vertices[0].y;
                currentVertex.atom.element = state.getElement();
                vertices[0].atom.freeBonds -= state.getBond();
                vertices[0].atom.bond = state.getBond();
                currentVertex.atom.freeBonds = State::getMaxBonds(state.getElement()) - state.getBond();
                currentVertex.atom.bond = state.getBond();
                currentVertex.shortFront = (state.getElement() != Element::Carbon);
                currentVertex.shortRear = vertices[0].shortFront;
                vertices.push_back(currentVertex);
            }
            break;
        default:
            bool flag = false;
            int i;
            for (i = 0; i <= counter; i++)
            {
                if ((vertices[i].x - 10 < x) && (vertices[i].x + 10 > x) && (vertices[i].y - 10 < y) && (vertices[i].y + 10 > y) && (vertices[i].atom.freeBonds - state.getBond() >= 0))
                {
                    flag = true;
                    break;
                }
            }
            if (flag)
            {
                int x1;
                int y1;
                int exodus;
                if (state.getBond() == 3 || vertices[i].atom.bond == 3 || (state.getBond() == 2 && vertices[i].atom.bond == 2))
                    exodus = 3;
                else
                {
                    if (vertices[i].atom.bond == 2)
                        exodus = 2 - vertices[i].atom.freeBonds;

                    else
                        exodus = (State::getMaxBonds(vertices[i].atom.element) - vertices[i].atom.freeBonds - 1);

                }
                getVertexPosition(vertices[i].prevx, vertices[i].prevy, vertices[i].x, vertices[i].y, exodus, x1, y1);
                currentVertex.x = x1;
                currentVertex.y = y1;
                currentVertex.prevx = vertices[i].x;
                currentVertex.prevy = vertices[i].y;
                currentVertex.atom.element = state.getElement();
                vertices[i].atom.freeBonds -= state.getBond();
                currentVertex.atom.bond = state.getBond();
                currentVertex.atom.freeBonds = State::getMaxBonds(state.getElement()) - state.getBond();
                currentVertex.shortFront = (state.getElement() != Element::Carbon);
                currentVertex.shortRear = vertices[i].shortFront;
                vertices.push_back(currentVertex);
            }
        }
        RedrawWindow(window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
    }
}

void SkeletonCanvas::getVertexPosition(int x0, int y0, int x, int y, int value, int &x1, int &y1)
{
    double alpha, c, s;
    int rx, ry;
    switch (value)
    {
    case 0:

        alpha = 2*pi/3;
        if (y0>=y)
            alpha = -alpha;
        break;
    case 1:

        alpha = 2*pi/3;
        if (y0<y)
            alpha = -alpha;
        break;
    case 2:
        alpha = pi/3;
        if (y0>=y)
            alpha = -alpha;
        break;
    case 3:
        alpha = pi;
        break;
    default:
        alpha = 2*pi/3;
    }

    rx = x0 - x;
    ry = y0 - y;
    c = cos(alpha);
    s = sin(alpha);
    x1 = round( x + rx * c - ry * s);
    y1 = round(y + rx * s + ry * c);
}

bool SkeletonCanvas::isWithinSafeZone(int x, int y) {
    RECT rect;
    GetClientRect(window, &rect);
    int x0 = rect.right;
    int y0 = rect.bottom;
    return ((x > 50) && (x < x0 - 50) && (y > 50) && (y < y0 - 50));
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

    case WM_LBUTTONUP:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
        delegate->getSkeletonCanvas()->leftClick(x, y);
        break;
    }

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
    return DefWindowProc(hWnd, message, wParam, lParam);
}