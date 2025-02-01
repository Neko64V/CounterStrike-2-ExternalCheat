#include "FrameCore.h"

void CFramework::RenderInfo()
{
    // Update colors
    ESP_Default.Value.w = m_flGlobalAlpha;
    ESP_Visible.Value.w = m_flGlobalAlpha;
    ESP_Team.Value.w = m_flGlobalAlpha;
    TEXT_COLOR.Value.w = m_flGlobalAlpha;

    ImGui::SetNextWindowPos(ImVec2(g.g_GamePoint.x, g.g_GamePoint.y));
    ImGui::SetNextWindowSize(ImVec2(g.g_GameRect.right, g.g_GameRect.bottom));
    ImGui::Begin("##Info", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

    // FPS
    String(Vector2(), TEXT_COLOR, std::to_string((int)ImGui::GetIO().Framerate).c_str());

    // FOV Circle
    if (g.g_AimBot && g.g_ShowFOV)
        Circle(Vector2((g.g_GameRect.right / 2.f), (g.g_GameRect.bottom / 2.f)), g.g_AimFOV, ImColor(AimFOV_Color));

    // Crosshair
    if (g.g_Crosshair)
    {
        switch (g.g_CrosshairType)
        {
        case 0: {
            ImVec2 Center = ImVec2(g.g_GameRect.right / 2, g.g_GameRect.bottom / 2);
            DrawLine(Vector2(Center.x - g.g_CrosshairSize, Center.y), Vector2((Center.x + g.g_CrosshairSize) + 1, Center.y), CrosshairColor, 1);
            DrawLine(Vector2(Center.x, Center.y - g.g_CrosshairSize), Vector2(Center.x, (Center.y + g.g_CrosshairSize) + 1), CrosshairColor, 1);
        }   break;
        case 1:
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.g_GameRect.right / 2.f, (float)g.g_GameRect.bottom / 2.f), g.g_CrosshairSize + 1, ImColor(0.f, 0.f, 0.f, 1.f), NULL);
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.g_GameRect.right / 2.f, (float)g.g_GameRect.bottom / 2.f), g.g_CrosshairSize, CrosshairColor, NULL);
            break;
        }
    }

    ImGui::End();
}

void CFramework::RenderESP()
{
    CEntity* pLocal = &local;

    // AimBot
    float FOV = 0.f;
    float MinFov = FLT_MAX;
    float MinDistance = FLT_MAX;
    CEntity target = CEntity();
    Vector2 ScreenMiddle = { g.g_GameRect.right / 2.f, g.g_GameRect.bottom / 2.f };

    // Localの更新に失敗したらこの関数を終了
    if (!pLocal->Update())
        return;

    // ViewMatrixとかいろいろ
    Matrix ViewMatrix = m.Read<Matrix>(m.m_gClientBaseAddr + offset::dwViewMatrix);

    ImGui::SetNextWindowPos(ImVec2(g.g_GamePoint.x, g.g_GamePoint.y));
    ImGui::SetNextWindowSize(ImVec2(g.g_GameRect.right, g.g_GameRect.bottom));
    ImGui::Begin("##Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
    
    // C4 dev
    uintptr_t c4_ptr = m.Read<uintptr_t>(m.m_gClientBaseAddr + offset::dwPlantedC4);

    if (c4_ptr != NULL) {

        C4->address = m.Read<uintptr_t>(c4_ptr);
        Vector3 GamePositon = C4->GetAbsOrigin();

        if (!Vec3_Empty(GamePositon)) {
            Vector2 pos{};
            WorldToScreen(ViewMatrix, g.g_GameRect, GamePositon, pos);

            std::string vout = "C4";// + std::to_string((int)C4->GetTimer()) + "sec";
            String(Vector2(pos.x - (ImGui::CalcTextSize(vout.c_str()).x / 2.f), pos.y - ImGui::GetFontSize() - 1.f), ImColor(0.f, 1.f, 0.f, 1.f), vout.c_str());
            Circle(pos, 2.f, ImColor(0.f, 1.f, 0.f, 1.f));
        }
    }

    // るーぷするよ
    for (auto& entity : EntityList)
    {
        CEntity* pEntity = &entity;

        if (!pEntity->Update())
            continue;

        std::vector<Vector3> BoneList;

        if (g.g_AimBot || g.g_ESP_Skeleton)
            BoneList = pEntity->GetBoneList();

        // 距離を取得
        const float flDistance = ((pLocal->m_vOldOrigin - pEntity->m_vOldOrigin).Length() * 0.01905f);

        // 各種チェック
        if (g.g_ESP_MaxDistance < flDistance)
            continue;
        else if (!g.g_ESP_Team && pEntity->m_iTeamNum == pLocal->m_iTeamNum)
            continue;

        Vector3 min = pEntity->vecMin();
        Vector3 max = pEntity->vecMax();

        int left, top, right, bottom;
        Vector2 flb, brt, blb, frt, frb, brb, blt, flt;

        Vector3 points[8] = { Vector3(min.x, min.y, min.z), Vector3(min.x, max.y, min.z), Vector3(max.x, max.y, min.z),
                    Vector3(max.x, min.y, min.z), Vector3(max.x, max.y, max.z), Vector3(min.x, max.y, max.z),
                    Vector3(min.x, min.y, max.z), Vector3(max.x, min.y, max.z) };

        if (!WorldToScreen(ViewMatrix, g.g_GameRect, points[3], flb) || !WorldToScreen(ViewMatrix, g.g_GameRect, points[5], brt) ||
            !WorldToScreen(ViewMatrix, g.g_GameRect, points[0], blb) || !WorldToScreen(ViewMatrix, g.g_GameRect, points[4], frt) ||
            !WorldToScreen(ViewMatrix, g.g_GameRect, points[2], frb) || !WorldToScreen(ViewMatrix, g.g_GameRect, points[1], brb) ||
            !WorldToScreen(ViewMatrix, g.g_GameRect, points[6], blt) || !WorldToScreen(ViewMatrix, g.g_GameRect, points[7], flt))
            continue;

        Vector2 vec2_array[] = { flb, brt, blb, frt, frb, brb, blt, flt };
        left = flb.x;
        top = flb.y;
        right = flb.x;
        bottom = flb.y;

        for (auto j = 1; j < 8; ++j)
        {
            if (left > vec2_array[j].x)
                left = vec2_array[j].x;
            if (bottom < vec2_array[j].y)
                bottom = vec2_array[j].y;
            if (right < vec2_array[j].x)
                right = vec2_array[j].x;
            if (top > vec2_array[j].y)
                top = vec2_array[j].y;
        }

        // サイズ算出
        const int Height = bottom - top;
        const int Width = right - left;
        const int Center = (right - left) / 2.f;
        const int bScale = (right - left) / 3.f;

        // 対象が見えてるかチェックする。
        bool visible = false;

        // 色を決める
        ImColor color = pLocal->m_iTeamNum == pEntity->m_iTeamNum ? ESP_Team : (visible ? ESP_Visible : ESP_Default);

        // Line
        if (g.g_ESP_Line)
            DrawLine(Vector2(g.g_GameRect.right / 2.f, g.g_GameRect.bottom), Vector2(right - (Width / 2), bottom), color, 1.f);

        // Box
        if (g.g_ESP_Box)
        {
            // BoxFilled
            if (g.g_ESP_BoxFilled)
                RectFilled(left, top,right, bottom, ESP_Shadow, 0.f, NULL);

            // Shadow
            /*
            DrawLine(Vector2(left - 1, top - 1), Vector2(right + 2, top - 1), ESP_Shadow, 1.f);
            DrawLine(Vector2(left - 1, top), Vector2(left - 1, bottom + 2), ESP_Shadow, 1.f);
            DrawLine(Vector2(right + 1, top), Vector2(right + 1, bottom + 2), ESP_Shadow, 1.f);
            DrawLine(Vector2(left - 1, bottom + 1), Vector2(right + 1, bottom + 1), ESP_Shadow, 1.f);
            */
            DrawBox(right + 1, left - 1, top - 1, bottom + 1, ESP_Shadow, 1.f);

            switch (g.g_ESP_BoxType)
            {
            case 0:
                DrawBox(right, left, top, bottom, color, 1.f);
                break;
            case 1:
                DrawLine(Vector2(left, top), Vector2(left + bScale, top), color, 1.f); // Top
                DrawLine(Vector2(right, top), Vector2(right - bScale, top), color, 1.f);
                DrawLine(Vector2(left, top), Vector2(left, top + bScale), color, 1.f); // Left
                DrawLine(Vector2(left, bottom), Vector2(left, bottom - bScale), color, 1.f);
                DrawLine(Vector2(right, top), Vector2(right, top + bScale), color, 1.f); // Right
                DrawLine(Vector2(right, bottom), Vector2(right, bottom - bScale), color, 1.f);
                DrawLine(Vector2(left, bottom), Vector2(left + bScale, bottom), color, 1.f); // Bottom
                DrawLine(Vector2(right + 1, bottom), Vector2(right - bScale, bottom), color, 1.f);
                break;
            }
        }

        // Skeleton
        if (g.g_ESP_Skeleton)
        {
            if (BoneList.size() == 32) {
                // 頭の円をレンダリング
                Vector2 pHead, pNeck;
                if (!WorldToScreen(ViewMatrix, g.g_GameRect, BoneList[BONE_HEAD], pHead) ||
                    !WorldToScreen(ViewMatrix, g.g_GameRect, BoneList[BONE_NECK], pNeck))
                    continue;

                Circle(pHead, pNeck.y - pHead.y, color);

                // 線を引くためのペアを作成する
                const Vector3 skeleton_list[][2] = {
                    { BoneList[BONE_NECK], BoneList[BONE_HIP] },
                    { BoneList[BONE_NECK], BoneList[BONE_LEFT_SHOULDER] },
                    { BoneList[BONE_LEFT_SHOULDER], BoneList[BONE_LEFT_ARM] },
                    { BoneList[BONE_LEFT_ARM], BoneList[BONE_LEFT_HAND] },
                    { BoneList[BONE_NECK], BoneList[BONE_RIGHT_SHOULDER] },
                    { BoneList[BONE_RIGHT_SHOULDER], BoneList[BONE_RIGHT_ARM] },
                    { BoneList[BONE_RIGHT_ARM], BoneList[BONE_RIGHT_HAND] },
                    { BoneList[BONE_HIP], BoneList[BONE_LEFT_KNEE] },
                    { BoneList[BONE_LEFT_KNEE], BoneList[BONE_LEFT_FEET] },
                    { BoneList[BONE_HIP], BoneList[BONE_RIGHT_KNEE] },
                    { BoneList[BONE_RIGHT_KNEE], BoneList[BONE_RIGHT_FEET] }
                };

                // WorldToScreenを行い各ペアをレンダリングする.
                for (int j = 0; j < 11; j++)
                {
                    Vector2 vOut0, vOut1;
                    if (!WorldToScreen(ViewMatrix, g.g_GameRect, skeleton_list[j][0], vOut0) ||
                        !WorldToScreen(ViewMatrix, g.g_GameRect, skeleton_list[j][1], vOut1))
                        break;

                    DrawLine(vOut0, vOut1, color, 1.f);
                }
            }
        }

        // Healthbar
        if (g.g_ESP_HealthBar) {
            HealthBar(left - 3, bottom + 1, 1, -Height - 1, pEntity->m_iHealth, pEntity->m_iMaxHealth); // Health
            ArmorBar(right + 3, bottom + 1, 1, -Height - 1, pEntity->m_ArmorValue, pEntity->m_iMaxHealth); // Armor
        }

        // Name
        if (g.g_ESP_Name) {
            StringEx(Vector2(right - Center - (ImGui::CalcTextSize(pEntity->m_namePlayer).x / 2.f), top - ImGui::GetFontSize()), TEXT_COLOR, ImGui::GetFontSize(), pEntity->m_namePlayer);
        }

        // DIstance & Weapon
        std::string outStr;

        // Distance
        if (g.g_ESP_Distance)
            outStr += "[ " + std::to_string((int)flDistance) + "m ]";

        // Weapon
        if (g.g_ESP_CurrentWeapon)
            outStr += " " + pEntity->m_nameWeapon;

        // Rendering
        if (g.g_ESP_Distance || g.g_ESP_CurrentWeapon)
            StringEx(Vector2(right - Center - (ImGui::CalcTextSize(outStr.c_str()).x / 2.f), bottom + 1), TEXT_COLOR, ImGui::GetFontSize(), outStr.c_str());

        if (flDistance > g.g_AimMaxDistance)
            continue;

        // AimBot
        if (g.g_AimBot && pLocal->m_iTeamNum != pEntity->m_iTeamNum)
        {
            for (const auto& bone : BoneList)
            {
                Vector2 BoneScreen{};
                if (!WorldToScreen(ViewMatrix, g.g_GameRect, bone, BoneScreen))
                    break;

                // In FOV?
                FOV = abs((ScreenMiddle - BoneScreen).Length());

                if (FOV < g.g_AimFOV)
                {
                    switch (g.g_AimType)
                    {
                    case 0: // Crosshair
                        if (MinFov > FOV) {
                            if (target.address == NULL || MinDistance > flDistance)
                            {
                                target = entity;
                                MinFov = FOV;
                                MinDistance = flDistance;
                            }
                        }
                        break;
                    case 1: // Game Distance
                        if (MinDistance > flDistance) {
                            target = entity;
                            MinDistance = flDistance;
                        }
                        break;
                    }
                }
            }
        }

        BoneList.clear();
    }

    // AimBotを実行
    if (target.address != NULL && AimAllow())
    {
        if (!target.m_nameClass.compare("cs_player_controller"))
        {
            int boneId = 1;
            switch (g.g_AimBone)
            {
            case 0: boneId = BONE_HEAD; break;
            case 1: boneId = BONE_NECK; break;
            case 2: boneId = BONE_SPINE; break;
            case 3: boneId = BONE_HIP; break;
            default:
                break;
            }

            Vector2 Angle = CalcAngle(pLocal->GetCameraPosition(), target.GetBoneByID(boneId));
            Vector3 ViewAngle = pLocal->GetViewAngle();
            Vector2 Delta = Angle - ViewAngle;
            NormalizeAngles(Delta);
            Vector2 SmoothedAngle = ViewAngle + (Delta / g.g_AimSmooth);
            NormalizeAngles(SmoothedAngle);

            if (!Vec2_Empty(SmoothedAngle))
            {
                //m.Write<Vector2>(pLocal->m_pCSPlayerPawn + 0x124C, SmoothedAngle);
                m.Write<Vector2>(m.m_gClientBaseAddr + offset::dwViewAngles, SmoothedAngle);
            }
        }
    }

    ImGui::End();
}