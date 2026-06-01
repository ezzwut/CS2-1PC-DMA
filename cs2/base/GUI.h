#pragma once

#include "../Language.h"
#include "../Globals.h"

#include "Cheats.h"
#include "Render.hpp"
#include "MenuConfig.hpp"
#include "../utils/ConfigMenu.hpp"
#include "../utils/ConfigSaver.hpp"


void setStyles() {

	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	if (!MenuConfig::DarkMode) {
		// Light Mode (Red/White)
		style->Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.80f, 0.50f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.00f, 0.00f, 1.00f);
	} else {
		// Dark Mode (Blue/Black)
		style->Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.50f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.50f, 1.00f, 1.00f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.40f, 0.80f, 1.00f);
	}

	if (MenuConfig::MaxFrameRate) Gui.delayticks = (1000000 / MenuConfig::MaxFrameRate) - 5000;
	else Gui.delayticks = 0;
}

void Cheats::Menu()
{
	static bool IsMenuInit = false;

	if (!IsMenuInit)
	{
		setStyles();
		IsMenuInit = true;
	}

	ImGui::GetIO().Fonts->Fonts[0];

	ImGui::Begin("CS 2 / DMA", nullptr, ImGuiWindowFlags_NoDecoration);
	ImGui::SetWindowSize({ globalVars::windowx, globalVars::windowy });
	{
		ImGui::BeginChild("Container##1", ImVec2((globalVars::windowx) / 10 * 2.3f, globalVars::windowy - 30.f), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		{
			static float subblockw = 2.0;

			static bool oldDarkMode = MenuConfig::DarkMode;
			if (oldDarkMode != MenuConfig::DarkMode) {
				setStyles();
				oldDarkMode = MenuConfig::DarkMode;
			}

			ImGui::BeginChild("Logo", ImVec2((globalVars::windowx) / 10 * subblockw, 50), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			{
				ImGui::SetWindowFontScale(0.9f);
				ImColor textColor = MenuConfig::DarkMode ? ImColor(255, 255, 255) : ImColor(0, 0, 0);
				ImGui::GetWindowDrawList()->AddText(ImVec2(ImGui::GetWindowPos().x + 5, ImGui::GetWindowPos().y + 15), textColor, "Premium CS2 Cheat");
				ImGui::SetWindowFontScale(1.0f);
			}
			ImGui::EndChild();

			ImGui::Separator();

			ImGui::BeginChild("ButtonBlock", ImVec2((globalVars::windowx) / 10 * subblockw, 240), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			{
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
				
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				ImGui::Checkbox("Dark Mode", &MenuConfig::DarkMode);

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				ImGui::Text("Language:");
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 10);
				if (ImGui::Combo("##Lang", &MenuConfig::SelectedLanguage, lang.utilities_langselect, IM_ARRAYSIZE(lang.utilities_langselect)))
				{
					switch (MenuConfig::SelectedLanguage)
					{
					case 0:
						lang.english();
						break;
					case 1:
						lang.german();
						break;
					case 2:
						lang.turkish();
						break;
					default:
						break;
					}
				}

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				if (ImGui::Button("Refresh DMA", ImVec2(ImGui::GetContentRegionAvail().x - 10, 25))) {
					ProcessMgr.RefindGame();
					Cheats::EntityList.clear();
					gGame.InitAddress();
				}

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
				if (ImGui::Button("Hide Terminal", ImVec2(ImGui::GetContentRegionAvail().x - 10, 25))) {
					ShowWindow(GetConsoleWindow(), SW_HIDE);
				}
			}
			ImGui::EndChild();

		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Container##2", ImVec2((globalVars::windowx) / 10 * 7.2f, globalVars::windowy - 30.f), true);
		{
			// Visuals Only
			Gui.MyCheckBox(lang.utilities_teamcheck.c_str(), &MenuConfig::TeamCheck);
			ImGui::Separator();
			
			Gui.MyCheckBox(lang.visuals_showbox.c_str(), &MenuConfig::ShowBoxESP);
			if (MenuConfig::ShowBoxESP) {
				ImGui::SameLine();
				ImGui::ColorEdit4(lang.visuals_boxcolor.c_str(), reinterpret_cast<float*>(&MenuConfig::BoxColor), ImGuiColorEditFlags_NoInputs);
				ImGui::Combo(lang.visuals_boxtype.c_str(), &MenuConfig::BoxType, lang.visuals_boxtypeselect, IM_ARRAYSIZE(lang.visuals_boxtypeselect));
			}
			ImGui::Separator();
			Gui.MyCheckBox(lang.visuals_showbone.c_str(), &MenuConfig::ShowBoneESP);
			if (MenuConfig::ShowBoneESP) {
				ImGui::SameLine();
				ImGui::ColorEdit4(lang.visuals_bonecolor.c_str(), reinterpret_cast<float*>(&MenuConfig::BoneColor), ImGuiColorEditFlags_NoInputs);
				Gui.MyCheckBox("Head ESP", &MenuConfig::ShowHeadESP);
				if (MenuConfig::ShowHeadESP) {
					ImGui::SameLine();
					ImGui::ColorEdit4("Head ESP Color", reinterpret_cast<float*>(&MenuConfig::HeadESPColor), ImGuiColorEditFlags_NoInputs);
				}
				Gui.MyCheckBox(lang.visuals_showeyeray.c_str(), &MenuConfig::ShowEyeRay);
				if (MenuConfig::ShowEyeRay) {
					ImGui::SameLine();
					ImGui::ColorEdit4(lang.visuals_eyeraycolor.c_str(), reinterpret_cast<float*>(&MenuConfig::EyeRayColor), ImGuiColorEditFlags_NoInputs);
				}
			}
			ImGui::Separator();
			Gui.MyCheckBox(lang.visuals_showbar.c_str(), &MenuConfig::ShowHealthBar);
			if (MenuConfig::ShowHealthBar) {
				ImGui::SameLine();
				ImGui::Combo(lang.visuals_barpos.c_str(), &MenuConfig::HealthBarType, lang.visuals_heathbarselect, IM_ARRAYSIZE(lang.visuals_heathbarselect));
			}
			
			Gui.MyCheckBox("Armor Bar", &MenuConfig::ShowArmorBar);
			ImGui::Separator();

			Gui.MyCheckBox(lang.visuals_weaponesp.c_str(), &MenuConfig::ShowWeaponESP);
			ImGui::SameLine();
			Gui.MyCheckBox(lang.visuals_distance.c_str(), &MenuConfig::ShowDistance);
			ImGui::SameLine();
			Gui.MyCheckBox(lang.visuals_name.c_str(), &MenuConfig::ShowPlayerName);
			ImGui::Separator();

			Gui.MyCheckBox(lang.visuals_line.c_str(), &MenuConfig::ShowLineToEnemy);
			if (MenuConfig::ShowLineToEnemy) {
				ImGui::SameLine();
				ImGui::ColorEdit4(lang.visuals_linecolor.c_str(), reinterpret_cast<float*>(&MenuConfig::LineToEnemyColor), ImGuiColorEditFlags_NoInputs);
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}
