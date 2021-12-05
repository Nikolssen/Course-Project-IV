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
        CW_USEDEFAULT, 0, 1280, 720, nullptr, nullptr, hInstance, nullptr);
}

void SkeletonCanvas::MakeVisible(int nCmdShow) {
    ShowWindow(window, nCmdShow);
    UpdateWindow(window);
}

void SkeletonCanvas::LeftClick(int x, int y) {
    Vertex2D currentVertex{};
    if (IsWithinSafeZone(x, y))
    {
        int counter = vertices.size() - 1;
        switch (counter)
        {
        case -1:
            currentVertex.x = x;
            currentVertex.y = y;
            currentVertex.prevx = x + round(length * cos(pi / 6));
            currentVertex.prevy = y - round(length * sin(pi / 6));
            currentVertex.shortFront = (state.GetElement() != Element::Carbon);
            currentVertex.shortRear = false;
            currentVertex.atom.element = state.GetElement();
            currentVertex.atom.freeBonds = State::GetMaxBonds(state.GetElement());
            currentVertex.atom.bond = 0;
            vertices.push_back(currentVertex);
            break;
        case 0:
            if ((vertices[0].x - 10 < x) && (vertices[0].x + 10 > x) && (vertices[0].y - 10 < y) && (vertices[0].y + 10 > y) && (vertices[0].atom.freeBonds - state.GetBond() >= 0))
            {
                currentVertex.x = vertices[0].prevx;
                currentVertex.y = vertices[0].prevy;
                currentVertex.prevx = vertices[0].x;
                currentVertex.prevy = vertices[0].y;
                currentVertex.atom.element = state.GetElement();
                vertices[0].atom.freeBonds -= state.GetBond();
                vertices[0].atom.bond = state.GetBond();
                currentVertex.atom.freeBonds = State::GetMaxBonds(state.GetElement()) - state.GetBond();
                currentVertex.atom.bond = state.GetBond();
                currentVertex.shortFront = (state.GetElement() != Element::Carbon);
                currentVertex.shortRear = vertices[0].shortFront;
                vertices.push_back(currentVertex);
            }
            break;
        default:
            bool flag = false;
            int i;
            for (i = 0; i <= counter; i++)
            {
                if ((vertices[i].x - 10 < x) && (vertices[i].x + 10 > x) && (vertices[i].y - 10 < y) && (vertices[i].y + 10 > y) && (vertices[i].atom.freeBonds - state.GetBond() >= 0))
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
                if (state.GetBond() == 3 || vertices[i].atom.bond == 3 || (state.GetBond() == 2 && vertices[i].atom.bond == 2))
                    exodus = 3;
                else
                {
                    if (vertices[i].atom.bond == 2)
                        exodus = 2 - vertices[i].atom.freeBonds;

                    else
                        exodus = (State::GetMaxBonds(vertices[i].atom.element) - vertices[i].atom.freeBonds - 1);

                }
                CalculateVertexPosition(vertices[i].prevx, vertices[i].prevy, vertices[i].x, vertices[i].y, exodus, x1, y1);
                currentVertex.x = x1;
                currentVertex.y = y1;
                currentVertex.prevx = vertices[i].x;
                currentVertex.prevy = vertices[i].y;
                currentVertex.atom.element = state.GetElement();
                vertices[i].atom.freeBonds -= state.GetBond();
                currentVertex.atom.bond = state.GetBond();
                currentVertex.atom.freeBonds = State::GetMaxBonds(state.GetElement()) - state.GetBond();
                currentVertex.shortFront = (state.GetElement() != Element::Carbon);
                currentVertex.shortRear = vertices[i].shortFront;
                vertices.push_back(currentVertex);
            }
        }
        RedrawWindow(window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
    }
}

void SkeletonCanvas::CalculateVertexPosition(int x0, int y0, int x, int y, int value, int &x1, int &y1)
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

bool SkeletonCanvas::IsWithinSafeZone(int x, int y) {
    int x0 = 640;
    int y0 = 650;
    return ((x > 100) && (x < x0 - 50) && (y > 50) && (y < y0 - 50));
}

void SkeletonCanvas::Paint(HDC dc, PAINTSTRUCT ps) {
    if (vertices.size() == 0) {
        return;
    }
    HGDIOBJ original = NULL;

    original = SelectObject(dc, GetStockObject(DC_PEN));
    DeletePen(original);

    HPEN pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    SelectObject(dc, pen);
    
    HFONT hFont = CreateFontW(30, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
    SelectObject(dc, hFont);
    SetBkMode(dc, TRANSPARENT);
    std::wstring str;
    int counter = vertices.size() - 1;
    if (counter == 0)
    {
        str = vertices[0].Description();
        LPCWSTR result =str.c_str();
        RECT rect{};
        rect.left = vertices[0].x;
        rect.top = vertices[0].y - 10;
        rect.bottom = rect.top + 40;
        rect.right = rect.left + 80;
        DrawText(dc, result, -1, &rect, DT_LEFT | DT_TOP);
    }
    else
    {
        if (vertices[0].atom.element != Element::Carbon)
        {
            str = vertices[0].Description();
            LPCWSTR result = str.c_str();
            RECT rect{};
            rect.left = vertices[0].x - 10;
            rect.top = vertices[0].y - 10;
            rect.bottom = rect.top + 40;
            rect.right = rect.left + 80;
            DrawText(dc, result, -1, &rect, DT_LEFT | DT_TOP);
        }
        int x0, x1, y0, y1;
        for (int i = 1; i <= counter; i++)
        {
            if (vertices[i].atom.element != Element::Carbon)
            {
                str = vertices[i].Description();
                LPCWSTR result = str.c_str();
                RECT rect{};
                rect.left = vertices[i].x - 5;
                rect.top = vertices[i].y - 10;
                rect.bottom = rect.top + 40;
                rect.right = rect.left + 80;
                DrawText(dc, result, -1, &rect, DT_LEFT | DT_TOP);
            }
            if (vertices[i].shortRear)
            {
                x0 = round(vertices[i].x / 4.0 + vertices[i].prevx / 4.0 * 3);
                y0 = round(vertices[i].y / 4.0 + vertices[i].prevy / 4.0 * 3);
            }
            else
            {
                x0 = vertices[i].prevx;
                y0 = vertices[i].prevy;
            }

            if (vertices[i].shortFront)
            {

                x1 = round(vertices[i].x / 4.0 * 3 + vertices[i].prevx / 4.0);
                y1 = round(vertices[i].y / 4.0 * 3 + vertices[i].prevy / 4.0);
            }
            else
            {
                x1 = vertices[i].x;
                y1 = vertices[i].y;
            }
            switch (vertices[i].atom.bond)
            {


            case 2:
                if (vertices[i].prevx == vertices[i].x)
                {
                    
                    MoveToEx(dc, x0 - 2, y0, NULL);
                    LineTo(dc, x1 - 2, y1);

                    MoveToEx(dc, x0 + 2, y0, NULL);
                    LineTo(dc, x1 + 2, y1);
                }
                else
                {
                    MoveToEx(dc, x0, y0 - 3, NULL);
                    LineTo(dc, x1, y1 - 3);

                    MoveToEx(dc, x0, y0 + 3, NULL);
                    LineTo(dc, x1, y1 + 3);
                }
                break;


            case 3:

                if (vertices[i].prevx == vertices[i].x)
                {
                    MoveToEx(dc, x0 - 5, y0, NULL);
                    LineTo(dc, x1 - 5, y1);

                    MoveToEx(dc, x0 + 5, y0, NULL);
                    LineTo(dc, x1 + 5, y1);
                }
                else
                {
                    MoveToEx(dc, x0, y0 - 5, NULL);
                    LineTo(dc, x1, y1 - 5);

                    MoveToEx(dc, x0, y0 + 5, NULL);
                    LineTo(dc, x1, y1 + 5);
                }
            case 1:
                MoveToEx(dc, x0, y0, NULL);
                LineTo(dc, x1, y1);
                break;
            }
        }
    }
    DeleteObject(hFont);
}

LRESULT CALLBACK SkeletonCanvas::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Win32Application* delegate = Win32Application::Instance();
    switch (message)
    {
    case WM_CREATE:
    {
        delegate->SetupChildWindows(hWnd);
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
        delegate->GetSkeletonCanvas()->LeftClick(x, y);
        break;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        delegate->GetSkeletonCanvas()->Paint(hdc, ps);
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