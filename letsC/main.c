#include <windows.h>

#include "main.h"

BOOL gGameIsRunning;

int wWinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE PreviousInstance, _In_ PWSTR CommandLine, _In_ INT CmdShow)
{
	UNREFERENCED_PARAMETER(PreviousInstance);
	UNREFERENCED_PARAMETER(CommandLine);
	UNREFERENCED_PARAMETER(CmdShow);

	if (GameIsAlreadyRunning() == TRUE)
	{
		MessageBoxW(NULL, L"Another instance of this program is already running!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	if (CreateMainGameWindow() != ERROR_SUCCESS)
	{
		goto Exit;
	}

	MSG Message = { 0 };

	gGameIsRunning = TRUE;

	while (gGameIsRunning == TRUE)
	{
		while (PeekMessageW(&Message, NULL, 0, 0) > 0)
		{
			TranslateMessage(&Message);
			DispatchMessageW(&Message);
		}
	}

Exit:
	return (0);
}


LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT MessageId, _In_ WPARAM WParam, _In_ LPARAM LParam)
{

	LRESULT Result = 0;

	wchar_t buf[12] = { 0 };

	_itow_s(MessageId, buf, _countof(buf), 10);

	OutputDebugStringW(buf);
	OutputDebugStringW(L"\n");

	switch (MessageId)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			gGameIsRunning = FALSE;
			break;
		}
		default:
		{
			Result = DefWindowProcW(WindowHandle, MessageId, WParam, LParam);
		}
	}
	return Result;
}

DWORD CreateMainGameWindow(void)
{
	DWORD Result = ERROR_SUCCESS;

	WNDCLASSEXW WindowClass = { 0 };
	HWND WindowHandle = 0;
	WindowClass.cbSize = sizeof(WNDCLASSEXW);
	WindowClass.style = 0;
	WindowClass.lpfnWndProc = MainWindowProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandleW(NULL);
	WindowClass.hIcon = LoadIconW(NULL, (LPCWSTR)IDI_APPLICATION);
	WindowClass.hIconSm = LoadIconW(NULL, (LPCWSTR)IDI_APPLICATION);
	WindowClass.hCursor = LoadCursorW(NULL, (LPCWSTR)IDC_ARROW);
	WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = GAME_NAME L"_WINDOWCLASS";

	if (RegisterClassExW(&WindowClass) == 0)
	{
		Result = GetLastError();

		MessageBoxW(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	WindowHandle = CreateWindowExW(
		WS_EX_CLIENTEDGE,
		WindowClass.lpszClassName,
		L"Window title",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1000,
		1000,
		NULL,
		NULL,
		GetModuleHandleW(NULL),
		NULL
	);


	if (WindowHandle == 0)
	{
		Result = GetLastError();

		MessageBoxW(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

Exit:

	return Result;
}

BOOL GameIsAlreadyRunning(void)
{
	HANDLE Mutex = NULL;

	Mutex = CreateMutexW(NULL, FALSE, GAME_NAME"_GameMutex");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}