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
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = NULL;         
	wc.lpszMenuName = NULL;
	wc.hInstance = hInst;
	wc.lpszClassName = szCanvasClass;
	RegisterClass(&wc);
	
	this->window = CreateWindowW(szCanvasClass, L"CalotteCanvas", WS_VISIBLE , 665, 0, 615, 720, parent, nullptr, hInst, nullptr);
	this->SetupContext();
	this->InitGL();
}

void CalotteCanvas::InitGL() {

	glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);               // Black Background
	glClearDepth(1.0f);                                 // Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, 615, 720);                       // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);                         // Select The Modelview Matrix

	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CalotteCanvas::SetupContext() {
	GLuint   PixelFormat;
	static  PIXELFORMATDESCRIPTOR pfd =              // pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor
		1,                                          // Version Number
		PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
		PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,                           // Must Support Double Buffering
		PFD_TYPE_RGBA,                              // Request An RGBA Format
		16,                                       // Select Our Color Depth
		0, 0, 0, 0, 0, 0,                           // Color Bits Ignored
		0,                                          // No Alpha Buffer
		0,                                          // Shift Bit Ignored
		0,                                          // No Accumulation Buffer
		0, 0, 0, 0,                                 // Accumulation Bits Ignored
		16,                                         // 16Bit Z-Buffer (Depth Buffer)  
		0,                                          // No Stencil Buffer
		0,                                          // No Auxiliary Buffer
		PFD_MAIN_PLANE,                             // Main Drawing Layer
		0,                                          // Reserved
		0, 0, 0                                     // Layer Masks Ignored
	};

	if (!(hDC = GetDC(window)))
	{
		ReleaseResources();
		return;
	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		ReleaseResources();
		return;
	}
	if (!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		ReleaseResources();
		return;
	}
	if (!(hRC = wglCreateContext(hDC)))
	{
		ReleaseResources();
		return;
	}
	if (!wglMakeCurrent(hDC, hRC))
	{
		ReleaseResources();
		return;
	}
	return;
}

void CalotteCanvas::ResetRotation() {
	xRot = yRot = zRot = 0.f;
}

void CalotteCanvas::Rotate(int x, int y, int z) {
	xRot += x;
	yRot += y;
	zRot += z;
}
void CalotteCanvas::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer
	glLoadIdentity();
}
LRESULT CALLBACK CalotteCanvas::CanvasProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	auto canvas = Win32Application::Instance()->GetCalotteCanvas();
	canvas->Render();
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void CalotteCanvas::ReleaseResources() {

	if (hRC)
	{
		wglMakeCurrent(nullptr, nullptr);

		wglDeleteContext(hRC);
		hRC = nullptr;
	}
	if (hDC && !ReleaseDC(window, hDC))
	{
		hDC = nullptr;
	}
}