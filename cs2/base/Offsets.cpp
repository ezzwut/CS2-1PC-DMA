#include "Offsets.h"

#include "rapidjson/document.h"
#include <iostream>
#include <iomanip>

using namespace rapidjson;

// Safe helper: returns field value or 0 if missing
static DWORD SafeGetField(const Value& classObj, const char* fieldName) {
	if (!classObj.HasMember("fields")) return 0;
	const auto& fields = classObj["fields"];
	if (!fields.HasMember(fieldName)) return 0;
	return (DWORD)fields[fieldName].GetUint64();
}

bool Offset::UpdateOffsets(std::string offsetdata, std::string clientdata)
{
	Document offsets, client;
	try {
		offsets.Parse(offsetdata.c_str());
		if (offsets.HasParseError()) {
			std::cout << "[ DMA ] ERROR: Failed to parse offsets.json. Make sure the file is valid JSON." << std::endl;
			return false;
		}

		client.Parse(clientdata.c_str());
		if (client.HasParseError()) {
			std::cout << "[ DMA ] ERROR: Failed to parse client_dll.json. Make sure the file is valid JSON." << std::endl;
			return false;
		}
	} catch (...) {
		std::cout << "[ DMA ] ERROR: Exception during JSON parsing." << std::endl;
		return false;
	}

	offsetdata.clear(); clientdata.clear();

	// Shorthand references
	const auto& classes = client["client.dll"]["classes"];

	// === Main offsets from offsets.json ===
	Offset::EntityList = offsets["client.dll"]["dwEntityList"].GetUint64();
	Offset::Matrix = offsets["client.dll"]["dwViewMatrix"].GetUint64();
	Offset::ViewAngle = offsets["client.dll"]["dwViewAngles"].GetUint64();
	Offset::LocalPlayerController = offsets["client.dll"]["dwLocalPlayerController"].GetUint64();
	Offset::LocalPlayerPawn = offsets["client.dll"]["dwLocalPlayerPawn"].GetUint64();
	Offset::GlobalVars = offsets["client.dll"]["dwGlobalVars"].GetUint64();

	// === Fields from client_dll.json (with safe fallbacks) ===
	Offset::Health = SafeGetField(classes["C_BaseEntity"], "m_iHealth");
	Offset::TeamID = SafeGetField(classes["C_BaseEntity"], "m_iTeamNum");
	Offset::Armor = SafeGetField(classes["CCSPlayerController"], "m_iPawnArmor");
	Offset::IsAlive = SafeGetField(classes["CCSPlayerController"], "m_bPawnIsAlive");
	Offset::MoneyService = SafeGetField(classes["CCSPlayerController"], "m_pInGameMoneyServices");
	Offset::PlayerPawn = SafeGetField(classes["CCSPlayerController"], "m_hPlayerPawn");
	Offset::iszPlayerName = SafeGetField(classes["CBasePlayerController"], "m_iszPlayerName");
	Offset::bIsLocalPlayerController = SafeGetField(classes["CBasePlayerController"], "m_bIsLocalPlayerController");
	Offset::Pos = SafeGetField(classes["C_BasePlayerPawn"], "m_vOldOrigin");
	Offset::MaxHealth = SafeGetField(classes["C_BaseEntity"], "m_iMaxHealth");
	Offset::CurrentHealth = SafeGetField(classes["C_BaseEntity"], "m_iHealth");
	Offset::GameSceneNode = SafeGetField(classes["C_BaseEntity"], "m_pGameSceneNode");
	Offset::BoneArray = SafeGetField(classes["CSkeletonInstance"], "m_modelState") + 0x80;
	Offset::angEyeAngles = SafeGetField(classes["C_CSPlayerPawn"], "m_angEyeAngles");

	// vecLastClipCameraPos was REMOVED in June 2 2026 update.
	// Use m_vecLastCameraSetupLocalOrigin from C_BasePlayerPawn as replacement.
	Offset::vecLastClipCameraPos = SafeGetField(classes["C_CSPlayerPawn"], "m_vecLastClipCameraPos");
	Offset::vecLastCameraSetupLocalOrigin = SafeGetField(classes["C_BasePlayerPawn"], "m_vecLastCameraSetupLocalOrigin");

	if (Offset::vecLastClipCameraPos == 0 && Offset::vecLastCameraSetupLocalOrigin != 0) {
		std::cout << "[ DMA ] NOTE: m_vecLastClipCameraPos removed, using m_vecLastCameraSetupLocalOrigin (0x"
			<< std::hex << Offset::vecLastCameraSetupLocalOrigin << std::dec << ")" << std::endl;
	}

	Offset::WeaponServices = SafeGetField(classes["C_BasePlayerPawn"], "m_pWeaponServices");
	Offset::ActiveWeapon = SafeGetField(classes["CPlayer_WeaponServices"], "m_hActiveWeapon");
	Offset::pEntity = SafeGetField(classes["CEntityInstance"], "m_pEntity");
	if (Offset::pEntity == 0) Offset::pEntity = 0x10;
	Offset::designerName = SafeGetField(classes["CEntityIdentity"], "m_designerName");
	if (Offset::designerName == 0) Offset::designerName = 0x20;
	Offset::iShotsFired = SafeGetField(classes["C_CSPlayerPawn"], "m_iShotsFired");
	Offset::flFlashDuration = SafeGetField(classes["C_CSPlayerPawnBase"], "m_flFlashDuration");

	// aimPunchAngle and aimPunchCache were REMOVED in June 2 2026 update.
	// They moved behind m_pAimPunchServices (a new pointer on C_CSPlayerPawn).
	Offset::aimPunchAngle = SafeGetField(classes["C_CSPlayerPawn"], "m_aimPunchAngle");
	Offset::aimPunchCache = SafeGetField(classes["C_CSPlayerPawn"], "m_aimPunchCache");
	Offset::AimPunchServices = SafeGetField(classes["C_CSPlayerPawn"], "m_pAimPunchServices");

	if (classes.HasMember("CCSPlayer_AimPunchServices")) {
		Offset::AimPunchAngleInService = SafeGetField(classes["CCSPlayer_AimPunchServices"], "m_predictableBaseAngle");
	}

	if (Offset::aimPunchAngle == 0 && Offset::AimPunchServices != 0) {
		std::cout << "[ DMA ] NOTE: m_aimPunchAngle removed, using AimPunchServices->m_predictableBaseAngle" << std::endl;
	}

	Offset::iIDEntIndex = SafeGetField(classes["C_CSPlayerPawn"], "m_iIDEntIndex");
	Offset::iTeamNum = SafeGetField(classes["C_BaseEntity"], "m_iTeamNum");
	Offset::CameraServices = SafeGetField(classes["C_BasePlayerPawn"], "m_pCameraServices");
	Offset::iFovStart = SafeGetField(classes["CCSPlayerBase_CameraServices"], "m_iFOVStart");
	Offset::fFlags = SafeGetField(classes["C_BaseEntity"], "m_fFlags");

	int m_entitySpottedState = SafeGetField(classes["C_CSPlayerPawn"], "m_entitySpottedState");
	int m_bSpottedByMask = SafeGetField(classes["EntitySpottedState_t"], "m_bSpottedByMask");
	Offset::bSpottedByMask = m_entitySpottedState + m_bSpottedByMask;

	auto print_hex = [](const char* name, uint64_t value) {
		std::cout << std::left << std::setw(30) << name << " : 0x" << std::hex << value << std::dec << std::endl;
	};
	auto print_int = [](const char* name, int value) {
		std::cout << std::left << std::setw(30) << name << " : " << value << std::endl;
	};

	std::cout << "[ DMA ] Loaded Offsets:" << std::endl;
	print_hex("EntityList", Offset::EntityList);
	print_hex("Matrix", Offset::Matrix);
	print_hex("ViewAngle", Offset::ViewAngle);
	print_hex("LocalPlayerController", Offset::LocalPlayerController);
	print_hex("LocalPlayerPawn", Offset::LocalPlayerPawn);
	print_hex("GlobalVars", Offset::GlobalVars);
	print_hex("Health", Offset::Health);
	print_hex("TeamID", Offset::TeamID);
	print_hex("Armor", Offset::Armor);
	print_hex("IsAlive", Offset::IsAlive);
	print_hex("MoneyService", Offset::MoneyService);
	print_hex("PlayerPawn", Offset::PlayerPawn);
	print_hex("iszPlayerName", Offset::iszPlayerName);
	print_hex("Pos", Offset::Pos);
	print_hex("MaxHealth", Offset::MaxHealth);
	print_hex("CurrentHealth", Offset::CurrentHealth);
	print_hex("GameSceneNode", Offset::GameSceneNode);
	print_hex("BoneArray", Offset::BoneArray);
	print_hex("angEyeAngles", Offset::angEyeAngles);
	print_hex("vecLastClipCameraPos", Offset::vecLastClipCameraPos);
	print_hex("vecLastCameraSetupLocal", Offset::vecLastCameraSetupLocalOrigin);
	print_hex("iShotsFired", Offset::iShotsFired);
	print_hex("flFlashDuration", Offset::flFlashDuration);
	print_hex("aimPunchAngle", Offset::aimPunchAngle);
	print_hex("aimPunchCache", Offset::aimPunchCache);
	print_hex("AimPunchServices", Offset::AimPunchServices);
	print_hex("AimPunchAngleInSvc", Offset::AimPunchAngleInService);
	print_hex("iIDEntIndex", Offset::iIDEntIndex);
	print_hex("iTeamNum", Offset::iTeamNum);
	print_hex("CameraServices", Offset::CameraServices);
	print_hex("iFovStart", Offset::iFovStart);
	print_hex("fFlags", Offset::fFlags);

	print_int("m_entitySpottedState", m_entitySpottedState);
	print_int("m_bSpottedByMask", m_bSpottedByMask);
	print_hex("bSpottedByMask", Offset::bSpottedByMask);

	return true;
}