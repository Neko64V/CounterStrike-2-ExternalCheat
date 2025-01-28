#include "FrameCore.h"

void CFramework::RenderInfo()
{
    String(Vector2(), TEXT_COLOR, std::to_string((int)ImGui::GetIO().Framerate).c_str());

    // Crosshair
    if (g.g_Crosshair)
    {
        switch (g.g_CrosshairType)
        {
        case 0: {
            ImVec2 Center = ImVec2(g.g_GameRect.right / 2, g.g_GameRect.bottom / 2);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x - g.g_CrosshairSize, Center.y), ImVec2((Center.x + g.g_CrosshairSize) + 1, Center.y), CrosshairColor, 1);
            ImGui::GetBackgroundDrawList()->AddLine(ImVec2(Center.x, Center.y - g.g_CrosshairSize), ImVec2(Center.x, (Center.y + g.g_CrosshairSize) + 1), CrosshairColor, 1);
        }   break;
        case 1:
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.g_GameRect.right / 2.f, (float)g.g_GameRect.bottom / 2.f), g.g_CrosshairSize + 1, ImColor(0.f, 0.f, 0.f, 1.f), NULL);
            ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2((float)g.g_GameRect.right / 2.f, (float)g.g_GameRect.bottom / 2.f), g.g_CrosshairSize, CrosshairColor, NULL);
            break;
        }
    }

    /*
    // SpectatorList
    if (g.g_SpectatorList)
    {
        if (SpectatorPlayerName.size() > 0)
            String(Vector2(g.g_GameRect.right / 2 - (ImGui::CalcTextSize("[ Spectator Found! ]").x / 2), g.g_GameRect.top), ImColor(1.f, 0.f, 0.f, 1.f), "[ Spectator Found! ]");

        ImGui::SetNextWindowBgAlpha(SpectatorPlayerName.size() > 0 ? 0.9f : 0.35f);
        ImGui::SetNextWindowPos(ImVec2(12.f, 16.f));
        ImGui::SetNextWindowSize(ImVec2(250.f, 125.f));
        std::string title = "Spectator [" + std::to_string(SpectatorPlayerName.size()) + "]";
        ImGui::Begin(title.c_str(), &g.g_ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        std::vector<std::string> spec_list = SpectatorPlayerName;

        for (auto& name : spec_list)
            ImGui::Text(name.c_str());

        ImGui::End();
    }
    */
}

void CFramework::RenderESP()
{
    CEntity* pLocal = &local;

    // Localの更新に失敗したらこの関数を終了
    if (!pLocal->Update())
        return;

    // AimBot関連
    float FOV = 0.f;
    float MinFov = 999.f;
    float MinDistance = 9999.f;
    CEntity target = CEntity();
    Vector2 ScreenMiddle = { (float)g.g_GameRect.right / 2.f, (float)g.g_GameRect.bottom / 2.f };

    // ViewMatrixとかいろいろ
    Matrix ViewMatrix = m.Read<Matrix>(m.m_gClientBaseAddr + offset::dwViewMatrix);

    // るーぷするよ
    for (auto& entity : EntityList)
    {
        CEntity* pEntity = &entity;

        if (!pEntity->Update())
            continue;

        // 距離を取得
        const float pDistance = ((pLocal->m_vOldOrigin - pEntity->m_vOldOrigin).Length() * 0.01905f);

        // 各種チェック
        if (g.g_ESP_MaxDistance < pDistance)
            continue;
        else if (!g.g_ESP_Team && pEntity->m_iTeamNum == pLocal->m_iTeamNum)
            continue;

        /* // 方法1 - 頭とベース座標の位置をベースにする
        Vector2 pBase{}, pHead{};
        const Vector3 Head = pEntity->GetEntityBonePosition(8) + Vector3(0.f, 0.f, 12.f);
        if (!WorldToScreen(ViewMatrix, g.g_GameRect, pEntity->m_vecAbsOrigin + Vector3(0.f, 0.f, -6.f), pBase) || !WorldToScreen(ViewMatrix, g.g_GameRect, Head, pHead))
            continue;
        */

        // 方法2 - m_Collision を使用する方法
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
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(left, top), ImVec2(right, bottom), ESP_Shadow);

            // Shadow
            DrawLine(Vector2(left - 1, top - 1), Vector2(right + 2, top - 1), ESP_Shadow, 1.f);
            DrawLine(Vector2(left - 1, top), Vector2(left - 1, bottom + 2), ESP_Shadow, 1.f);
            DrawLine(Vector2(right + 1, top), Vector2(right + 1, bottom + 2), ESP_Shadow, 1.f);
            DrawLine(Vector2(left - 1, bottom + 1), Vector2(right + 1, bottom + 1), ESP_Shadow, 1.f);

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
            std::vector<Vector3> BoneList = pEntity->GetBoneList();

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

            BoneList.clear();
        }

        // Healthbar
        if (g.g_ESP_HealthBar) {
            HealthBar(left - 3, bottom + 1, 1, -Height - 1, pEntity->m_iHealth, pEntity->m_iMaxHealth); // Health
            ArmorBar(right + 3, bottom + 1, 1, -Height - 1, pEntity->m_ArmorValue, pEntity->m_iMaxHealth); // Armor
        }

        // Distance
        if (g.g_ESP_Distance) {
            const std::string distStr = std::to_string((int)pDistance) + "m";
            StringEx(Vector2(right - Center - (ImGui::CalcTextSize(distStr.c_str()).x / 2.f), bottom + 1), TEXT_COLOR, ImGui::GetFontSize(), distStr.c_str());
        }

        // Name
        if (g.g_ESP_Name) {
            StringEx(Vector2(right - Center - (ImGui::CalcTextSize(pEntity->pName).x / 2.f), top - ImGui::GetFontSize() - 1), TEXT_COLOR, ImGui::GetFontSize(), pEntity->pName);
        }
    }
}