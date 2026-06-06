#include "CheatsThread.h"

#include <winnt.h>
#include <thread>

using namespace Cheats;

VOID UpdateMatrix()
{
	// Matrix is now read inside ScatterReadThreads for atomic sync
	while (true)
	{
		Sleep(10000);
	}
}

VOID LoadLocalEntity()
{
	while (true)
	{
		try {
			Sleep(100);
			DWORD64 LocalControllerAddress = 0;
			DWORD64 LocalPawnAddress = 0;
			
			ProcessMgr.ReadMemory(gGame.GetLocalControllerAddress(), LocalControllerAddress);

			if (LocalControllerAddress == 0)
			{
				DWORD64 listEntry = gGame.GetEntityListEntry();
				if (listEntry != 0) {
					for (int i = 0; i < 128; i++)
					{
						DWORD64 EntityAddress = 0;
						ProcessMgr.ReadMemory<DWORD64>(listEntry + (i + 1) * 0x70, EntityAddress);
						if (EntityAddress == 0) continue;

						CEntity temp;
						temp.Controller.Address = EntityAddress;
						if (temp.Controller.GetIsLocalPlayerController()) {
							LocalControllerAddress = EntityAddress;
							break;
						}
					}
				}
			}

			if (LocalControllerAddress == 0)
				continue;

			CEntity LocalPlayer;
			if (!LocalPlayer.UpdateController(LocalControllerAddress))
				continue;
				
			LocalPawnAddress = LocalPlayer.Pawn.Address;
			if (LocalPawnAddress == 0)
				continue;

			if (!LocalPlayer.UpdatePawn(LocalPawnAddress))
				continue;

			LocalEntityPlayer = LocalPlayer;
		}
		catch (const std::exception& ex) {
		}
	}
}

std::vector<CEntity> TempEntityList;

bool isSameCEntity(const std::vector<CEntity>& list1, const std::vector<CEntity>& list2) {
	return list1.size() == list2.size();
}

VOID LoadEntity()
{
	while (true)
	{
		try {
			if (LocalEntityPlayer.Controller.Address == 0) {
				Sleep(500);
				continue;
			}

			VMMDLL_SCATTER_HANDLE handle = ProcessMgr.CreateScatterHandle();
			DWORD64 EntityAddresses[128]{ 0 };
			for (int i = 0; i < 128; i++)
			{
				ProcessMgr.AddScatterReadRequest(handle, gGame.GetEntityListEntry() + (i + 1) * 0x70, &EntityAddresses[i], sizeof(DWORD64));
			}
			ProcessMgr.ExecuteReadScatter(handle);

			TempEntityList.clear();
			for (int i = 0; i < 128; i++)
			{
				DWORD64 EntityAddress = EntityAddresses[i];
				if (EntityAddress == 0)
					continue;

				if (EntityAddress == LocalEntityPlayer.Controller.Address)
				{
					LocalEntityPlayer.LocalPlayerControllerIndex = i;
					continue;
				}

				CEntity Entity;
				if (!Entity.UpdateController(EntityAddress))
					continue;

				if (!Entity.UpdatePawn(Entity.Pawn.Address))
					continue;

				if (!Entity.IsAlive())
					continue;

				TempEntityList.push_back(Entity);
			}

			if (!isSameCEntity(EntityList, TempEntityList)) {
				EntityList = TempEntityList;
			}
		}
		catch (const std::exception& ex) {
		}
		Sleep(500);
	}
}

VOID UpdateEntityListEntry()
{
	while (true)
	{
		try {
			gGame.UpdateEntityListEntry();

			uintptr_t mapaddress;
			uintptr_t mapaddress2;

			ProcessMgr.ReadMemory(gGame.GetClientDLLAddress() + Offset::GlobalVars, mapaddress);
			ProcessMgr.ReadMemory(mapaddress + 0x0180, mapaddress2);
			ProcessMgr.ReadMemory(mapaddress2, mapname);

			Sleep(5000);
		}
		catch (const std::exception& ex) {
			//std::cout << "4: " << ex.what() << std::endl;
		}
		catch (const std::string& ex) {
			//std::cout << "4: " << ex << std::endl;
		}
		catch (...) {
			//std::cout << "4: " << std::endl;
		}
	}
}

VOID UpdateWeaponName(int index) {
	if (EntityList.size() <= index) return;
	EntityList[index].Pawn.GetWeaponName();
}

VOID UpdateVlue(int index) {
	std::vector<BoneJointPos> BonePosList;
	try {
		for (int i = 0; i < 30; i++)
		{
			Vec2 ScreenPos;
			bool IsVisible = false;

			if (EntityList.size() <= index) return;

			if (gGame.View.WorldToScreen(EntityList[index].TempBoneArray[i].Pos, ScreenPos))
				IsVisible = true;

			if (EntityList.size() <= index) return;
			BonePosList.push_back({ EntityList[index].TempBoneArray[i].Pos ,ScreenPos,IsVisible });
		}
		if (EntityList.size() <= index) return;

		EntityList[index].Pawn.BoneData.BonePosList = BonePosList;

		EntityList[index].Pawn.Pos = EntityList[index].TempPos;
		EntityList[index].Pawn.Health = EntityList[index].TempHealth;

		BonePosList.clear();
	}
	catch (const std::exception& ex) {
		BonePosList.clear();
	}
}

VOID ScatterReadThreads()
{
	while (true)
	{
		try {
			Sleep(3);

			VMMDLL_SCATTER_HANDLE handle = ProcessMgr.CreateScatterHandle();

			// Matrix MUST be in the same scatter batch as entity data for atomic sync
			ProcessMgr.AddScatterReadRequest(handle, gGame.GetMatrixAddress(), gGame.View.Matrix, 64);

			for (int i = 0; i < EntityList.size(); i++)
			{
				ProcessMgr.AddScatterReadRequest(handle, EntityList[i].Pawn.BoneData.BoneArrayAddress, EntityList[i].TempBoneArray, 30 * sizeof(BoneJointData));
				ProcessMgr.AddScatterReadRequest(handle, EntityList[i].Pawn.Address + Offset::Pos, &EntityList[i].TempPos, sizeof(Vec3));
				ProcessMgr.AddScatterReadRequest(handle, EntityList[i].Pawn.Address + Offset::CurrentHealth, &EntityList[i].TempHealth, sizeof(int));
			}
			ProcessMgr.ExecuteReadScatter(handle);

			for (int i = 0; i < EntityList.size(); i++)
			{
				UpdateVlue(i);
			}
		}
		catch (const std::exception& ex) {
		}
		catch (const std::string& ex) {
		}
		catch (...) {
		}
	}
}

VOID UpdateWeaponNameThreads()
{
	while (true)
	{
		try {
			Sleep(1000);

			for (int i = 0; i < EntityList.size(); i++)
			{
				UpdateWeaponName(i);
			}
		}
		catch (const std::exception& ex) {
			//std::cout << "6: " << ex.what() << std::endl;
		}
		catch (const std::string& ex) {
			//std::cout << "6: " << ex << std::endl;
		}
		catch (...) {
			//std::cout << "6: " << std::endl;
		}
	}
}

VOID KeysCheckThread() {
	while (true)
	{
		Sleep(10);
		Keys::LeftKey = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
		Keys::MenuKey = (GetAsyncKeyState(VK_INSERT) & 0x8000) != 0;
	}
}
