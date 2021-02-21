#pragma once

#define GAME_NAME L"LetsC"

LRESULT CALLBACK MainWindowProc(_In_ HWND WindowHandle, _In_ UINT MessageId, _In_ WPARAM WParam, _In_ LPARAM LParam);

DWORD CreateMainGameWindow(void);

BOOL GameIsAlreadyRunning(void);