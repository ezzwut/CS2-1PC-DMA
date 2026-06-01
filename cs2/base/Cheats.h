#pragma once

#include "../radar/mapsdata.h"
#include "Game.h"
#include "Entity.h"
#include "../radar/Radar.h"


namespace Cheats
{
	void Menu();
	void RadarSetting(Base_Radar& Radar);
	void Run();
	inline CEntity LocalEntityPlayer;

	inline std::vector<CEntity> EntityList;

	inline char mapname[32];
}
