// Nastya_Lab_1.cpp: ���������� ����� ����� ��� ����������.
//

#include  <ctype.h>
#include "stdafx.h"
#include "Nastya_Lab_1.h"
#include "CommDlg.h" 
#include "winbase.h"
#include <iostream>
#include <locale.h>

#define MAX_LOADSTRING 100
void REPORT_ERROR(HWND hWnd);
// ���������� ����������:
HINSTANCE hInst;                                // ������� ���������
WCHAR szTitle[MAX_LOADSTRING];                  // ����� ������ ���������
WCHAR szWindowClass[MAX_LOADSTRING];            // ��� ������ �������� ����
char szMemory[1] = { 0 };
void Error_Report(HWND);


// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    setlocale(LC_CTYPE, "Russian");

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���������� ��� �����.

    // ������������� ���������� �����
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NASTYA_LAB_1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);




    // ��������� ������������� ����������:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NASTYA_LAB_1));

    MSG msg;

    // ���� ��������� ���������:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NASTYA_LAB_1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_NASTYA_LAB_1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ������������ ��������� � ������� ����.
//
//  WM_COMMAND � ���������� ���� ����������
//  WM_PAINT � ���������� ������� ����
//  WM_DESTROY � ��������� ��������� � ������ � ���������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int i = 1;
    OPENFILENAME fIn = { 0 }, fOut = { 0 };
    WCHAR szFile[MAX_LOADSTRING] = { 0 };
    static DWORD nIn, nOut;
    DWORD step = 0;
    static HANDLE hIn, hOut, hIn_;
    char read[1];
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // ��������� ����� � ����:
            switch (wmId)
            {
            case ID_32771:

                fIn.lStructSize = sizeof(fIn);
                fIn.hwndOwner = hWnd;
                fIn.hInstance = hInst;
                fIn.lpstrFile = szFile;//����
                fIn.lpstrFilter = _T("Text\0*.txt\0Word\0*.doc");//������� ������
                fIn.nMaxFile = 256;//������������ ����� ����� �����
                fOut.lStructSize = sizeof(fOut);
                fOut.hwndOwner = hWnd;
                fOut.hInstance = hInst;
                fOut.lpstrFile = szFile;//����
                fOut.lpstrFilter = _T("Text\0*.txt\0Word\0*.doc");//������� ������
                fOut.nMaxFile = 256;
                if (GetOpenFileName(&fIn))
                {
                    hIn = CreateFile(fIn.lpstrFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
                    if (hIn == INVALID_HANDLE_VALUE) REPORT_ERROR(hWnd);
                    if (GetSaveFileName(&fOut))
                    {
                        hOut = CreateFile(fOut.lpstrFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
                        if (hOut == INVALID_HANDLE_VALUE) REPORT_ERROR(hWnd);
						SetFilePointer( hIn, 0L, NULL, FILE_BEGIN );
                        DWORD fileSize = SetFilePointer( hIn, 0L, NULL, FILE_END );
						SetFilePointer( hIn, 0L, NULL, FILE_BEGIN );
						
                        DWORD dread;
                        HLOCAL local = LocalAlloc( LPTR, fileSize + 1u );
                        char *buf = (char*) LocalLock( local );
                        ReadFile( hIn, (LPVOID) buf, fileSize, &dread, NULL );
						CloseHandle( hIn );
                        buf[dread] = '\0';
					
						char *outstr = CharLowerA( buf );

                        WriteFile( hOut, outstr, fileSize, NULL, NULL );
                    }
                }
;
                break;
            
            
            

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �������� ���� ����� ��� ����������, ������������ HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���������� ��������� ��� ���� "� ���������".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
void REPORT_ERROR(HWND hWnd) {
    LPWSTR mass = nullptr;
    int error = GetLastError();
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
        , NULL
        , error
        , MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
        , (LPWSTR)&mass
        , 0
        , NULL);
    MessageBox(hWnd, (LPWSTR)mass, _T("ErrorMessage" + error), MB_OK | MB_ICONERROR);
    exit(1);
};