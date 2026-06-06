#include "Entity.h"

bool CEntity::UpdateController(const DWORD64& PlayerControllerAddress)
{
	if (PlayerControllerAddress == 0)
		return false;

	this->Controller.Address = PlayerControllerAddress;

	if (!this->Controller.GetHealth())
		return false;

	if (!this->Controller.GetArmor())
		return false;

	if (!this->Controller.GetIsAlive())
		return false;

	if (!this->Controller.GetTeamID())
		return false;

	if (!this->Controller.GetPlayerName())
		return false;

	this->Pawn.Address = this->Controller.GetPlayerPawnAddress();
	if (this->Pawn.Address == 0)
		return false;

	return true;
}

bool CEntity::UpdatePawn(const DWORD64& PlayerPawnAddress)
{
	if (PlayerPawnAddress == 0)
		return false;

	this->Pawn.Address = PlayerPawnAddress;

	// === CRITICAL FIELDS ONLY — ScatterReadThreads handles the rest ===
	if (!this->Pawn.GetPos())
		return false;
	if (!this->Pawn.GetHealth())
		return false;
	if (!this->Pawn.GetTeamID())
		return false;

	// Resolve bone array address so scatter thread knows where to read
	DWORD64 GameSceneNode = 0;
	if (ProcessMgr.ReadMemory<DWORD64>(PlayerPawnAddress + Offset::GameSceneNode, GameSceneNode) && GameSceneNode != 0) {
		ProcessMgr.ReadMemory<DWORD64>(GameSceneNode + Offset::BoneArray, this->Pawn.BoneData.BoneArrayAddress);
	}

	// Camera pos fallback
	if (!this->Pawn.GetCameraPos()) {
		this->Pawn.CameraPos = this->Pawn.Pos;
	}
	this->Pawn.GetViewAngle(); // needed for initial render

	return true;
}

bool PlayerController::GetTeamID()
{
	return GetDataAddressWithOffset<int>(Address, Offset::TeamID, this->TeamID);
}

bool PlayerController::GetHealth()
{
	return GetDataAddressWithOffset<int>(Address, Offset::Health, this->Health);
}


bool PlayerController::GetIsAlive()
{
	return GetDataAddressWithOffset<int>(Address, Offset::IsAlive, this->AliveStatus);
}

bool PlayerController::GetPlayerName()
{
	char Buffer[MAX_PATH]{};

	if (!ProcessMgr.ReadMemory(Address + Offset::iszPlayerName, Buffer, MAX_PATH))
		return false;

	this->PlayerName = Buffer;
	if (this->PlayerName.empty())
		this->PlayerName = "Name_None";

	return true;
}

bool PlayerController::GetArmor()
{
	if (!GetDataAddressWithOffset<int>(Address, Offset::Armor, this->Armor))
		this->Armor = 0;
	return true;
}

bool PlayerController::GetMoney()
{
	DWORD64 addr;
	GetDataAddressWithOffset<DWORD64>(Address, Offset::MoneyService, addr);
	return GetDataAddressWithOffset<int>(addr, 64, this->Money);
}

bool PlayerController::GetIsLocalPlayerController()
{
	bool isLocal = false;
	GetDataAddressWithOffset<bool>(Address, Offset::bIsLocalPlayerController, isLocal);
	return isLocal;
}

bool PlayerPawn::GetViewAngle()
{
	return GetDataAddressWithOffset<Vec2>(Address, Offset::angEyeAngles, this->ViewAngle);
}

bool PlayerPawn::GetCameraPos()
{
	// Try original field first
	if (Offset::vecLastClipCameraPos != 0) {
		return GetDataAddressWithOffset<Vec3>(Address, Offset::vecLastClipCameraPos, this->CameraPos);
	}
	// Fallback: use vecLastCameraSetupLocalOrigin (June 2 2026+ builds)
	if (Offset::vecLastCameraSetupLocalOrigin != 0) {
		return GetDataAddressWithOffset<Vec3>(Address, Offset::vecLastCameraSetupLocalOrigin, this->CameraPos);
	}
	// Both missing: will fallback to Pos in UpdatePawn
	return false;
}

bool PlayerPawn::GetSpotted()
{
	return GetDataAddressWithOffset<DWORD64>(Address, Offset::bSpottedByMask, this->bSpottedByMask);
}

bool PlayerPawn::GetWeaponName()
{
	this->WeaponName = "Unknown";
	try {
		DWORD64 WeaponServices = 0;
		if (!ProcessMgr.ReadMemory(this->Address + Offset::WeaponServices, WeaponServices) || WeaponServices == 0)
			return true;

		DWORD ActiveWeaponHandle = 0;
		if (!ProcessMgr.ReadMemory(WeaponServices + Offset::ActiveWeapon, ActiveWeaponHandle) || ActiveWeaponHandle == 0xFFFFFFFF)
			return true;

		DWORD ActiveWeaponIndex = ActiveWeaponHandle & 0x7FFF;
		DWORD64 EntityListEntry = 0;
		if (!ProcessMgr.ReadMemory(gGame.GetEntityListAddress() + 8 * (ActiveWeaponIndex >> 9) + 0x10, EntityListEntry) || EntityListEntry == 0)
			return true;

		DWORD64 BaseWeapon = 0;
		if (!ProcessMgr.ReadMemory(EntityListEntry + 120 * (ActiveWeaponIndex & 0x1FF), BaseWeapon) || BaseWeapon == 0)
			return true;

		DWORD64 VData = 0;
		if (!ProcessMgr.ReadMemory(BaseWeapon + Offset::pEntity, VData) || VData == 0)
			return true;

		DWORD64 szNamePtr = 0;
		if (!ProcessMgr.ReadMemory(VData + Offset::designerName, szNamePtr) || szNamePtr == 0)
			return true;

		char Buffer[MAX_PATH]{};
		if (!ProcessMgr.ReadMemory(szNamePtr, Buffer, MAX_PATH))
			return true;
			
		std::string weaponName = std::string(Buffer);
		size_t prefixPos = weaponName.find("weapon_");
		if (prefixPos != std::string::npos) {
			weaponName = weaponName.substr(prefixPos + 7);
		}
		this->WeaponName = weaponName;

		return true;
	}
	catch (const std::exception& ex) {
		return true;
	}
}

bool PlayerPawn::GetShotsFired()
{
	return GetDataAddressWithOffset<DWORD>(Address, Offset::iShotsFired, this->ShotsFired);
}

bool PlayerPawn::GetAimPunchAngle()
{
	// Try original direct field first
	if (Offset::aimPunchAngle != 0) {
		return GetDataAddressWithOffset<Vec2>(Address, Offset::aimPunchAngle, this->AimPunchAngle);
	}
	// Fallback: read through AimPunchServices pointer (June 2 2026+ builds)
	if (Offset::AimPunchServices != 0 && Offset::AimPunchAngleInService != 0) {
		DWORD64 punchService = 0;
		if (ProcessMgr.ReadMemory<DWORD64>(Address + Offset::AimPunchServices, punchService) && punchService != 0) {
			return GetDataAddressWithOffset<Vec2>(punchService, Offset::AimPunchAngleInService, this->AimPunchAngle);
		}
	}
	// Both missing: zero out and succeed
	this->AimPunchAngle.x = 0;
	this->AimPunchAngle.y = 0;
	return true;
}

bool PlayerPawn::GetTeamID()
{
	return GetDataAddressWithOffset<int>(Address, Offset::iTeamNum, this->TeamID);
}

bool PlayerPawn::GetAimPunchCache()
{
	if (Offset::aimPunchCache == 0) {
		// Field removed in June 2 2026 update — zero out and succeed
		this->AimPunchCache.Count = 0;
		this->AimPunchCache.Data = 0;
		return true;
	}
	return GetDataAddressWithOffset<C_UTL_VECTOR>(Address, Offset::aimPunchCache, this->AimPunchCache);
}

DWORD64 PlayerController::GetPlayerPawnAddress()
{
	DWORD64 EntityPawnListEntry = 0;
	DWORD64 EntityPawnAddress = 0;

	if (!GetDataAddressWithOffset<DWORD>(Address, Offset::PlayerPawn, this->Pawn))
		return 0;

	if (!ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListAddress(), EntityPawnListEntry))
		return 0;

	if (!ProcessMgr.ReadMemory<DWORD64>(EntityPawnListEntry + 0x10 + 8 * ((Pawn & 0x7FFF) >> 9), EntityPawnListEntry))
		return 0;

	if (!ProcessMgr.ReadMemory<DWORD64>(EntityPawnListEntry + 0x70 * (Pawn & 0x1FF), EntityPawnAddress))
		return 0;

	return EntityPawnAddress;
}

bool PlayerPawn::GetPos()
{
	return GetDataAddressWithOffset<Vec3>(Address, Offset::Pos, this->Pos);
}

bool PlayerPawn::GetHealth()
{
	return GetDataAddressWithOffset<int>(Address, Offset::CurrentHealth, this->Health);
}

bool PlayerPawn::GetFov()
{
	DWORD64 CameraServices = 0;
	if (!ProcessMgr.ReadMemory<DWORD64>(Address + Offset::CameraServices, CameraServices))
		return false;
	return GetDataAddressWithOffset<int>(CameraServices, Offset::iFovStart, this->Fov);
}

bool PlayerPawn::GetFFlags()
{
	return GetDataAddressWithOffset<int>(Address, Offset::fFlags, this->fFlags);
}

bool CEntity::IsAlive()
{
	return this->Controller.AliveStatus == 1 && this->Pawn.Health > 0;
}

bool CEntity::IsInScreen()
{
	return gGame.View.WorldToScreen(this->Pawn.Pos, this->Pawn.ScreenPos);
}

CBone CEntity::GetBone() const
{
	if (this->Pawn.Address == 0)
		return CBone{};
	return this->Pawn.BoneData;
}