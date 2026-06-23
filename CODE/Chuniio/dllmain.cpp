#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    (void) hModule;
    (void) ul_reason_for_call;
    (void) lpReserved;

    return TRUE;
}
