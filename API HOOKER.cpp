//////////////////////////////////////////////////////////////////////////////
//
//  Detours Test Program (simple.cpp of simple.dll)
//
//  Microsoft Research Detours Package, Version 3.0.
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
//  This DLL will detour the Windows SleepEx API so that TimedSleep function
//  gets called instead.  TimedSleepEx records the before and after times, and
//  calls the real SleepEx API through the TrueSleepEx function pointer.
//
#include <stdio.h>
#include <windows.h>
#include "detours.h"
#pragma comment(lib, "detours.lib")


static LONG dwSlept = 0;

static HWND (WINAPI * SysCreateWindowExA)(
  _In_      DWORD dwExStyle,
  _In_opt_  LPCSTR lpClassName,
  _In_opt_  LPCSTR lpWindowName,
  _In_      DWORD dwStyle,
  _In_      int x,
  _In_      int y,
  _In_      int nWidth,
  _In_      int nHeight,
  _In_opt_  HWND hWndParent,
  _In_opt_  HMENU hMenu,
  _In_opt_  HINSTANCE hInstance,
  _In_opt_  LPVOID lpParam
)=CreateWindowExA;

static HWND (WINAPI * SysCreateWindowExW)(
  _In_      DWORD dwExStyle,
  _In_opt_  LPCWSTR lpClassName,
  _In_opt_  LPCWSTR lpWindowName,
  _In_      DWORD dwStyle,
  _In_      int x,
  _In_      int y,
  _In_      int nWidth,
  _In_      int nHeight,
  _In_opt_  HWND hWndParent,
  _In_opt_  HMENU hMenu,
  _In_opt_  HINSTANCE hInstance,
  _In_opt_  LPVOID lpParam
)=CreateWindowExW;




HWND WINAPI MyCreateWindowExA(
  _In_      DWORD dwExStyle,
  _In_opt_  LPCSTR lpClassName,
  _In_opt_  LPCSTR lpWindowName,
  _In_      DWORD dwStyle,
  _In_      int x,
  _In_      int y,
  _In_      int nWidth,
  _In_      int nHeight,
  _In_opt_  HWND hWndParent,
  _In_opt_  HMENU hMenu,
  _In_opt_  HINSTANCE hInstance,
  _In_opt_  LPVOID lpParam
)
{

	return SysCreateWindowExA(
dwExStyle,
lpClassName,
lpWindowName,
WS_POPUPWINDOW,
x,
y,
800,
600,
hWndParent,
hMenu,
hInstance,
lpParam
);
}


HWND WINAPI MyCreateWindowExW(
  _In_      DWORD dwExStyle,
  _In_opt_  LPCWSTR lpClassName,
  _In_opt_  LPCWSTR lpWindowName,
  _In_      DWORD dwStyle,
  _In_      int x,
  _In_      int y,
  _In_      int nWidth,
  _In_      int nHeight,
  _In_opt_  HWND hWndParent,
  _In_opt_  HMENU hMenu,
  _In_opt_  HINSTANCE hInstance,
  _In_opt_  LPVOID lpParam)
{
	return SysCreateWindowExW(
dwExStyle,
lpClassName,
lpWindowName,
WS_POPUPWINDOW,
x,
y,
800,
600,
hWndParent,
hMenu,
hInstance,
lpParam);
}





BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
    LONG error;
    (void)hinst;
    (void)reserved;

    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
        DetourRestoreAfterWith();


        fflush(stdout);

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)SysCreateWindowExW, MyCreateWindowExW);
        DetourAttach(&(PVOID&)SysCreateWindowExA, MyCreateWindowExA);
        error = DetourTransactionCommit();


    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
 
        DetourDetach(&(PVOID&)SysCreateWindowExW, MyCreateWindowExW);
        DetourDetach(&(PVOID&)SysCreateWindowExA, MyCreateWindowExA);		
		
        error = DetourTransactionCommit();

        fflush(stdout);
    }
    return TRUE;
}


