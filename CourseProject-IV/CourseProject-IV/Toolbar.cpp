#include "Toolbar.h"

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
	wct.hbrBackground = CreateSolidBrush(RGB(245, 245, 245));;
	wct.lpszMenuName = NULL;
	wct.lpszClassName = szToolsDialogClass;
	RegisterClassEx(&wct);
	this->window = CreateWindowExW(WS_EX_TOOLWINDOW, szToolsDialogClass, L"Tools", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 50, 615, parent, NULL, hInst, NULL);
}

LRESULT CALLBACK Toolbar::ToolsProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HWND hTools[12];
	static HBITMAP hBitmap[12];
	State* state = Win32Application::Instance()->GetSkeletonCanvas()->GetState();
	switch (message)
	{
	case WM_CREATE:
		for (int i = 0, y = 0, id = IDT_CARBON, idb = IDB_CARBON; i < 12; y += 48, i++, id++, idb++)
		{
			if (i == 9) { y += 10; }
			hTools[i] = CreateWindowW(L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_BITMAP, 1, y, 48, 48,
					hWnd, (HMENU)id, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
			hBitmap[i] = (HBITMAP)LoadImageW((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCEW(idb), IMAGE_BITMAP, 48, 48, NULL);
			SendDlgItemMessage(hWnd, id, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap[i]);
		}
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) 
		{
		case IDT_CARBON:
			state->SetElement(Element::Carbon);
			break;
		case IDT_NITROGEN:
			state->SetElement(Element::Nitrogen);
			break;
		case IDT_OXYGEN:
			state->SetElement(Element::Oxygen);
			break;
		case IDT_SULFUR:
			state->SetElement(Element::Sulfur);
			break;
		case IDT_PHOSPHORUS:
			state->SetElement(Element::Phosphorus);
			break;
		case IDT_FLUORINE:
			state->SetElement(Element::Fluorine);
			break;
		case IDT_CHLORINE:
			state->SetElement(Element::Chlorine);
			break;
		case IDT_BROMINE:
			state->SetElement(Element::Bromine);
			break;
		case IDT_IODINE:
			state->SetElement(Element::Iodine);
			break;
		case IDT_SINGLE:
			state->SetBond(1);
			break;
		case IDT_DOUBLE:
			state->SetBond(2);
			break;
		case IDT_TRIPLE:
			state->SetBond(3);
			break;
		}
		break;
	}
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