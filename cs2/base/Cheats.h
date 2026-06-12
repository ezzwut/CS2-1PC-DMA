#pragma once

#include <mutex>
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
	inline std::vector<CEntity> RenderEntityList;
	inline std::mutex EntityMutex;

	inline char mapname[32];
}
