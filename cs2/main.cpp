#include "setmgr/SettingsManager.h"

#include "Language.h"
#include "Globals.h"


#ifndef NTSTATUS
typedef long NTSTATUS;
#endif

#include "CheatsThread.h"
#include "base/MenuConfig.hpp"
#include <iostream>
#include <filesystem>
#include <windows.h>

#pragma comment(lib, "D3DX11.lib")

namespace fs = std::filesystem;

std::string readFile(const std::string& path) {
	std::ifstream file(path);
	if (!file) return "";
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}


#include <TlHelp32.h>

void KillOtherInstances() {
    DWORD currentPid = GetCurrentProcessId();
    char currentExeName[MAX_PATH];
    GetModuleFileNameA(NULL, currentExeName, MAX_PATH);
    std::string exePath = currentExeName;
    size_t lastSlash = exePath.find_last_of("\\/");
    std::string exeName = (lastSlash != std::string::npos) ? exePath.substr(lastSlash + 1) : exePath;

    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnap, &pe)) {
            do {
                if (pe.th32ProcessID != currentPid) {
                    std::string pName;
#ifdef UNICODE
                    std::wstring wName = pe.szExeFile;
                    pName = std::string(wName.begin(), wName.end());
#else
                    pName = pe.szExeFile;
#endif
                    if (pName.find("SysUpdate") == 0 || pName == exeName) {
                        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
                        if (hProcess) {
                            TerminateProcess(hProcess, 0);
                            CloseHandle(hProcess);
                        }
                    }
                }
            } while (Process32Next(hSnap, &pe));
        }
        CloseHandle(hSnap);
    }
}

void main(HMODULE module) {
	SetConsoleTitleA("SysUpdate");
	KillOtherInstances();

	std::cout << " -- Beta Esp by ezzwut -- " << std::endl << std::endl;

	std::cout << "[ DMA ] Starting..." << std::endl;

	auto ProcessStatus = ProcessMgr.Attach("cs2.exe");

	if (ProcessStatus != StatusCode::SUCCEED)
	{
		std::cout << "[ DMA ] Error! Failed to attach process, StatusCode:" << ProcessStatus << std::endl;
		system("pause"); // user pause
		return;
	}

	std::cout << "[ DMA ] Attached to game process" << std::endl;

	std::string offsets = readFile("offsets.json");

	std::cout << "[ DMA ] Readed offsets.json" << std::endl;

	std::string client = readFile("client_dll.json");

	std::cout << "[ DMA ] Readed client_dll.json" << std::endl;

	Offset::UpdateOffsets(offsets, client);

	std::cout << "[ DMA ] Updated offsets" << std::endl;

	ProcessMgr.init_keystates();

	if (!gGame.InitAddress())
	{
		std::cout << "[ DMA ] Error! Failed to call InitAddress()." << std::endl;
		return;
	}

	// Verify offsets are correct
	std::cout << "[ DMA ] Verifying offsets..." << std::endl;
	DWORD64 testEntityList = 0;
	ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListAddress(), testEntityList);
	DWORD64 testLocalCtrl = 0;
	ProcessMgr.ReadMemory<DWORD64>(gGame.GetLocalControllerAddress(), testLocalCtrl);

	if (testEntityList == 0 && testLocalCtrl == 0) {
		std::cout << std::endl;
		std::cout << "[ DMA ] ========================================================" << std::endl;
		std::cout << "[ DMA ] ERROR: OFFSETS NOT UP TO DATE!                          " << std::endl;
		std::cout << "[ DMA ] The game has updated and the cheat cannot read memory.   " << std::endl;
		std::cout << "[ DMA ] Please close this window and double click the            " << std::endl;
		std::cout << "[ DMA ] 'update_offsets.bat' file to fix it automatically!       " << std::endl;
		std::cout << "[ DMA ] ========================================================" << std::endl;
		std::cout << std::endl;
		system("pause");
		return;
	}
	std::cout << "[ DMA ] Offsets are up to date!" << std::endl;

	if (!fs::directory_entry(MenuConfig::path).exists()) {
		fs::create_directory(MenuConfig::path);
		std::cout << "[ DMA ] Created config folder" << std::endl;
	}

	std::cout << "[ DMA ] Starting threads..." << std::endl;

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(UpdateMatrix), NULL, 0, 0);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(LoadLocalEntity), NULL, 0, 0);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(LoadEntity), NULL, 0, 0);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(UpdateEntityListEntry), NULL, 0, 0);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(ScatterReadThreads), NULL, 0, 0);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(UpdateWeaponNameThreads), NULL, 0, 0);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(KeysCheckThread), NULL, 0, 0);

	if (settingsJson.language == "en") lang.english();
	else if (settingsJson.language == "de") lang.german();
	else if (settingsJson.language == "tr") lang.turkish();
	else lang.english();

	SetThreadPriority(GetCurrentThread(), HIGH_PRIORITY_CLASS);

	std::cout << "[ DMA ] Enjoy gameplay $" << std::endl;

	Gui.AttachAnotherWindow("Counter-Strike 2", "SDL_app", Cheats::Run);
}