#include <windows.h>

#include "HTMLParser.h"
#include "utils.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		case WM_PAINT: {
			
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd,&ps);
			
			if(page) {
				page->Draw(hdc);
			}
			
			EndPaint(hwnd,&ps);
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		case WM_SIZE: {
			InvalidateRgn(hwnd,NULL,true);
			break;
		}
		case WM_COMMAND: {
			switch(LOWORD(wParam)) {
				case ID_FILE_OPEN: {
					
					char file[MAX_PATH] = "";
					
					OPENFILENAMEA ofn;
					memset(&ofn,0,sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = hwnd;
					ofn.lpstrFilter = "HTML files\0*.html;*.htm\0All Files\0*.*\0";
					ofn.lpstrFile = file;
					ofn.nMaxFile = sizeof(file);
					
					if(GetOpenFileName(&ofn)) {
						page->Load(ofn.lpstrFile);
						InvalidateRgn(hwnd,NULL,true);
					}
					break;
				}
				case ID_FILE_EXIT: {
					SendMessage(hwnd,WM_CLOSE,0,0);
					break;
				}
				case ID_EDIT_REFRESH: {
					page->Refresh();
					InvalidateRgn(hwnd,NULL,true);
					break;
				}
			}
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	MSG Msg;

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MYMENU);
	wc.hIcon		 = LoadIcon(NULL, "A");
	wc.hIconSm		 = LoadIcon(NULL, "A");

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	RECT result = {0,0,800,600};
	AdjustWindowRect(&result,WS_VISIBLE|WS_OVERLAPPEDWINDOW,false);

	hwnd = CreateWindow("WindowClass","Browser",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, // x
		CW_USEDEFAULT, // y
		result.right - result.left, // width
		result.bottom - result.top, // height
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	// Load menu shortcuts
	HACCEL accelerator = LoadAccelerators(hInstance,"Accelerators");
	
	page = new Page("index.html");

	while(GetMessage(&Msg, NULL, 0, 0) > 0) {
		if(TranslateAccelerator(hwnd, accelerator, &Msg) == 0) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
	return Msg.wParam;
}
