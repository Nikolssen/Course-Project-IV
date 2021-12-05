#pragma once
#include <Windows.h>
#include <vector>
#include <gl/GL.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#pragma comment( lib, "./OpenGL32.Lib" )
#include "Vertex3D.h"
#include "Win32Application.h"

class CalotteCanvas
{
public:
    CalotteCanvas();
    HWND GetWindow() { return window; }
    void Configure(HWND parent, HINSTANCE hInst);
    //void Zoom();
    void Rotate(int x, int y, int z);
    void ResetRotation();
    void InitGL();
    void SetupContext();
    void ReleaseResources();
    void Render();
private:
    HWND window;
    std::vector<Vertex3D> vertices3D;
    std::vector<GLUquadricObj*> reusableQuadrics;
    LPCWSTR szCanvasClass = L"Calotte Canvas";
    const GLfloat light_ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    const GLfloat light_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[4] = { 2.0f, 5.0f, 5.0f, 0.0f };

    const GLfloat mat_ambient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[1] = { 100.0f };

    GLfloat xRot, yRot, zRot;
    HGLRC      hRC;
    HDC        hDC;
    static LRESULT CALLBACK CanvasProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

