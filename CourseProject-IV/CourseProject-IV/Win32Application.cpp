#include "Win32Application.h"

Win32Application* Win32Application::Instance() {
	static Win32Application instance;
	return &instance;
}

Win32Application::Win32Application() {
	this->toolbar = nullptr;
	this->skeletonCanvas = nullptr;
	this->hInst = nullptr;
	this->calotteCanvas = nullptr;
}

int Win32Application::Run(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance;
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_COURSEPROJECTIV, szWindowClass, MAX_LOADSTRING);

	

	this->skeletonCanvas = new SkeletonCanvas();
	skeletonCanvas->Configure(hInstance, szWindowClass, szTitle);
	skeletonCanvas->MakeVisible(nCmdShow);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COURSEPROJECTIV));
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAcceleratorW(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	this->calotteCanvas->ReleaseResources();
	delete calotteCanvas;
	delete toolbar;
	delete skeletonCanvas;
	return (int)msg.wParam;
}

int Win32Application::SetupChildWindows(HWND hwnd) {
	this->toolbar = new Toolbar();
	this->toolbar->Configure(hwnd, this->hInst);

	this->calotteCanvas = new CalotteCanvas();
	this->calotteCanvas->Configure(hwnd, this->hInst);

	return 0;
}