#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit1, hEdit2, hButton;
	static HFONT hFont1, hFont2;
	switch (msg)
	{
	case WM_CREATE:
		hFont1 = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("MS UI Gothic"));
		hFont2 = CreateFont(8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("ＭＳ ゴシック"));
		hEdit1 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), 0, WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, 10, 10, 128, 32, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hEdit2 = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), 0, WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_READONLY, 10, 50, 640, 128, hWnd, 0, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		hButton = CreateWindow(TEXT("BUTTON"), TEXT("取得"), WS_VISIBLE | WS_CHILD, 148, 10, 64, 32, hWnd, (HMENU)100, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		SendMessage(hEdit2, WM_SETFONT, (WPARAM)hFont2, 0);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 100)
		{
			TCHAR szText[1024];
			GetWindowText(hEdit1, szText, 1024);
			HDC hDC = GetDC(hWnd);
			HDC hMemDC = CreateCompatibleDC(hDC);
			SIZE size;
			SelectObject(hMemDC, hFont1);
			GetTextExtentPoint32(hMemDC, szText, lstrlen(szText), &size);
			HBITMAP hBitmap = CreateBitmap(size.cx, size.cy, 1, 1, 0);
			SelectObject(hMemDC, hBitmap);
			TextOut(hMemDC, 0, 0, szText, lstrlen(szText));
			SetWindowText(hEdit2, 0);
			LPTSTR p = (LPTSTR)GlobalAlloc(0, (size.cx + 3) * sizeof(TCHAR));
			for (int y = 0; y<size.cy; y++)
			{
				int x;
				for (x = 0; x<size.cx; x++)
				{
					if (GetPixel(hMemDC, x, y) == RGB(255, 255, 255))
					{
						p[x] = TEXT('　');
					}
					else
					{
						p[x] = TEXT('■');
					}
				}
				p[x++] = TEXT('\r');
				p[x++] = TEXT('\n');
				p[x++] = TEXT('\0');
				SendMessage(hEdit2, EM_REPLACESEL, 0, (LPARAM)p);
			}
			GlobalFree(p);
			DeleteDC(hMemDC);
			ReleaseDC(hWnd, hDC);
			DeleteObject(hBitmap);
		}
		break;
	case WM_DESTROY:
		DeleteObject(hFont1);
		DeleteObject(hFont2);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("AA"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
