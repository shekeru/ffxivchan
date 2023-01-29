// #include "resource.h"
#include "context.h"
#include "hooks.h"
#include "offsets.h"

PVOID o_security_check_cookie;
void _cdecl hk_security_check_cookie
(uintptr_t StackCookie) {
    ;
};

PVOID oIsDebuggerPresent;
BOOL _stdcall hkIsDebuggerPresent() {
    return false;
};

VOID WINAPI ModuleEntry(HMODULE hInstance) {
    Utils::EnableConsole();
    auto ctx = Context::Current(true);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    IconSwaps::IsIconReplaceable::AttachHook();
    
    //Offsets::GetIcon.ToFunction(IconSwaps::GetIcon_Test).AttachHook(ctx.game);
    
    IconSwaps::GetIcon::AttachHook();

    // Anti-Anti-Debugger
    oIsDebuggerPresent = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "IsDebuggerPresent");
    DetourAttach(&oIsDebuggerPresent, hkIsDebuggerPresent);
    // Fuck your Stack Cookies
    o_security_check_cookie = Offsets::security_check_cookie.Resolve(ctx.game);
    DetourAttach(&o_security_check_cookie, hk_security_check_cookie);

    DetourTransactionCommit();

    // Load Resource Files
    // HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(IDR_FONT1), MAKEINTRESOURCE(RT_RCDATA));
    // overlay.font_data = LockResource(LoadResource(hInstance, hRes));
    // overlay.font_size = SizeofResource(hInstance, hRes);
    
    ctx.gui.prevProc = (WNDPROC) SetWindowLongPtr(ctx.gui.hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
    // Disable/Renable Debugging
    ctx.game.EnableDebug();

    while (!ctx.Stop) {
        Sleep(1000);

    }; // overlay.~IM_Engine();
}

BOOL APIENTRY DllMain(
    HMODULE hInstance,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
) {
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DetourRestoreAfterWith();
        DisableThreadLibraryCalls(hInstance);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)
            ModuleEntry, hInstance, 0, 0); break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        // DetourTransactionBegin();
        // DetourUpdateThread(GetCurrentThread());
        //
        //On_Death_1::ReleaseHook();
        // DetourTransactionCommit();

        //Utils::DisableConsole();
        break;
    }
    return TRUE;
}