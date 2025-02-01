#include "Config.h"

bool ConfigManager::CheckDirectory(const char* path)
{
    return std::filesystem::exists(path) && std::filesystem::is_directory(path);
}

std::vector<std::string> ConfigManager::GetFileList(const std::string& path)
{
    std::vector<std::string> iniFileList;



    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_regular_file())
        {
            auto path = entry.path();
            if (path.extension() == ".ini")
                iniFileList.push_back(path.filename().string());
        }
    }

    return iniFileList;
}

void ConfigManager::GenerateFile(const std::string& Name)
{
    if (Name.empty())
        return;

    // 拡張子をチェック
    std::string path = Name.find(".ini") == std::string::npos ? ".\\Config\\" + path += ".ini" : ".\\Config\\" + Name;

    // config.iniの存在をチェック
    mINI::INIFile file(path.c_str());
    std::filesystem::is_regular_file(path.c_str()) ? file.read(ini) : file.generate(ini, true);
}

void ConfigManager::LoadConfig(const std::string& Name)
{
    if (Name.empty())
        return;

    // 拡張子をチェック
    std::string path = Name.find(".ini") == std::string::npos ? ".\\Config\\" + path += ".ini" : ".\\Config\\" + Name;

    // config.iniの存在をチェック
    mINI::INIFile file(path.c_str());
    std::filesystem::is_regular_file(path.c_str()) ? file.read(ini) : file.generate(ini, true);

    // Visual
    std::istringstream(ini["visual"]["enable"]) >> g.g_ESP;
    std::istringstream(ini["visual"]["team"]) >> g.g_ESP_Team;
    std::istringstream(ini["visual"]["c4"]) >> g.g_ESP_C4;
    std::istringstream(ini["visual"]["weapon_esp"]) >> g.g_ESP_Weapon;

    std::istringstream(ini["visual"]["box"]) >> g.g_ESP_Box;
    std::istringstream(ini["visual"]["filled"]) >> g.g_ESP_BoxFilled;
    std::istringstream(ini["visual"]["line"]) >> g.g_ESP_Line;
    std::istringstream(ini["visual"]["skeleton"]) >> g.g_ESP_Skeleton;
    std::istringstream(ini["visual"]["healthbar"]) >> g.g_ESP_HealthBar;
    std::istringstream(ini["visual"]["name"]) >> g.g_ESP_Name;
    std::istringstream(ini["visual"]["distance"]) >> g.g_ESP_Distance;
    std::istringstream(ini["visual"]["weapon"]) >> g.g_ESP_CurrentWeapon;
    std::istringstream(ini["visual"]["box_type"]) >> g.g_ESP_BoxType;
    std::istringstream(ini["visual"]["distance_max"]) >> g.g_ESP_MaxDistance;

    // Crosshair
    std::istringstream(ini["crosshair"]["enable"]) >> g.g_Crosshair;
    std::istringstream(ini["crosshair"]["size"]) >> g.g_CrosshairSize;
    std::istringstream(ini["crosshair"]["type"]) >> g.g_CrosshairType;

    // System
    std::istringstream(ini["system"]["framerate"]) >> g.g_MaxFramerate;
}

void ConfigManager::SaveConfig(const std::string& Name)
{
    if (Name.empty())
        return;

    // 拡張子をチェック
    std::string path = Name.find(".ini") == std::string::npos ? ".\\Config\\" + path += ".ini" : ".\\Config\\" + Name;

    // config.iniの存在をチェック
    mINI::INIFile file(path.c_str());
    std::filesystem::is_regular_file(path.c_str()) ? file.read(ini) : file.generate(ini, true);

    // Visual
    ini["visual"]["enable"] = std::to_string(g.g_ESP).c_str();
    ini["visual"]["team"] = std::to_string(g.g_ESP_Team).c_str();
    ini["visual"]["c4"] = std::to_string(g.g_ESP_C4).c_str();
    ini["visual"]["weapon_esp"] = std::to_string(g.g_ESP_Weapon).c_str();

    ini["visual"]["box"] = std::to_string(g.g_ESP_Box).c_str();
    ini["visual"]["filled"] = std::to_string(g.g_ESP_BoxFilled).c_str();
    ini["visual"]["line"] = std::to_string(g.g_ESP_Line).c_str();
    ini["visual"]["skeleton"] = std::to_string(g.g_ESP_Skeleton).c_str();
    ini["visual"]["healthbar"] = std::to_string(g.g_ESP_HealthBar).c_str();
    ini["visual"]["name"] = std::to_string(g.g_ESP_Name).c_str();
    ini["visual"]["distance"] = std::to_string(g.g_ESP_Distance).c_str();
    ini["visual"]["weapon"] = std::to_string(g.g_ESP_CurrentWeapon).c_str();
    ini["visual"]["box_type"] = std::to_string(g.g_ESP_BoxType).c_str();
    ini["visual"]["distance_max"] = std::to_string(g.g_ESP_MaxDistance).c_str();

    // Crosshair
    ini["crosshair"]["enable"] = std::to_string(g.g_Crosshair).c_str();
    ini["crosshair"]["size"] = std::to_string(g.g_CrosshairSize).c_str();
    ini["crosshair"]["type"] = std::to_string(g.g_CrosshairType).c_str();

    // System
    ini["system"]["framerate"] = std::to_string(g.g_MaxFramerate).c_str();

    file.write(ini);
}

Globals g;