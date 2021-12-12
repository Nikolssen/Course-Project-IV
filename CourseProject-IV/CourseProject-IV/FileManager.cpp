#include "FileManager.h"
void FileManager::Save(HBITMAP bitmap) {
	OPENFILENAME ofn;
	TCHAR szFileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = Win32Application::Instance()->GetSkeletonCanvas()->GetWindow();
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = sizeof(szFileName);
	ofn.lpstrFilter = TEXT("Bitmap(*.bmp)\0*.bmp\0Все файлы(*.*)\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = TEXT("Select destination path");
	ofn.lpstrDefExt = TEXT("bmp");
	if (GetSaveFileName(&ofn) == TRUE) {
		CreateBMPFile(ofn.lpstrFile, bitmap);
	}
}
void FileManager::CreateBMPFile(LPTSTR pszFile, HBITMAP hBMP)
{
	HANDLE hf;                 // file handle  
	BITMAPFILEHEADER hdr;       // bitmap file-header  
	PBITMAPINFOHEADER pbih;     // bitmap info-header  
	LPBYTE lpBits;              // memory pointer  
	DWORD dwTotal;              // total count of bytes  
	DWORD cb;                   // incremental count of bytes  
	BYTE* hp;                   // byte pointer  
	DWORD dwTmp;
	PBITMAPINFO pbi;
	HDC hDC;

	hDC = CreateCompatibleDC(GetWindowDC(GetDesktopWindow()));
	SelectObject(hDC, hBMP);

	pbi = CreateBitmapInfoStruct(hBMP);

	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	assert(lpBits);

	assert(GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
		DIB_RGB_COLORS));

	hf = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
	assert(hf != INVALID_HANDLE_VALUE);

	hdr.bfType = 0x4d42;       
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	hdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD);
 
	assert(WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL));

	assert(WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+ pbih->biClrUsed * sizeof(RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)));

	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	assert(WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL));
 
	assert(CloseHandle(hf));

	// Free memory.  
	GlobalFree((HGLOBAL)lpBits);
}
PBITMAPINFO  FileManager::CreateBitmapInfoStruct(HBITMAP hBmp)
{
	BITMAP bmp{};
	PBITMAPINFO pbmi;
	WORD    cClrBits;
 
	assert(GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp));

	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;


	if (cClrBits < 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * (1 << cClrBits));

	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER));

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
 
	pbmi->bmiHeader.biCompression = BI_RGB;

	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
		* pbmi->bmiHeader.biHeight;
	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;
}