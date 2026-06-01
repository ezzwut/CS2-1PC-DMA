#include "../KeysValues.h"

#include "GUI.h"
#include "../Globals.h"

#include "Cheats.h"
#include "Render.hpp"
#include "MenuConfig.hpp"
#include "../utils/ConfigMenu.hpp"
#include "../utils/ConfigSaver.hpp"


void Cheats::RadarSetting(Base_Radar& Radar)
{
	ImGui::SetNextWindowBgAlpha(0.1f);
	ImGui::Begin("Radar", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	ImGui::SetWindowSize({ mp::RadarSize, mp::RadarSize });

	Radar.SetDrawList(ImGui::GetWindowDrawList());

	Radar.SetPos({ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y });
	Radar.Opened = true;
}

void Cheats::Run()
{
	HWND hwnd = FindWindowA("SDL_app", "Counter-Strike 2");
	HWND fg = GetForegroundWindow();
	if (hwnd && fg != hwnd && fg != Gui.Window.hWnd) {
		return;
	}

	try {
		static std::chrono::time_point LastTimePoint = std::chrono::steady_clock::now();
		auto CurTimePoint = std::chrono::steady_clock::now();

		if (Keys::MenuKey
			&& CurTimePoint - LastTimePoint >= std::chrono::milliseconds(150))
		{
			MenuConfig::ShowMenu = !MenuConfig::ShowMenu;
			LastTimePoint = CurTimePoint;
		}

		if (MenuConfig::ShowMenu)
			Menu();

		static std::map<DWORD64, Render::HealthBar> HealthBarMap;

		Base_Radar Radar;

		if (MenuConfig::ShowRadar)
			RadarSetting(Radar);

		if (MenuConfig::ShowRadar)
			Radar.UpdateMap(mapname);

		for (int i = 0; i < EntityList.size(); i++)
		{
			CEntity Entity = EntityList[i];

			if (Entity.Pawn.Health <= 0) {
				continue;
			}

			if (MenuConfig::ShowRadar)
				Radar.AddPoint(LocalEntityPlayer.Pawn.Pos, Entity.Pawn.ViewAngle.y, Entity.Pawn.Pos, ImColor(237, 85, 106, 200), Entity.Pawn.ViewAngle.y, (int)(Entity.Controller.TeamID == LocalEntityPlayer.Controller.TeamID), Entity.Pawn.Health);

			if (MenuConfig::TeamCheck && Entity.Controller.TeamID == LocalEntityPlayer.Controller.TeamID)
				continue;

			if (!Entity.IsInScreen()) {
				continue;
			}

			if (MenuConfig::ShowBoneESP)
				Render::DrawBone(Entity, MenuConfig::BoneColor, 1.3);

			if (MenuConfig::ShowHeadESP)
			{
				if (Entity.GetBone().BonePosList.size() > BONEINDEX::head) {
					Vec2 HeadPos = Entity.GetBone().BonePosList[BONEINDEX::head].ScreenPos;
					Gui.CircleFilled(HeadPos, 4.0f, MenuConfig::HeadESPColor, 0);
				}
			}

			if (MenuConfig::ShowEyeRay)
				Render::ShowLosLine(Entity, 50, MenuConfig::EyeRayColor, 1.3);

			ImVec4 Rect;
			switch (MenuConfig::BoxType)
			{
			case 0:
				Rect = Render::Get2DBox(Entity);
				break;
			case 1:
				Rect = Render::Get2DBoneRect(Entity);
				break;
			default:
				break;
			}

			if (MenuConfig::ShowLineToEnemy)
				Render::LineToEnemy(Rect, MenuConfig::LineToEnemyColor, 1.2);

			if (MenuConfig::ShowBoxESP)
				Gui.Rectangle({ Rect.x,Rect.y }, { Rect.z,Rect.w }, MenuConfig::BoxColor, 1.3);

			if (MenuConfig::ShowHealthBar)
			{
				ImVec2 HealthBarPos, HealthBarSize;
				if (MenuConfig::HealthBarType == 0) // vertical
				{
					HealthBarPos = { Rect.x - 4.f,Rect.y };
					HealthBarSize = { 3 ,Rect.w };
				}
				else // horizontal
				{
					HealthBarPos = { Rect.x + Rect.z / 2 - 70 / 2,Rect.y - 13 };
					HealthBarSize = { 70, 4 };
				}
				Render::DrawHealthBar(Entity.Controller.Address, 100, Entity.Pawn.Health, HealthBarPos, HealthBarSize, MenuConfig::HealthBarType);
			}

			if (MenuConfig::ShowArmorBar)
			{
				ImVec2 ArmorBarPos, ArmorBarSize;
				if (MenuConfig::HealthBarType == 0) // vertical
				{
					ArmorBarPos = { Rect.x + Rect.z + 1.f, Rect.y };
					ArmorBarSize = { 3, Rect.w };
				}
				else // horizontal
				{
					ArmorBarPos = { Rect.x + Rect.z / 2 - 70 / 2, Rect.y - 10 };
					ArmorBarSize = { 70, 4 };
				}
				Render::DrawArmorBar(100.f, Entity.Controller.Armor, ArmorBarPos, ArmorBarSize, MenuConfig::HealthBarType);
			}

			if (MenuConfig::ShowWeaponESP)
				Gui.StrokeText(Entity.Pawn.WeaponName, { Rect.x + Rect.z / 2,Rect.y + Rect.w }, ImColor(255, 255, 255, 255), 14, true);

			if (MenuConfig::ShowDistance)
				Render::DrawDistance(LocalEntityPlayer, Entity, Rect);

			if (MenuConfig::ShowPlayerName)
			{
				if (MenuConfig::HealthBarType == 0)
					Gui.StrokeText(Entity.Controller.PlayerName, { Rect.x + Rect.z / 2,Rect.y - 14 }, ImColor(255, 255, 255, 255), 14, true);
				else
					Gui.StrokeText(Entity.Controller.PlayerName, { Rect.x + Rect.z / 2,Rect.y - 13 - 14 }, ImColor(255, 255, 255, 255), 14, true);
			}
		}

		if (MenuConfig::ShowRadar && LocalEntityPlayer.IsAlive())
			Radar.AddPoint(LocalEntityPlayer.Pawn.Pos, LocalEntityPlayer.Pawn.ViewAngle.y, LocalEntityPlayer.Pawn.Pos, ImColor(237, 85, 106, 200), LocalEntityPlayer.Pawn.ViewAngle.y, 2, LocalEntityPlayer.Pawn.Health);

		if (MenuConfig::ShowRadar)
		{
			Radar.Render();
			ImGui::End();
		}

		// Visuals only
	}
	catch (std::exception const& e)
	{
		//if (globalVars::debug) std::cout << e.what() << std::endl;

	}
}