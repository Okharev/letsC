#pragma warning(push, 3)
#include <windows.h>
#pragma warning(pop)

#include "main.h"

HWND gGameWindow;

BOOL gGameIsRunning;


int WinMain(_In_ HINSTANCE Instance, _In_opt_ HINSTANCE PreviousInstance, _In_ PSTR CommandLine, _In_ INT CmdShow)
{
	UNREFERENCED_PARAMETER(Instance);
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
		while (PeekMessageW(&Message, gGameWindow, 0, 0, PM_REMOVE))
		{
			DispatchMessageW(&Message);
		}

		ProcessPlayerInput();

		Sleep(1);
	}


Exit:

	return(0);
}

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT Message, _In_ WPARAM WParam, _In_ LPARAM LParam)
{
	LRESULT Result = 0;

	switch (Message)
	{
		case WM_CLOSE:
		{
			gGameIsRunning = FALSE;

			PostQuitMessage(0);

			break;
		}
		default:
		{
			Result = DefWindowProcW(WindowHandle, Message, WParam, LParam);
		}
	}

	return(Result);
}

DWORD CreateMainGameWindow(void)
{
	DWORD Result = ERROR_SUCCESS;
	WNDCLASSEXW WindowClass = { 0 };
	WindowClass.cbSize = sizeof(WNDCLASSEXW);
	WindowClass.style = 0;
	WindowClass.lpfnWndProc = MainWindowProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandleW(NULL);
	WindowClass.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	WindowClass.hIconSm = LoadIconW(NULL, IDI_APPLICATION);
	WindowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	WindowClass.hbrBackground = CreateSolidBrush(RGB(255, 0, 255));
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = GAME_NAME L"_WINDOWCLASS";


	if (RegisterClassExW(&WindowClass) == 0)
	{
		Result = GetLastError();

		MessageBoxW(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);

		goto Exit;
	}

	gGameWindow = CreateWindowExW(WS_EX_CLIENTEDGE, WindowClass.lpszClassName, L"Window Title", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, NULL, NULL, GetModuleHandleW(NULL), NULL);

	if (gGameWindow == NULL)
	{
		Result = GetLastError();

		MessageBoxW(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);

		goto Exit;
	}

Exit:

	return(Result);
}

BOOL GameIsAlreadyRunning(void)
{
	HANDLE Mutex = NULL;

	Mutex = CreateMutexW(NULL, FALSE, GAME_NAME L"_GameMutex");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

void ProcessPlayerInput(void)
{
	short EscapeKeyIsDown = GetAsyncKeyState(VK_ESCAPE);

	if (EscapeKeyIsDown)
	{
		SendMessageW(gGameWindow, WM_CLOSE, 0, 0);
	}
}