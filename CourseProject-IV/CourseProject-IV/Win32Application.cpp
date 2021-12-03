#include "Win32Application.h"

Win32Application* Win32Application::Instance() {
	static Win32Application instance;
	return &instance;
}

Win32Application::Win32Application() {
	this->toolbar = nullptr;
	this->skeletonCanvas = nullptr;
	//this->calotteCanvas = nullptr;
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

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

int Win32Application::SetupToolBar(HWND hwnd) {
	this->toolbar = new Toolbar();
	this->toolbar->Configure(hwnd, this->hInst);

	return 0;
}