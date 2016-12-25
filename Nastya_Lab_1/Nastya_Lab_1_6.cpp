// Nastya_Lab_1.cpp: определяет точку входа для приложения.
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
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
char szMemory[1] = { 0 };
void Error_Report(HWND);


// Отправить объявления функций, включенных в этот модуль кода:
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

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NASTYA_LAB_1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);




    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NASTYA_LAB_1));

    MSG msg;

    // Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
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
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

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
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
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
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case ID_32771:

                fIn.lStructSize = sizeof(fIn);
                fIn.hwndOwner = hWnd;
                fIn.hInstance = hInst;
                fIn.lpstrFile = szFile;//файл
                fIn.lpstrFilter = _T("Text\0*.txt\0Word\0*.doc");//Фильтры файлов
                fIn.nMaxFile = 256;//максимальная длина имени файла
                fOut.lStructSize = sizeof(fOut);
                fOut.hwndOwner = hWnd;
                fOut.hInstance = hInst;
                fOut.lpstrFile = szFile;//файл
                fOut.lpstrFilter = _T("Text\0*.txt\0Word\0*.doc");//Фильтры файлов
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
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
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

// Обработчик сообщений для окна "О программе".
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