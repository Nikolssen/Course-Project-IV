#include "Toolbar.h"
#include "math.h"

#define IDT_CARBON                      30017
#define IDT_NITROGEN                    30018
#define IDT_OXYGEN                      30019

#define IDT_SULFUR                      30020
#define IDT_PHOSPHORUS                  30021

#define IDT_FLUORINE                    30022
#define IDT_CHLORINE                    30023
#define IDT_BROMINE                     30024
#define IDT_IODINE                      30025

#define IDT_SINGLE                      30026
#define IDT_DOUBLE                      30027
#define IDT_TRIPLE                      30028

Toolbar::Toolbar() {
	this->window = nullptr;
}

void Toolbar::Configure(HWND parent, HINSTANCE hInst) {
	WNDCLASSEX  wct;
	wct.cbSize = sizeof(WNDCLASSEX);
	wct.style = 0;
	wct.lpfnWndProc = (WNDPROC)ToolsProc;
	wct.cbClsExtra = 0;
	wct.cbWndExtra = 0;
	wct.hInstance = (HINSTANCE)GetWindowLong(parent, GWL_HINSTANCE);
	wct.hIcon = NULL;
	wct.hIconSm = NULL;
	wct.hCursor = LoadCursor(NULL, IDC_ARROW);
	wct.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wct.lpszMenuName = NULL;
	wct.lpszClassName = szToolsDialogClass;
	RegisterClassEx(&wct);
	this->window = CreateWindowExW(WS_EX_TOOLWINDOW, szToolsDialogClass, L"Tools", WS_VISIBLE, 0, 300, 90, 270, parent, NULL, hInst, NULL);
}

LRESULT CALLBACK Toolbar::ToolsProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HWND hTools[12];
	static HBITMAP hBitmap[12];
	switch (message)
	{
	case WM_CREATE:
		for (int i = 0, y = 5, id = IDT_CARBON, idb = IDB_CARBON; y < 185; y += 35)
		{
			for (int x = 5; x < 70; x += 35, i++, id++, idb++)
			{
				
				hTools[i] = CreateWindow(L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | BS_BITMAP, x, y, 30, 30,
					hWnd, (HMENU)id, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
				hBitmap[i] = (HBITMAP)LoadImageW((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCEW(idb), IMAGE_BITMAP, 25, 25, NULL);
				SendDlgItemMessage(hWnd, id, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[i]);
			}
		}
		break;
	case WM_DESTROY:
		for (int i = 0; i < 12; i++)
		{
			DestroyWindow(hTools[i]);
			DeleteObject(hBitmap[i]);
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}