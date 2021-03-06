#include "CalotteCanvas.h"

CalotteCanvas::CalotteCanvas() {
	this->window = nullptr;
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
	
	this->window = CreateWindowW(szCanvasClass, L"CalotteCanvas", WS_VISIBLE | WS_CHILD , 655, 0, 615, 615, parent, nullptr, hInst, nullptr);
	this->SetupContext();
	this->InitGL();
}

void CalotteCanvas::InitGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClearDepth(1.0f);	
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LEQUAL); 
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glViewport(0, 0, 615, 615);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio_w_h = 1;
	gluPerspective(45 , ratio_w_h, 0.1 , 200 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

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
void CalotteCanvas::Render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer
	glLoadIdentity();
	
	while (reusableQuadrics.size() < vertices3D.size())
	{
		GLUquadricObj* quad = gluNewQuadric();
		reusableQuadrics.push_back(quad);
	}

	for (unsigned int i = 0; i < vertices3D.size(); i++)
		Sphere(vertices3D.at(i), i);


	glFlush();
	SwapBuffers(hDC);
}

void CalotteCanvas::Sphere(Vertex3D& vertex, int pos)
{

	AtomColor color;
	float radius;

	switch (vertex.element)
	{
	case Element::Carbon:
		color = { 0.57, 0.57, 0.57 };
		radius = 1.12;
		break;
	case Element::Oxygen:
		color = { 0.96, 0.07, 0.01 };
		radius = 1;
		break;
	case Element::Nitrogen:
		color = { 0.2, 0.33, 0.9255 };
		radius = 1.02;
		break;
	case Element::Phosphorus:
		color = { 0.98, 0.5, 0.02 };
		radius = 1.18;
		break;
	case Element::Sulfur:
		color = { 1, 0.98, 0.267 };
		radius = 1.18;
		break;
	case Element::Chlorine:
		color = { 0.094, 0.964, 0.09 };
		radius = 1.15;
		break;
	case Element::Fluorine:
		color = { 0.54, 0.89, 0.325 };
		radius = 0.97;
		break;
	case Element::Bromine:
		color = { 0.65, 0.16, 0.15 };
		radius = 1.22;
		break;
	case Element::Iodine:
		color = { 0.58, 0, 0.58 };
		radius = 1.3;
		break;
	default:
		color = { 1,1,1 };
		radius = 0.79;
		break;
	}
	glPushMatrix();
	glColor3f(color.red, color.green, color.blue);
	glTranslatef(vertex.point.x, vertex.point.y, vertex.point.z - dist);
	gluSphere(reusableQuadrics[pos], radius, 30, 30);
	glPopMatrix();

}

void CalotteCanvas::Zoom(int delta) {

	dist += round(delta / 120.0);
	RedrawWindow(window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

LRESULT CALLBACK CalotteCanvas::CanvasProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	auto canvas = Win32Application::Instance()->GetCalotteCanvas();
	int zDelta;
	switch (message) {
	case WM_MOUSEWHEEL:
	{
		zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		canvas->Zoom(zDelta);
		break;
	}

	case WM_PAINT:
	{
		canvas->Render();
		break;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void CalotteCanvas::Save() {
	HDC hdc = GetWindowDC(window);
	auto memdc = CreateCompatibleDC(hdc);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, 600, 600);
	SelectObject(memdc, hBmp);
	BitBlt(memdc,0,0,600,600,hdc,0,	0,SRCCOPY);
	FileManager::Save(hBmp);
	DeleteObject(hBmp);
	DeleteDC(memdc);
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