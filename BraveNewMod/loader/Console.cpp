#include "context.h"
#include <iostream>
#include <fcntl.h>
#include <io.h>

struct console {
	HANDLE err, out, in;
} old, now;

void Utils::EnableConsole()
{
	//Save Old Handles
	old.err = GetStdHandle(STD_ERROR_HANDLE);
	old.out = GetStdHandle(STD_OUTPUT_HANDLE);
	old.in = GetStdHandle(STD_INPUT_HANDLE);
	//Enable Console
	::AllocConsole() && ::AttachConsole(GetCurrentProcessId());
	BindCrtHandlesToStdHandles(true, true, true);
	//New Handles
	now.err = GetStdHandle(STD_ERROR_HANDLE);
	now.out = GetStdHandle(STD_OUTPUT_HANDLE);
	now.in = GetStdHandle(STD_INPUT_HANDLE);
	// Output Settings
	SetConsoleMode(now.out,
		ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);
	// Input Settings
	SetConsoleMode(now.in,
		ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
		ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);
	// Finally Set Title
	SetConsoleTitle(L"[FFXIV] Console");
}

void Utils::DisableConsole()
{
	// Free Console
	if (now.err && now.out && now.in)
		FreeConsole();
	// Free Handles
	if (old.err)
		SetStdHandle(STD_ERROR_HANDLE, old.err);
	if (old.out)
		SetStdHandle(STD_OUTPUT_HANDLE, old.out);
	if (old.in)
		SetStdHandle(STD_INPUT_HANDLE, old.in);
}

void Utils::BindCrtHandlesToStdHandles(bool bindStdIn, bool bindStdOut, bool bindStdErr)
{
	if (bindStdIn)
	{
		FILE* dummyFile;
		freopen_s(&dummyFile, "nul", "r", stdin);
	}
	if (bindStdOut)
	{
		FILE* dummyFile;
		freopen_s(&dummyFile, "nul", "w", stdout);
	}
	if (bindStdErr)
	{
		FILE* dummyFile;
		freopen_s(&dummyFile, "nul", "w", stderr);
	}
	// Redirect unbuffered stdin from the current standard input handle
	if (bindStdIn)
	{
		HANDLE stdHandle = GetStdHandle(STD_INPUT_HANDLE);
		if (stdHandle != INVALID_HANDLE_VALUE)
		{
			int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
			if (fileDescriptor != -1)
			{
				FILE* file = _fdopen(fileDescriptor, "r");
				if (file != NULL)
				{
					int dup2Result = _dup2(_fileno(file), _fileno(stdin));
					if (dup2Result == 0)
					{
						setvbuf(stdin, NULL, _IONBF, 0);
					}
				}
			}
		}
	}
	// Redirect unbuffered stdout to the current standard output handle
	if (bindStdOut)
	{
		HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (stdHandle != INVALID_HANDLE_VALUE)
		{
			int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
			if (fileDescriptor != -1)
			{
				FILE* file = _fdopen(fileDescriptor, "w");
				if (file != NULL)
				{
					int dup2Result = _dup2(_fileno(file), _fileno(stdout));
					if (dup2Result == 0)
					{
						setvbuf(stdout, NULL, _IONBF, 0);
					}
				}
			}
		}
	}
	// Redirect unbuffered stderr to the current standard error handle
	if (bindStdErr)
	{
		HANDLE stdHandle = GetStdHandle(STD_ERROR_HANDLE);
		if (stdHandle != INVALID_HANDLE_VALUE)
		{
			int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
			if (fileDescriptor != -1)
			{
				FILE* file = _fdopen(fileDescriptor, "w");
				if (file != NULL)
				{
					int dup2Result = _dup2(_fileno(file), _fileno(stderr));
					if (dup2Result == 0)
					{
						setvbuf(stderr, NULL, _IONBF, 0);
					}
				}
			}
		}
	}
	if (bindStdIn)
	{
		std::wcin.clear();
		std::cin.clear();
	}
	if (bindStdOut)
	{
		std::wcout.clear();
		std::cout.clear();
	}
	if (bindStdErr)
	{
		std::wcerr.clear();
		std::cerr.clear();
	}
}

void ClearConsole(char fill = ' ')
{
	COORD tl = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO s;
	GetConsoleScreenBufferInfo(now.out, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	FillConsoleOutputCharacter(now.out, fill, cells, tl, &written);
	FillConsoleOutputAttribute(now.out, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(now.out, tl);
}

char ConsoleReadKey()
{
	// Console Inactive
	if (!now.in)
		return false;
	// Get KeyPress
	auto key = char{ 0 };
	auto keysread = DWORD{ 0 };
	ReadConsole(now.in, &key, 1, &keysread, nullptr);
	return key;
}