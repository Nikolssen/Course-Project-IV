#include "CalotteCanvas.h"

CalotteCanvas::CalotteCanvas() {
	this->window = nullptr;
	ResetRotation();
}

void CalotteCanvas::Configure(HWND parent, HINSTANCE hInst) {
	WNDCLASS wc{} ;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpfnWndProc = CanvasProc;
	wc.hInstance = hInst;
	wc.lpszClassName = szCanvasClass;
	RegisterClass(&wc);
	
	this->window = CreateWindowW(szCanvasClass, L"CalotteCanvas", WS_VISIBLE, 0, 0, 600, 600, nullptr, nullptr, hInst, nullptr);
}

void CalotteCanvas::ResetRotation() {
	xRot = yRot = zRot = 0.f;
}

void CalotteCanvas::Rotate(int x, int y, int z) {
	xRot += x;
	yRot += y;
	zRot += z;
}

LRESULT CALLBACK CalotteCanvas::CanvasProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	auto canvas = Win32Application::Instance()->GetCalotteCanvas();
	return DefWindowProc(hWnd, message, wParam, lParam);
}