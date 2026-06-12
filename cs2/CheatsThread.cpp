#include "CheatsThread.h"

#include <winnt.h>
#include <thread>
#include <cmath>
#include <algorithm>
#include <chrono>
#include "base/MenuConfig.hpp"

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

// Roster: entity discovery result, written by LoadEntity, read by ScatterReadThreads
static std::vector<CEntity> EntityRoster;
static std::mutex RosterMutex;
static volatile bool LoadEntityBusy = false; // Pause flag for scatter thread
static volatile bool RosterUpdated = false;  // Signifies new roster is available

VOID LoadEntity()
{
	// Persistent state
	static DWORD64 lastLocalPlayerAddress = 0;

	while (true)
	{
		try {
			if (LocalEntityPlayer.Controller.Address == 0) {
				Sleep(500);
				continue;
			}

			int strategy = MenuConfig::DMAStrategy;
			int maxSlots = (strategy == 2 || strategy == 4) ? 64 : 128;
			bool skipTeammates = (strategy == 2 || strategy == 3 || strategy == 4);
			int localTeam = LocalEntityPlayer.Controller.TeamID;

			DWORD64 entityListEntry = gGame.GetEntityListEntry();
			if (entityListEntry == 0) {
				Sleep(100);
				continue;
			}

			// Handle resetting if the local player address changes
			if (LocalEntityPlayer.Controller.Address != lastLocalPlayerAddress) {
				lastLocalPlayerAddress = LocalEntityPlayer.Controller.Address;
				std::lock_guard<std::mutex> lock(RosterMutex);
				EntityRoster.clear();
				RosterUpdated = true;
			}

			auto startScan = std::chrono::steady_clock::now();

			if (strategy == 0) {
				// ----------------------------------------------------
				// LEGACY Strategy 0: Individual Reads (Blocking)
				// ----------------------------------------------------
				LoadEntityBusy = true;
				Sleep(15); // Let scatter thread finish its current batch

				VMMDLL_SCATTER_HANDLE handle = ProcessMgr.CreateScatterHandle();
				std::vector<DWORD64> EntityAddresses(maxSlots, 0);
				for (int i = 0; i < maxSlots; i++) {
					ProcessMgr.AddScatterReadRequest(handle, entityListEntry + (i + 1) * 0x70, &EntityAddresses[i], sizeof(DWORD64));
				}
				ProcessMgr.ExecuteReadScatter(handle);

				std::vector<CEntity> newRoster;
				for (int i = 0; i < maxSlots; i++)
				{
					DWORD64 EntityAddress = EntityAddresses[i];
					if (EntityAddress == 0) continue;
					if (EntityAddress == LocalEntityPlayer.Controller.Address) {
						LocalEntityPlayer.LocalPlayerControllerIndex = i;
						continue;
					}

					CEntity Entity;
					Entity.SlotIndex = i;
					if (!Entity.UpdateController(EntityAddress, skipTeammates, localTeam)) continue;
					if (!Entity.UpdatePawn(Entity.Pawn.Address)) continue;
					if (!Entity.IsAlive()) continue;

					newRoster.push_back(Entity);
				}

				LoadEntityBusy = false;

				{
					std::lock_guard<std::mutex> lock(RosterMutex);
					EntityRoster = std::move(newRoster);
					RosterUpdated = true;
				}

				auto endScan = std::chrono::steady_clock::now();
				MenuConfig::ScanDurationMs = std::chrono::duration<float, std::milli>(endScan - startScan).count();
				Sleep(500);
			}
			else {
				// ----------------------------------------------------
				// PURE SCATTER-BASED DISCOVERY (Strategies 1, 2, 3, 4)
				// ----------------------------------------------------
				LoadEntityBusy = false;

				// Pass 1: Read all controller addresses via Scatter
				VMMDLL_SCATTER_HANDLE handle1 = ProcessMgr.CreateScatterHandle();
				std::vector<DWORD64> ControllerAddresses(maxSlots, 0);
				for (int i = 0; i < maxSlots; i++) {
					ProcessMgr.AddScatterReadRequest(handle1, entityListEntry + (i + 1) * 0x70, &ControllerAddresses[i], sizeof(DWORD64));
				}
				ProcessMgr.ExecuteReadScatter(handle1);

				// Gather active slot indices
				std::vector<int> activeSlots;
				for (int i = 0; i < maxSlots; i++) {
					DWORD64 addr = ControllerAddresses[i];
					if (addr == 0 || addr == LocalEntityPlayer.Controller.Address) continue;
					activeSlots.push_back(i);
				}

				std::vector<CEntity> newRoster;

				if (!activeSlots.empty()) {
					// Pass 2: Read TeamID, IsAlive, and PawnHandle from controllers in one batch
					VMMDLL_SCATTER_HANDLE handle2 = ProcessMgr.CreateScatterHandle();
					struct ControllerData {
						int TeamID = 0;
						int Health = 0;
						int Armor = 0;
						int IsAlive = 0;
						DWORD PawnHandle = 0;
					};
					std::vector<ControllerData> ctrlData(maxSlots);

					for (int idx : activeSlots) {
						DWORD64 addr = ControllerAddresses[idx];
						ProcessMgr.AddScatterReadRequest(handle2, addr + Offset::TeamID, &ctrlData[idx].TeamID, sizeof(int));
						ProcessMgr.AddScatterReadRequest(handle2, addr + Offset::Health, &ctrlData[idx].Health, sizeof(int));
						ProcessMgr.AddScatterReadRequest(handle2, addr + Offset::Armor, &ctrlData[idx].Armor, sizeof(int));
						ProcessMgr.AddScatterReadRequest(handle2, addr + Offset::IsAlive, &ctrlData[idx].IsAlive, sizeof(int));
						ProcessMgr.AddScatterReadRequest(handle2, addr + Offset::PlayerPawn, &ctrlData[idx].PawnHandle, sizeof(DWORD));
					}
					ProcessMgr.ExecuteReadScatter(handle2);

					// Filter based on teammates check and alive status
					std::vector<int> filteredSlots;
					for (int idx : activeSlots) {
						if (skipTeammates && localTeam != 0 && ctrlData[idx].TeamID == localTeam) continue;
						if (ctrlData[idx].IsAlive != 1) continue;
						if (ctrlData[idx].PawnHandle == 0 || ctrlData[idx].PawnHandle == 0xFFFFFFFF) continue;
						filteredSlots.push_back(idx);
					}

					if (!filteredSlots.empty()) {
						// Pass 3 & 4: Resolve Pawn Addresses
						DWORD64 EntityPawnListEntry = 0;
						ProcessMgr.ReadMemory(gGame.GetEntityListAddress(), EntityPawnListEntry);

						if (EntityPawnListEntry != 0) {
							// Pass 3: Get 2nd-level list entry addresses
							VMMDLL_SCATTER_HANDLE handle3 = ProcessMgr.CreateScatterHandle();
							std::vector<DWORD64> listEntry2(maxSlots, 0);

							for (int idx : filteredSlots) {
								DWORD handle = ctrlData[idx].PawnHandle;
								DWORD64 listEntry2Addr = EntityPawnListEntry + 0x10 + 8 * ((handle & 0x7FFF) >> 9);
								ProcessMgr.AddScatterReadRequest(handle3, listEntry2Addr, &listEntry2[idx], sizeof(DWORD64));
							}
							ProcessMgr.ExecuteReadScatter(handle3);

							// Pass 4: Get final pawn addresses
							VMMDLL_SCATTER_HANDLE handle4 = ProcessMgr.CreateScatterHandle();
							std::vector<DWORD64> pawnAddresses(maxSlots, 0);

							for (int idx : filteredSlots) {
								if (listEntry2[idx] == 0) continue;
								DWORD handle = ctrlData[idx].PawnHandle;
								DWORD64 pawnAddrLoc = listEntry2[idx] + 0x70 * (handle & 0x1FF);
								ProcessMgr.AddScatterReadRequest(handle4, pawnAddrLoc, &pawnAddresses[idx], sizeof(DWORD64));
							}
							ProcessMgr.ExecuteReadScatter(handle4);

							// Pass 5: Read Player Name and Pawn Data (Health, Team, Pos, GameSceneNode, ViewAngle, CameraPos)
							VMMDLL_SCATTER_HANDLE handle5 = ProcessMgr.CreateScatterHandle();
							struct PawnData {
								int Health = 0;
								int TeamID = 0;
								Vec3 Pos{};
								DWORD64 GameSceneNode = 0;
								Vec2 ViewAngle{};
								Vec3 CameraPos{};
								char Name[32]{};
							};
							std::vector<PawnData> pData(maxSlots);

							std::vector<int> validPawnSlots;
							for (int idx : filteredSlots) {
								DWORD64 pawnAddr = pawnAddresses[idx];
								if (pawnAddr == 0) continue;
								validPawnSlots.push_back(idx);

								DWORD64 ctrlAddr = ControllerAddresses[idx];
								// Read Name (32 bytes)
								ProcessMgr.AddScatterReadRequest(handle5, ctrlAddr + Offset::iszPlayerName, pData[idx].Name, 32);
								// Read Pawn Data
								ProcessMgr.AddScatterReadRequest(handle5, pawnAddr + Offset::CurrentHealth, &pData[idx].Health, sizeof(int));
								ProcessMgr.AddScatterReadRequest(handle5, pawnAddr + Offset::iTeamNum, &pData[idx].TeamID, sizeof(int));
								ProcessMgr.AddScatterReadRequest(handle5, pawnAddr + Offset::Pos, &pData[idx].Pos, sizeof(Vec3));
								ProcessMgr.AddScatterReadRequest(handle5, pawnAddr + Offset::GameSceneNode, &pData[idx].GameSceneNode, sizeof(DWORD64));
								ProcessMgr.AddScatterReadRequest(handle5, pawnAddr + Offset::angEyeAngles, &pData[idx].ViewAngle, sizeof(Vec2));

								if (Offset::vecLastClipCameraPos != 0) {
									ProcessMgr.AddScatterReadRequest(handle5, pawnAddr + Offset::vecLastClipCameraPos, &pData[idx].CameraPos, sizeof(Vec3));
								} else if (Offset::vecLastCameraSetupLocalOrigin != 0) {
									ProcessMgr.AddScatterReadRequest(handle5, pawnAddr + Offset::vecLastCameraSetupLocalOrigin, &pData[idx].CameraPos, sizeof(Vec3));
								}
							}
							ProcessMgr.ExecuteReadScatter(handle5);

							// Pass 6: Read BoneArray address from GameSceneNode
							VMMDLL_SCATTER_HANDLE handle6 = ProcessMgr.CreateScatterHandle();
							std::vector<DWORD64> boneArrayAddresses(maxSlots, 0);

							for (int idx : validPawnSlots) {
								DWORD64 sceneNode = pData[idx].GameSceneNode;
								if (sceneNode == 0) continue;
								ProcessMgr.AddScatterReadRequest(handle6, sceneNode + Offset::BoneArray, &boneArrayAddresses[idx], sizeof(DWORD64));
							}
							ProcessMgr.ExecuteReadScatter(handle6);

							// Populate new roster
							for (int idx : validPawnSlots) {
								CEntity Entity;
								Entity.SlotIndex = idx;
								Entity.Controller.Address = ControllerAddresses[idx];
								Entity.Controller.TeamID = ctrlData[idx].TeamID;
								Entity.Controller.Health = ctrlData[idx].Health;
								Entity.Controller.Armor = ctrlData[idx].Armor;
								Entity.Controller.AliveStatus = ctrlData[idx].IsAlive;
								Entity.Controller.Pawn = ctrlData[idx].PawnHandle;

								pData[idx].Name[31] = '\0'; // ensure null termination
								Entity.Controller.PlayerName = pData[idx].Name;
								if (Entity.Controller.PlayerName.empty()) {
									Entity.Controller.PlayerName = "Name_None";
								}

								Entity.Pawn.Address = pawnAddresses[idx];
								Entity.Pawn.Health = pData[idx].Health;
								Entity.Pawn.TeamID = pData[idx].TeamID;
								Entity.Pawn.Pos = pData[idx].Pos;
								Entity.Pawn.BoneData.BoneArrayAddress = boneArrayAddresses[idx];
								Entity.Pawn.ViewAngle = pData[idx].ViewAngle;
								Entity.Pawn.CameraPos = pData[idx].CameraPos;
								if (Entity.Pawn.CameraPos.x == 0 && Entity.Pawn.CameraPos.y == 0 && Entity.Pawn.CameraPos.z == 0) {
									Entity.Pawn.CameraPos = Entity.Pawn.Pos;
								}

								Entity.TempHealth = pData[idx].Health;
								Entity.TempPos = pData[idx].Pos;
								Entity.TempViewAngle = pData[idx].ViewAngle;

								newRoster.push_back(Entity);
							}
						}
					}
				}

				{
					std::lock_guard<std::mutex> lock(RosterMutex);
					EntityRoster = std::move(newRoster);
					RosterUpdated = true;
				}

				auto endScan = std::chrono::steady_clock::now();
				MenuConfig::ScanDurationMs = std::chrono::duration<float, std::milli>(endScan - startScan).count();

				// Adaptive Sleep based on strategy
				if (strategy == 3) {
					Sleep(2000); // Strategy 3: Low-frequency 128-slot scan (every 2 seconds)
				}
				else if (strategy == 4) {
					Sleep(250);  // Strategy 4: Full scan every 250ms (extremely light, no lag)
				}
				else {
					Sleep(500);  // Strategy 1, 2: scan every 500ms
				}
			}
		}
		catch (const std::exception& ex) {
			LoadEntityBusy = false;
			Sleep(500);
		}
		catch (...) {
			LoadEntityBusy = false;
			Sleep(500);
		}
	}
}

// Helper: check if a bone position is valid (not garbage from freed memory)
static bool IsBoneValid(const Vec3& pos) {
	if (std::isnan(pos.x) || std::isnan(pos.y) || std::isnan(pos.z)) return false;
	if (std::isinf(pos.x) || std::isinf(pos.y) || std::isinf(pos.z)) return false;
	const float MAX_COORD = 32768.0f;
	if (std::abs(pos.x) > MAX_COORD || std::abs(pos.y) > MAX_COORD || std::abs(pos.z) > MAX_COORD) return false;
	return true;
}

// Process bone world-to-screen for a single entity
static void ProcessBones(int index, std::vector<CEntity>& list) {
	if ((int)list.size() <= index) return;

	// If the entity is dead or bone array address is invalid, skip bone processing
	if (list[index].Pawn.Health <= 0 || list[index].Pawn.BoneData.BoneArrayAddress == 0) {
		list[index].Pawn.Pos = list[index].TempPos;
		list[index].Pawn.Health = list[index].TempHealth;
		return;
	}

	for (int i = 0; i < 30; i++) {
		if (!IsBoneValid(list[index].TempBoneArray[i].Pos)) {
			list[index].Pawn.Pos = list[index].TempPos;
			list[index].Pawn.Health = list[index].TempHealth;
			return;
		}
	}

	std::vector<BoneJointPos> BonePosList;
	BonePosList.reserve(30);
	for (int i = 0; i < 30; i++) {
		Vec2 ScreenPos;
		bool IsVisible = false;
		if (gGame.View.WorldToScreen(list[index].TempBoneArray[i].Pos, ScreenPos))
			IsVisible = true;
		BonePosList.push_back({ list[index].TempBoneArray[i].Pos, ScreenPos, IsVisible });
	}

	list[index].Pawn.BoneData.BonePosList = BonePosList;
	list[index].Pawn.Pos = list[index].TempPos;
	list[index].Pawn.Health = list[index].TempHealth;
}

VOID ScatterReadThreads()
{
	// Working list owned by this thread — never shared directly
	std::vector<CEntity> workingList;

	// Timing and rate metrics
	auto lastFpsTime = std::chrono::steady_clock::now();
	int fpsCount = 0;

	while (true)
	{
		try {
			Sleep(10);

			// Yield to LoadEntity when it's doing legacy individual reads (Strategy 0 only)
			if (MenuConfig::DMAStrategy == 0 && LoadEntityBusy) continue;

			// Check if LoadEntity has a new roster for us
			bool rosterUpdatedLocal = false;
			std::vector<CEntity> tempRoster;
			{
				std::lock_guard<std::mutex> lock(RosterMutex);
				if (RosterUpdated) {
					tempRoster = EntityRoster;
					RosterUpdated = false;
					rosterUpdatedLocal = true;
				}
			}

			if (rosterUpdatedLocal) {
				// Merge: adopt new roster but carry over bone data/weapon names/view angles from old working list
				for (auto& newEnt : tempRoster) {
					for (auto& oldEnt : workingList) {
						if (oldEnt.Controller.Address == newEnt.Controller.Address) {
							// ONLY copy the calculated screen/world bone list (BonePosList)
							// Do NOT copy BoneArrayAddress because newEnt has the fresh, correct BoneArrayAddress!
							newEnt.Pawn.BoneData.BonePosList = oldEnt.Pawn.BoneData.BonePosList;
							
							newEnt.Pawn.Pos = oldEnt.Pawn.Pos;
							newEnt.Pawn.Health = oldEnt.Pawn.Health;
							newEnt.Pawn.WeaponName = oldEnt.Pawn.WeaponName;
							newEnt.Pawn.ViewAngle = oldEnt.Pawn.ViewAngle;
							break;
						}
					}
				}
				workingList = std::move(tempRoster);
			}

			// Clean up and clear render list if working list is empty (e.g. no enemies left or round ended)
			if (workingList.empty()) {
				std::lock_guard<std::mutex> lock(EntityMutex);
				RenderEntityList.clear();
				MenuConfig::ActiveEntitiesCount = 0;
				Sleep(50); // Sleep longer to save DMA bandwidth
				continue;
			}

			auto startScatter = std::chrono::steady_clock::now();

			// Scatter read: matrix + all entity data in one batch
			VMMDLL_SCATTER_HANDLE handle = ProcessMgr.CreateScatterHandle();

			ProcessMgr.AddScatterReadRequest(handle, gGame.GetMatrixAddress(), gGame.View.Matrix, 64);

			for (int i = 0; i < (int)workingList.size(); i++) {
				if (workingList[i].Pawn.Address == 0) continue;
				// Only read bones if the entity was alive in the last frame and has a valid bone array
				if (workingList[i].Pawn.Health > 0 && workingList[i].Pawn.BoneData.BoneArrayAddress != 0) {
					ProcessMgr.AddScatterReadRequest(handle, workingList[i].Pawn.BoneData.BoneArrayAddress, workingList[i].TempBoneArray, 30 * sizeof(BoneJointData));
				}
				ProcessMgr.AddScatterReadRequest(handle, workingList[i].Pawn.Address + Offset::Pos, &workingList[i].TempPos, sizeof(Vec3));
				ProcessMgr.AddScatterReadRequest(handle, workingList[i].Pawn.Address + Offset::CurrentHealth, &workingList[i].TempHealth, sizeof(int));
				ProcessMgr.AddScatterReadRequest(handle, workingList[i].Pawn.Address + Offset::angEyeAngles, &workingList[i].TempViewAngle, sizeof(Vec2));
			}
			ProcessMgr.ExecuteReadScatter(handle);

			auto endScatter = std::chrono::steady_clock::now();
			MenuConfig::ScatterDurationMs = std::chrono::duration<float, std::milli>(endScatter - startScatter).count();

			// Process bones (WorldToScreen)
			for (int i = 0; i < (int)workingList.size(); i++) {
				ProcessBones(i, workingList);
				workingList[i].Pawn.ViewAngle = workingList[i].TempViewAngle;
			}

			// Publish complete snapshot — render thread only sees fully populated data
			{
				std::lock_guard<std::mutex> lock(EntityMutex);
				RenderEntityList = workingList;
				MenuConfig::ActiveEntitiesCount = (int)workingList.size();
			}

			// Calculate scatter read rate (Hz)
			fpsCount++;
			auto now = std::chrono::steady_clock::now();
			if (now - lastFpsTime >= std::chrono::seconds(1)) {
				MenuConfig::ScatterRateHz = fpsCount;
				fpsCount = 0;
				lastFpsTime = now;
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

			if (!MenuConfig::ShowWeaponESP) {
				std::lock_guard<std::mutex> lock(EntityMutex);
				for (auto& ent : RenderEntityList) {
					ent.Pawn.WeaponName.clear();
				}
				continue;
			}

			std::vector<CEntity> localList;
			{
				std::lock_guard<std::mutex> lock(EntityMutex);
				localList = RenderEntityList;
			}

			for (int i = 0; i < (int)localList.size(); i++) {
				if (localList[i].Pawn.Health > 0 && localList[i].Pawn.Address != 0) {
					localList[i].Pawn.GetWeaponName();
				}
			}

			{
				std::lock_guard<std::mutex> lock(EntityMutex);
				if (RenderEntityList.size() == localList.size()) {
					for (int i = 0; i < (int)localList.size(); i++) {
						if (RenderEntityList[i].Controller.Address == localList[i].Controller.Address) {
							RenderEntityList[i].Pawn.WeaponName = localList[i].Pawn.WeaponName;
						}
					}
				}
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

VOID KeysCheckThread() {
	while (true)
	{
		Sleep(10);
		Keys::LeftKey = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
		Keys::MenuKey = (GetAsyncKeyState(VK_INSERT) & 0x8000) != 0;
	}
}
