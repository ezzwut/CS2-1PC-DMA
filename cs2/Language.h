#pragma once
#include <string>

class Language {
public:
	std::string tab_visuals;
	std::string tab_aimbot;
	std::string tab_radar;
	std::string tab_trigger;
	std::string tab_settings;
	std::string tab_config;

	std::string visuals_showbox;
	std::string visuals_boxcolor;
	std::string visuals_boxtype;
	std::string visuals_showbone;
	std::string visuals_bonecolor;
	std::string visuals_showeyeray;
	std::string visuals_eyeraycolor;
	std::string visuals_showbar;
	std::string visuals_barpos;
	std::string visuals_weaponesp;
	std::string visuals_distance;
	std::string visuals_name;
	std::string visuals_line;
	std::string visuals_linecolor;

	std::string aimbot_enable;
	std::string aimbot_hotkey;
	std::string aimbot_fov;
	std::string aimbot_showaimfov;
	std::string aimbot_aimfovcolor;
	std::string aimbot_smooth;

	std::string aimbot_posrifles;
	std::string aimbot_pospis;
	std::string aimbot_possn;
	std::string aimbot_posshotguns;
	std::string aimbot_posdef;
	std::string aimbot_recolcontrol;

	std::string aimbot_autoshot;
	std::string aimbot_visiblecheck;
	std::string aimbot_ignoreonshot;
	std::string aimbot_customweapon;
	std::string aimbot_customsmooth;

	std::string aimbot_pistols;
	std::string aimbot_rifles;
	std::string aimbot_snipers;
	std::string aimbot_shotguns;
	std::string aimbot_other;

	std::string radar_showradar;
	std::string radar_enableweb;
	std::string radar_radarsize;
	std::string radar_euserver;

	std::string trigger_enable;
	std::string trigger_hotkey;
	std::string trigger_mode;
	std::string trigger_delay;

	std::string utilities_teamcheck;
	std::string utilities_closehack;
	std::string utilities_reloadhack;
	std::string utilities_language;

	std::string config_newconfig;
	std::string config_create;
	std::string config_load;
	std::string config_save;
	std::string config_delete;

	const char* visuals_boxtypeselect[2];
	const char* visuals_heathbarselect[2];

	const char* aimbot_hotkeyselect[8] = { "LBUTTON", "MENU", "RBUTTON", "XBUTTON1", "XBUTTON2", "CAPITAL", "SHIFT", "CONTROL" };
	const char* aimbot_posselect[3];

	const char* radar_sizeselect[2];

	const char* trigger_hotkeyselect[7] = { "MENU", "RBUTTON", "XBUTTON1", "XBUTTON2", "CAPITAL", "SHIFT", "CONTROL" };
	const char* trigger_onkeyselect[2];

	const char* utilities_langselect[3] = { "English", "Deutsch", "Turkce" };

	std::string days;

	std::string frames;


	void english() {
		this->tab_visuals = "Visuals";
		this->tab_aimbot = "Aimbot";
		this->tab_radar = "Radar";
		this->tab_trigger = "TriggerBot";
		this->tab_settings = "Settings";
		this->tab_config = "Config";

		this->visuals_showbox = "Show Box";
		this->visuals_boxcolor = "Box Color";
		this->visuals_boxtype = "Box Type";
		this->visuals_barpos = "Health Bar Position";
		this->visuals_showbone = "Bones";
		this->visuals_bonecolor = "Bones Color";
		this->visuals_showeyeray = "Eye Ray";
		this->visuals_eyeraycolor = "Eye Ray Color";
		this->visuals_showbar = "Health Bar";
		this->visuals_weaponesp = "Weapon";
		this->visuals_distance = "Distance";
		this->visuals_name = "Name";
		this->visuals_line = "Lines to Player";
		this->visuals_linecolor = "Lines color";

		this->aimbot_enable = "Enable";
		this->aimbot_hotkey = "Aimbot Hotkey";
		this->aimbot_fov = "fov";
		this->aimbot_showaimfov = "Show AimFov";
		this->aimbot_aimfovcolor = "Aimfov Color";
		this->aimbot_smooth = "Smooth";
		this->aimbot_recolcontrol = "Recoil Control";

		this->aimbot_posdef = "Aim Target / Default";
		this->aimbot_pospis = "Aim Target / Pistols";
		this->aimbot_possn = "Aim Target / Snipers";
		this->aimbot_posrifles = "Aim Target / Rifles";
		this->aimbot_posshotguns = "Aim Target / Shotguns";

		this->aimbot_pistols = "Pistols";
		this->aimbot_rifles = "Rifles";
		this->aimbot_snipers = "Snipers";
		this->aimbot_shotguns = "Shotguns";
		this->aimbot_other = "Other";

		this->aimbot_autoshot = "AutoShot";
		this->aimbot_visiblecheck = "Visible Check";
		this->aimbot_ignoreonshot = "IgnoreOnShot";
		this->aimbot_customweapon = "Custom Settings";
		this->aimbot_customsmooth = "Gun Smooth";

		this->radar_showradar = "Show Radar";
		this->radar_enableweb = "Enable Web Radar";
		this->radar_radarsize = "Radar Size";
		this->radar_euserver = "Web / EU Server";

		this->trigger_enable = "Enable";
		this->trigger_hotkey = "Trigger Hotkey";
		this->trigger_mode = "Trigger Mode";
		this->trigger_delay = "Delay";

		this->utilities_teamcheck = "Team Check";
		this->utilities_closehack = "Close Software";
		this->utilities_reloadhack = "Reload game";
		this->utilities_language = "Select Language";

		this->config_newconfig = "Config Name";
		this->config_create = "Create config";
		this->config_load = "Load";
		this->config_save = "Save";
		this->config_delete = "Delete";

		this->visuals_boxtypeselect[0] = "Normal"; this->visuals_boxtypeselect[1] = "Slim";
		this->visuals_heathbarselect[0] = "Left"; this->visuals_heathbarselect[1] = "Up";

		this->aimbot_posselect[0] = "Head"; this->aimbot_posselect[1] = "Neck"; this->aimbot_posselect[2] = "Spine";
		this->radar_sizeselect[0] = "Small"; this->radar_sizeselect[1] = "Big";

		this->trigger_onkeyselect[0] = "OnKey"; this->trigger_onkeyselect[1] = "Always";

		this->days = "days";

		this->frames = "Frames";
	}

	void german() {
		this->tab_visuals = "Visuals";
		this->tab_aimbot = "Aimbot";
		this->tab_radar = "Radar";
		this->tab_trigger = "TriggerBot";
		this->tab_settings = "Einstellungen";
		this->tab_config = "Config";

		this->visuals_showbox = "Box Anzeigen";
		this->visuals_boxcolor = "Box Farbe";
		this->visuals_boxtype = "Box Typ";
		this->visuals_barpos = "Lebensbalken Position";
		this->visuals_showbone = "Knochen";
		this->visuals_bonecolor = "Knochen Farbe";
		this->visuals_showeyeray = "Blickrichtung";
		this->visuals_eyeraycolor = "Blickrichtung Farbe";
		this->visuals_showbar = "Lebensbalken";
		this->visuals_weaponesp = "Waffe";
		this->visuals_distance = "Distanz";
		this->visuals_name = "Name";
		this->visuals_line = "Linie zum Spieler";
		this->visuals_linecolor = "Linie Farbe";

		this->aimbot_enable = "Aktivieren";
		this->aimbot_hotkey = "Aimbot Taste";
		this->aimbot_fov = "Sichtfeld";
		this->aimbot_showaimfov = "Zeige Sichtfeld";
		this->aimbot_aimfovcolor = "Sichtfeld Farbe";
		this->aimbot_smooth = "Glatt";
		this->aimbot_recolcontrol = "Ruckstosskontrolle";

		this->aimbot_posdef = "Ziel Position / Standard";
		this->aimbot_pospis = "Ziel Position / Pistolen";
		this->aimbot_possn = "Ziel Position / Scharfschutzen";
		this->aimbot_posrifles = "Ziel Position / Sturmgewehre";
		this->aimbot_posshotguns = "Ziel Position / Schrotflinten";

		this->aimbot_pistols = "Pistolen";
		this->aimbot_rifles = "Sturmgewehre";
		this->aimbot_snipers = "Scharfschutzen";
		this->aimbot_shotguns = "Schrotflinten";
		this->aimbot_other = "Andere";

		this->aimbot_autoshot = "Auto-Schuss";
		this->aimbot_visiblecheck = "Sichtbarkeitsprufung";
		this->aimbot_ignoreonshot = "Ignorieren bei Schuss";
		this->aimbot_customweapon = "Eigene Waffen Einstellungen";
		this->aimbot_customsmooth = "Waffen Glatt";

		this->radar_showradar = "Radar Anzeigen";
		this->radar_enableweb = "Web-Radar Aktivieren";
		this->radar_radarsize = "Radar Grosse";
		this->radar_euserver = "Web / EU Server";

		this->trigger_enable = "Aktivieren";
		this->trigger_hotkey = "Trigger Taste";
		this->trigger_mode = "Trigger Modus";
		this->trigger_delay = "Verzogerung";

		this->utilities_teamcheck = "Team Uberprufen";
		this->utilities_closehack = "Software Schliessen";
		this->utilities_reloadhack = "Spiel neu laden";
		this->utilities_language = "Sprache Auswahlen";

		this->config_newconfig = "Config Name";
		this->config_create = "Config Erstellen";
		this->config_load = "Laden";
		this->config_save = "Speichern";
		this->config_delete = "Loschen";

		this->visuals_boxtypeselect[0] = "Normal"; this->visuals_boxtypeselect[1] = "Dunn";
		this->visuals_heathbarselect[0] = "Links"; this->visuals_heathbarselect[1] = "Oben";

		this->aimbot_posselect[0] = "Kopf"; this->aimbot_posselect[1] = "Hals"; this->aimbot_posselect[2] = "Bauch";
		this->radar_sizeselect[0] = "Klein"; this->radar_sizeselect[1] = "Gross";

		this->trigger_onkeyselect[0] = "Auf Taste"; this->trigger_onkeyselect[1] = "Immer";

		this->days = "Tage";

		this->frames = "Bilder";
	}

	void turkish() {
		this->tab_visuals = "Gorseller";
		this->tab_aimbot = "Aimbot";
		this->tab_radar = "Radar";
		this->tab_trigger = "Tetikleyici";
		this->tab_settings = "Ayarlar";
		this->tab_config = "Yapilandirma";

		this->visuals_showbox = "Kutu Goster";
		this->visuals_boxcolor = "Kutu Rengi";
		this->visuals_boxtype = "Kutu Tipi";
		this->visuals_barpos = "Can Cubugu Pozisyonu";
		this->visuals_showbone = "Kemikler";
		this->visuals_bonecolor = "Kemik Rengi";
		this->visuals_showeyeray = "Bakis Yonu";
		this->visuals_eyeraycolor = "Bakis Yonu Rengi";
		this->visuals_showbar = "Can Cubugu";
		this->visuals_weaponesp = "Silah";
		this->visuals_distance = "Mesafe";
		this->visuals_name = "Isim";
		this->visuals_line = "Oyuncuya Cizgi";
		this->visuals_linecolor = "Cizgi Rengi";

		this->aimbot_enable = "Etkinlestir";
		this->aimbot_hotkey = "Aimbot Tusu";
		this->aimbot_fov = "Gorus Alani";
		this->aimbot_showaimfov = "Gorus Alanini Goster";
		this->aimbot_aimfovcolor = "Gorus Alani Rengi";
		this->aimbot_smooth = "Yumusatma";
		this->aimbot_recolcontrol = "Tepme Kontrolu";

		this->aimbot_posdef = "Nisan Noktasi / Varsayilan";
		this->aimbot_pospis = "Nisan Noktasi / Tabancalar";
		this->aimbot_possn = "Nisan Noktasi / Keskin Nisancilar";
		this->aimbot_posrifles = "Nisan Noktasi / Tufekler";
		this->aimbot_posshotguns = "Nisan Noktasi / Pompali";

		this->aimbot_pistols = "Tabancalar";
		this->aimbot_rifles = "Tufekler";
		this->aimbot_snipers = "Keskin Nisancilar";
		this->aimbot_shotguns = "Pompali Tufekler";
		this->aimbot_other = "Diger";

		this->aimbot_autoshot = "Otomatik Atis";
		this->aimbot_visiblecheck = "Gorunurluk Kontrolu";
		this->aimbot_ignoreonshot = "Atis Sirasinda Yoksay";
		this->aimbot_customweapon = "Ozel Silah Ayarlari";
		this->aimbot_customsmooth = "Silah Yumusatma";

		this->radar_showradar = "Radar Goster";
		this->radar_enableweb = "Web Radarini Etkinlestir";
		this->radar_radarsize = "Radar Boyutu";
		this->radar_euserver = "Web / EU Sunucu";

		this->trigger_enable = "Etkinlestir";
		this->trigger_hotkey = "Tetikleyici Tus";
		this->trigger_mode = "Tetikleyici Modu";
		this->trigger_delay = "Gecikme";

		this->utilities_teamcheck = "Takim Kontrolu";
		this->utilities_closehack = "Yazilimi Kapat";
		this->utilities_reloadhack = "Oyunu Yenile";
		this->utilities_language = "Dil Secimi";

		this->config_newconfig = "Yapilandirma Ismi";
		this->config_create = "Yapilandirma Olustur";
		this->config_load = "Yukle";
		this->config_save = "Kaydet";
		this->config_delete = "Sil";

		this->visuals_boxtypeselect[0] = "Normal"; this->visuals_boxtypeselect[1] = "Ince";
		this->visuals_heathbarselect[0] = "Sol"; this->visuals_heathbarselect[1] = "Ust";

		this->aimbot_posselect[0] = "Kafa"; this->aimbot_posselect[1] = "Boyun"; this->aimbot_posselect[2] = "Gogus";
		this->radar_sizeselect[0] = "Kucuk"; this->radar_sizeselect[1] = "Buyuk";

		this->trigger_onkeyselect[0] = "Tusa Basilinca"; this->trigger_onkeyselect[1] = "Her Zaman";

		this->days = "Gunler";

		this->frames = "Kareler";
	}
};

inline Language lang;