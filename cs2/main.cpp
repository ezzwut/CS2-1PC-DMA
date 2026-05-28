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


void main(HMODULE module) {
	SetConsoleTitleA("SysUpdate");
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	std::cout << " -- Software coded by github.com/ezzwut -- " << std::endl << std::endl;

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