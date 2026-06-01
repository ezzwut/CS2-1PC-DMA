#pragma once
#include "Game.h"
#include "Bone.h"
#include <map>

namespace MenuConfig
{
	inline int MaxFrameRate = 300;

	inline int SelectedLanguage = 0;

	inline std::string path = "config";

	inline bool ShowBoneESP = true;
	inline bool ShowBoxESP = true;
	inline bool ShowHealthBar = true;
	inline bool ShowArmorBar = true;
	inline bool ShowWeaponESP = true;
	inline bool ShowDistance = true;
	inline bool ShowEyeRay = false;
	inline bool ShowPlayerName = true;
	inline bool ShowHeadESP = false;
	inline bool DarkMode = false;
	
	// 0: normal 1: dynamic
	inline int  BoxType = 0;
	// 0: Vertical 1: Horizontal
	inline int  HealthBarType = 0;

	inline ImColor BoxColor = ImColor(61, 171, 155, 255);
	inline ImColor BoneColor = ImColor(255, 255, 255, 255);
	inline ImColor HeadESPColor = ImColor(255, 0, 0, 255);
	inline ImColor EyeRayColor = ImColor(255, 0, 0, 255);

	inline bool ShowMenu = true;

	inline bool ShowRadar = false;

	inline int RadarType = 0;

	inline bool TeamCheck = true;
	inline bool BypassOBS = false;
	inline int VisibleCheck = 1;
	inline bool ShowConsole = false;

	inline bool ShowLineToEnemy = false;
	inline ImColor LineToEnemyColor = ImColor(255, 255, 255, 220);

}

namespace GunList {
	inline std::vector <std::string> pistolsList = { "hkp2000", "glock", "cz75a", "deagle", "elite", "fiveseven", "p250", "revolver", "tec9", "usp_silencer" };

	inline std::vector <std::string> snipersList = { "awp", "g3sg1", "scar20", "ssg08" };

	inline std::vector <std::string> shotgunsList = { "mag7", "nova", "sawedoff", "xm1014" };

	inline std::vector <std::string> riflesList = { "ak47", "aug", "famas", "galilar", "m4a1", "m4a1_silencer", "m4a4" };

}

namespace KmBox {
	inline std::string type;
}
