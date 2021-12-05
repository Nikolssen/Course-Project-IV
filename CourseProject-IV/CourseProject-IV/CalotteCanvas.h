#pragma once
#include <Windows.h>
#include <vector>
#include <gl/gl.h>
#include <gl/glu.h>
#pragma comment( lib, "./OpenGL32.Lib" )
#pragma comment( lib, "./GLU32.Lib" )
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

    void ReleaseResources();
    void Render();
    void setVertices(std::vector<Vertex3D> vertices) { vertices3D.clear(); vertices3D = vertices; }
private:
    void Sphere(Vertex3D& vertex, int pos);
    void ResetRotation();
    void InitGL();
    void SetupContext();

    HWND window;
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
    std::vector<Vertex3D> vertices3D;
    GLfloat xRot, yRot, zRot;
    HGLRC      hRC;
    HDC        hDC;
    static LRESULT CALLBACK CanvasProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    float dist = 7;
};

