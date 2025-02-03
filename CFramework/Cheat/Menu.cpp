#include "FrameCore.h"

// Config
ConfigManager Config;
char ConfigPath[] = ".\\Config\\";

// ImGui::Combo/ImGui::List等で使う文字列群
const char* AimBoneList[] = { "Head", "Neck", "Chest", "Spine" };
const char* AimKeyModeList[] = { "None Key", "and", "or" };
const char* BoxTypeList[] = { "Simple", "Cornered" };
const char* CrosshairList[] = { "Cross", "Circle" };
std::vector<const char*> MenuStringList = { "AimBot", "Visual", "Misc", "Setting" };
std::vector<const char*> MenuIconList = { ICON_FA_CROSSHAIRS, ICON_FA_EYE, ICON_FA_BARS, ICON_FA_GEAR };

// チートのメニュー
void CFramework::RenderMenu()
{
    // Setup
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    
    // Aim & Config
    static int BindingID = 0;
    static int FileNum = 0;
    static char InputName[12];
    static bool DeleteFlag = false;

    //ImGui::SetNextWindowBgAlpha(0.975f);
    ImGui::SetNextWindowSize(ImVec2(725.f, 450.f));
    ImGui::Begin("Counter-Strike 2 [ EXTERNAL ]", &g.g_ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    //---// Clild 0 //-----------------------------------//
    ImGui::BeginChild("##SelectChild", ImVec2(150.f, ImGui::GetContentRegionAvail().y), false);

    static float size = ImGui::GetContentRegionAvail().x;

    ImGui::SetCursorPosY(25.f);

    ImGui::PushFont(icon);

    for (int i = 0; i < MenuIconList.size(); i++)
    {
        if (ImGui::CustomButton(MenuIconList[i], MenuStringList[i], ImVec2(ImGui::GetContentRegionAvail().x, 35.f), Index == i ? true : false))
            Index = i;
    }

    ImGui::PopFont();

    ImGui::EndChild();
    //---// Clild 0 End //-------------------------------//

    ImGui::SameLine();

    //---// Clild 1 //-----------------------------------//
    ImGui::BeginChild("##MainChild", ImVec2(ImGui::GetContentRegionAvail()), false);

    //---// Left //--------------------------------------//
    ImGui::BeginChild("##LeftChild", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - (style.WindowPadding.x * 2), ImGui::GetContentRegionAvail().y), false);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.f));

    switch (Index)
    {
    case 0:
        ImGui::BeginChild("##C000", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 3.f), true);
        ImGui::Text("Visual");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("AimBot", &g.g_AimBot);

        ImGui::EndChild();
        ImGui::BeginChild("##C001", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::Text("AimBot Setting");
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGui::CustomSliderInt("Aim FOV", "##a_fov", &g.g_AimFOV, 30, 300);
        ImGui::CustomSliderFloat("Smooth", "##a_smt", &g.g_AimSmooth, 1.f, 30.f);

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Combo("TargetBone", &g.g_AimBone, AimBoneList, IM_ARRAYSIZE(AimBoneList));

        ImGui::EndChild();
        break;
    case 1: // visual
        ImGui::BeginChild("##C010", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 3.f), true);
        ImGui::Text("Visual");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("ESP", &g.g_ESP);
        ImGui::Checkbox("Team ESP", &g.g_ESP_Team);

        ImGui::Checkbox("C4", &g.g_ESP_C4);
        ImGui::Checkbox("Weapon", &g.g_ESP_Weapon);
       
        ImGui::EndChild();
        ImGui::BeginChild("##C011", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::Text("ESP Options");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Box", &g.g_ESP_Box);
        ImGui::Checkbox("BoxFilled", &g.g_ESP_BoxFilled);
        ImGui::Checkbox("Line", &g.g_ESP_Line);
        ImGui::Checkbox("Skeleton", &g.g_ESP_Skeleton);
        ImGui::Checkbox("HealthBar", &g.g_ESP_HealthBar);
        ImGui::Checkbox("Name", &g.g_ESP_Name);
        ImGui::Checkbox("Distance", &g.g_ESP_Distance);
        ImGui::Checkbox("Weapon", &g.g_ESP_CurrentWeapon);

        ImGui::EndChild();
        break;
    case 2: { // misc
        ImGui::BeginChild("##C020", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 3.f), true);

        ImGui::EndChild();
    } break;
    case 3: // system
        ImGui::BeginChild("##C030", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 1.75f), true);

        ImGui::Text("System");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Checkbox("Crosshair", &g.g_Crosshair);

        ImGui::NewLine();

        ImGui::CustomSliderInt("MaxFramerate", "##MaxFrame", &g.g_MaxFramerate, 30, 500);

        ImGui::EndChild();
        ImGui::BeginChild("##C031", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::Text("Crosshair");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Crosshair", &g.g_Crosshair);
        ImGui::CustomSliderInt("CrosshairSize", "##SizeCH", &g.g_CrosshairSize, 1, 10);
        ImGui::ColorEdit4("Color##C", &CrosshairColor.Value.x);
        ImGui::Combo("Type##C", &g.g_CrosshairType, CrosshairList, IM_ARRAYSIZE(CrosshairList));

        ImGui::EndChild();
        break;
    default:
        break;
    }

    ImGui::PopStyleColor();

    ImGui::EndChild();
    //---------------------------------------------------//

    ImGui::SameLine();

    //---// Right //--------------------------------------//
    ImGui::BeginChild("##RightChild", ImVec2(ImGui::GetContentRegionAvail()), false);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 1.f));

    switch (Index)
    {
    case 0:
        ImGui::BeginChild("##C100", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 2.7f), true);

        ImGui::Text("FOV Setting");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Checkbox("Draw FOV", &g.g_ShowFOV);
        ImGui::Checkbox("Rainbow FOV", &g.g_ShowFOV);
        //ImGui::Checkbox("Rainbow FOV", &g.g_RainbowFOV);
        ImGui::ColorEdit3("Color", &AimFOV_Color.Value.x);
        ImGui::CustomSliderFloat("Smooth", "##a_smt", &g.g_AimSmooth, 1.f, 30.f);

        ImGui::EndChild();
        ImGui::BeginChild("##101", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::Text("KeyBind");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Combo("KeyMode", &g.g_AimKeyMode, AimKeyModeList, IM_ARRAYSIZE(AimKeyModeList));

        ImGui::NewLine();

        ImGui::Text("1st Key");
        if (ImGui::Button(BindingID == 1 ? "< Press Any Key >" : KeyNames[g.g_AimKey_0], ImVec2(215.f, 22.5f))) {
            BindingID = 1;
            std::thread([&]() { KeyBinder(g.g_AimKey_0, BindingID); }).detach();
        }

        ImGui::Spacing();

        ImGui::Text("2nd Key");
        if (ImGui::Button(BindingID == 2 ? "< Press Any Key >" : KeyNames[g.g_AimKey_1], ImVec2(215.f, 22.5f))) {
            BindingID = 2;
            std::thread([&]() { KeyBinder(g.g_AimKey_1, BindingID); }).detach();
        }

        if (g.g_AimKey_0 == g.g_AimKey_1)
            g.g_AimKey_1 = NULL;

        ImGui::EndChild();
        break;
    case 1: // visual
        ImGui::BeginChild("##C110", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 2.75f), true);

        ImGui::Text("ESP Setting");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::CustomSliderFloat("Distance", "##Distance", &g.g_ESP_MaxDistance, 10.f, 100.f);

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Combo("BoxType", &g.g_ESP_BoxType, BoxTypeList, IM_ARRAYSIZE(BoxTypeList));

        ImGui::EndChild();
        ImGui::BeginChild("##C111", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::Text("ESP Colors");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::ColorEdit3("Default", &ESP_Default.Value.x);
        ImGui::ColorEdit3("Visible", &ESP_Visible.Value.x);
        ImGui::ColorEdit3("Team", &ESP_Team.Value.x);
        ImGui::ColorEdit3("Shadow", &ESP_Shadow.Value.x, ImGuiColorEditFlags_DisplayRGB);

        ImGui::NewLine();

        ImGui::SeparatorText("Alpha");
        ImGui::CustomSliderFloat("Global", "##GlobalA", &m_flGlobalAlpha, 0.3f, 1.0f, "%.2f");
        ImGui::CustomSliderFloat("Shadow", "##ShadowA", &m_flShadowAlpha, 0.1f, 0.3f, "%.2f");

        ImGui::EndChild();
        break;
    case 2: // misc
        ImGui::BeginChild("##120", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::EndChild();
        break;
    case 3: // system
    {
        ImGui::BeginChild("##130", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y / 1.25f), true);

        ImGui::Text("Config");
        ImGui::Separator();
        ImGui::Spacing();

        auto vec = Config.GetFileList(ConfigPath);
        const char** FileList = new const char* [vec.size()];

        for (size_t j = 0; j < vec.size(); j++)
            FileList[j] = vec[j].c_str();

        ImGui::InputText("Name", InputName, IM_ARRAYSIZE(InputName));
        if (ImGui::Button("Generate ConfigFile", ImVec2(ImGui::GetContentRegionAvail().x, 20.f))) {
            std::thread([&]() {Config.GenerateFile(InputName); }).join();
            ZeroMemory(InputName, sizeof(InputName));
        }

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::ListBox("##List", &FileNum, FileList, vec.size());

        // Button
        if (ImGui::Button("Save", ImVec2(ImGui::GetContentRegionAvail().x / 3.f - 4.f, 20.f)))
            std::thread([&]() {Config.SaveConfig(InputName[0] != NULL ? InputName : FileList[FileNum]); }).join();
        ImGui::SameLine();
        if (ImGui::Button("Load", ImVec2(ImGui::GetContentRegionAvail().x / 2.f - 4.f, 20.f)) && vec.size() != 0)
            std::thread([&]() {Config.LoadConfig(FileList[FileNum]); }).join();
        ImGui::SameLine();
        if (ImGui::Button("Delete", ImVec2(ImGui::GetContentRegionAvail().x, 20.f)) && vec.size() != 0)
            DeleteFlag = true;

        if (DeleteFlag)
        {
            ImGui::Text("Delete this file?");

            if (ImGui::Button("OK", ImVec2(90.f, 20.f))) {
                std::thread([&]() {Config.DeleteConfig(FileList[FileNum]); }).join();
                DeleteFlag = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2(90.f, 20.f)))
                DeleteFlag = false;
        }

        delete[] FileList;

        ImGui::EndChild();
        ImGui::BeginChild("##131", ImVec2(ImGui::GetContentRegionAvail()), true);

        ImGui::Text("Exit");
        ImGui::Separator();
        ImGui::Spacing();
        if (ImGui::Button("Exit", ImVec2(ImGui::GetContentRegionAvail().x, 30.f)))
            g.process_active = false;

        ImGui::EndChild();
    }   break;
    default:
        break;
    }

    ImGui::PopStyleColor();

    ImGui::EndChild();
    ImGui::EndChild();
    //---------------------------------------------------//

    ImGui::End();
}