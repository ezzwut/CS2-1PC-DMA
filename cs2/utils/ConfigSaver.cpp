#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "ConfigSaver.hpp"
#include "../base/MenuConfig.hpp" 
#include "../radar/mapsdata.h"

namespace MyConfigSaver {

    void SaveConfig(const std::string& filename) {
        std::ofstream configFile(MenuConfig::path+'/'+filename);
        if (!configFile.is_open()) {
            std::cerr << "Error: Could not open the configuration file." << std::endl;
            return;
        }
       
        configFile << "ShowBoneESP " << MenuConfig::ShowBoneESP << std::endl;
        configFile << "ShowBoxESP " << MenuConfig::ShowBoxESP << std::endl;
        configFile << "ShowHealthBar " << MenuConfig::ShowHealthBar << std::endl;
        configFile << "ShowArmorBar " << MenuConfig::ShowArmorBar << std::endl;
        configFile << "TeamCheck " << MenuConfig::TeamCheck << std::endl;
        configFile << "BypassOBS " << MenuConfig::BypassOBS << std::endl;
        configFile << "ShowLineToEnemy " << MenuConfig::ShowLineToEnemy << std::endl;
        configFile << "ShowWeaponESP " << MenuConfig::ShowWeaponESP << std::endl;
        configFile << "ShowDistance " << MenuConfig::ShowDistance << std::endl;
        configFile << "ShowEyeRay " << MenuConfig::ShowEyeRay << std::endl;
        configFile << "ShowPlayerName " << MenuConfig::ShowPlayerName << std::endl;
        configFile << "HealthBarType " << MenuConfig::HealthBarType << std::endl;
        configFile << "BoneColor " << MenuConfig::BoneColor.Value.x << " " << MenuConfig::BoneColor.Value.y << " " << MenuConfig::BoneColor.Value.z << " " << MenuConfig::BoneColor.Value.w << std::endl;
        configFile << "LineToEnemyColor " << MenuConfig::LineToEnemyColor.Value.x << " " << MenuConfig::LineToEnemyColor.Value.y << " " << MenuConfig::LineToEnemyColor.Value.z << " " << MenuConfig::LineToEnemyColor.Value.w << std::endl;
        configFile << "BoxColor " << MenuConfig::BoxColor.Value.x << " " << MenuConfig::BoxColor.Value.y << " " << MenuConfig::BoxColor.Value.z << " " << MenuConfig::BoxColor.Value.w << std::endl;
        configFile << "EyeRayColor " << MenuConfig::EyeRayColor.Value.x << " " << MenuConfig::EyeRayColor.Value.y << " " << MenuConfig::EyeRayColor.Value.z << " " << MenuConfig::EyeRayColor.Value.w << std::endl;
        configFile << "ShowMenu " << MenuConfig::ShowMenu << std::endl;
        configFile << "ShowRadar " << MenuConfig::ShowRadar << std::endl;
        configFile << "RadarType " << MenuConfig::RadarType << std::endl;
        configFile << "RadarSize " << mp::RadarSize << std::endl;
        configFile << "RadarLineLenght " << mp::LineLenght << std::endl;
        configFile << "RadarCircleSize " << mp::CircleSize << std::endl;
        configFile << "BoxType " << MenuConfig::BoxType << std::endl;
        configFile << "ShowMenu " << MenuConfig::ShowMenu << std::endl;
        configFile << "TeamCheck " << MenuConfig::TeamCheck << std::endl;
		configFile << "VisibleCheck " << MenuConfig::VisibleCheck << std::endl;
        configFile << "Frames " << MenuConfig::MaxFrameRate << std::endl;

        configFile.close();
    }

    void LoadConfig(const std::string& filename) {
        std::string tempkey;

        std::ifstream configFile(MenuConfig::path + '/' + filename);
        if (!configFile.is_open()) {
            return;
        }

        std::string line;
        while (std::getline(configFile, line)) {
            std::istringstream iss(line);
            std::string key;
            if (iss >> key) {
                if (key == "ShowBoneESP") iss >> MenuConfig::ShowBoneESP;
                else if (key == "ShowBoxESP") iss >> MenuConfig::ShowBoxESP;
                else if (key == "ShowHealthBar") iss >> MenuConfig::ShowHealthBar;
                else if (key == "ShowArmorBar") iss >> MenuConfig::ShowArmorBar;
                else if (key == "ShowLineToEnemy") iss >> MenuConfig::ShowLineToEnemy;
                else if (key == "ShowWeaponESP") iss >> MenuConfig::ShowWeaponESP;
                else if (key == "ShowDistance") iss >> MenuConfig::ShowDistance;
                else if (key == "ShowEyeRay") iss >> MenuConfig::ShowEyeRay;
                else if (key == "ShowPlayerName") iss >> MenuConfig::ShowPlayerName;
                else if (key == "HealthBarType") iss >> MenuConfig::HealthBarType;
                else if (key == "BoneColor") iss >> MenuConfig::BoneColor.Value.x >> MenuConfig::BoneColor.Value.y >> MenuConfig::BoneColor.Value.z >> MenuConfig::BoneColor.Value.w;
                else if (key == "LineToEnemyColor") iss >> MenuConfig::LineToEnemyColor.Value.x >> MenuConfig::LineToEnemyColor.Value.y >> MenuConfig::LineToEnemyColor.Value.z >> MenuConfig::LineToEnemyColor.Value.w;
                else if (key == "BoxColor") iss >> MenuConfig::BoxColor.Value.x >> MenuConfig::BoxColor.Value.y >> MenuConfig::BoxColor.Value.z >> MenuConfig::BoxColor.Value.w;
                else if (key == "EyeRayColor") iss >> MenuConfig::EyeRayColor.Value.x >> MenuConfig::EyeRayColor.Value.y >> MenuConfig::EyeRayColor.Value.z >> MenuConfig::EyeRayColor.Value.w;
                else if (key == "ShowMenu") iss >> MenuConfig::ShowMenu;
                else if (key == "ShowRadar") iss >> MenuConfig::ShowRadar;
                else if (key == "RadarType") iss >> MenuConfig::RadarType;
                else if (key == "RadarSize") iss >> mp::RadarSize;
                else if (key == "RadarLineLenght") iss >> mp::LineLenght;
                else if (key == "RadarCircleSize") iss >> mp::CircleSize;
                else if (key == "BoxType") iss >> MenuConfig::BoxType;
                else if (key == "TeamCheck") iss >> MenuConfig::TeamCheck;
                else if (key == "VisibleCheck") iss >> MenuConfig::VisibleCheck;
                else if (key == "Frames") iss >> MenuConfig::MaxFrameRate;
            }
        }

        configFile.close();
    }
}
