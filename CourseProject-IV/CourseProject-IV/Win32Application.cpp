#include "Win32Application.h"

Win32Application* Win32Application::Instance() {
	static Win32Application instance;
	return &instance;
}

Win32Application::Win32Application() {
	//this->toolbar = nullptr;
	this->skeletonCanvas = nullptr;
	//this->calotteCanvas = nullptr;
}

int Win32Application::Run(HINSTANCE hInstance, int nCmdShow) {
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_COURSEPROJECTIV, szWindowClass, MAX_LOADSTRING);

	//this->toolbar = new Toolbar();

	this->skeletonCanvas = new SkeletonCanvas();
	skeletonCanvas->Configure(hInstance, szWindowClass, szTitle);
	skeletonCanvas->MakeVisible(nCmdShow);

	return skeletonCanvas->RunLoop(hInstance);
}