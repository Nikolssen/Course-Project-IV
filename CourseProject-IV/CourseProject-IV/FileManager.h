#pragma once
#include <Windows.h>
#include "Win32Application.h"
#include <assert.h>
class FileManager
{
public:
	static void Save(HBITMAP bitmap);
private:
	static void CreateBMPFile(LPTSTR pszFile, HBITMAP hBMP);
	static PBITMAPINFO  CreateBitmapInfoStruct(HBITMAP hBmp);
};

