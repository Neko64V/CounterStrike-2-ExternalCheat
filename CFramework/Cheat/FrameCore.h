#pragma once
#include "../Framework/ImGui/imgui.h"
#include "../Framework/ImGui/imgui_impl_win32.h"
#include "../Framework/ImGui/imgui_impl_dx11.h"
#include "../Framework/ImGui/Custom.h"
#include "SDK/CEntity/CEntity.h"

class CFramework
{
public:
    void UpdateList();
    void MiscAll();

	void RenderInfo();
	void RenderMenu();
	void RenderESP();
private:
    // Entity
    CEntity local, *pLocal = &local;
    std::vector<CEntity> EntityList;
    std::vector<std::string> SpectatorPlayerName;

    // Colors
    float m_flGlobalAlpha = 0.925f;
    float m_flShadowAlpha = 0.235f;
    ImColor ESP_Default = { 1.f, 1.f, 1.f, m_flGlobalAlpha };
    ImColor ESP_Visible = { 1.f, 0.f, 0.f, m_flGlobalAlpha };
    ImColor ESP_Team    = { 0.f, 0.75f, 1.f, m_flGlobalAlpha };
    ImColor ESP_Shadow  = { 0.f, 0.f, 0.f, m_flShadowAlpha };

    ImColor CrosshairColor = { 0.f, 1.f, 0.f, 1.f };

    ImColor TEXT_COLOR = { 1.f, 1.f, 1.f, m_flGlobalAlpha };

    // int‚É‚µ‚È‚¢‚ÆŸø‚Þ
    void DrawLine(const Vector2 a, const Vector2 b, ImColor color, float width) {
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2((int)a.x, (int)a.y), ImVec2((int)b.x, (int)b.y), color, width);
    }
    void DrawBox(int right, int left, int top, int bottom, ImColor color, float width) {
        DrawLine(Vector2(left, top), Vector2(right, top), color, 1.f);
        DrawLine(Vector2(left, top), Vector2(left, bottom), color, 1.f);
        DrawLine(Vector2(right, top), Vector2(right, bottom), color, 1.f);
        DrawLine(Vector2(left, bottom), Vector2(right + 1, bottom), color, 1.f);
    }
    void RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
    {
        ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2((int)x0, (int)y0), ImVec2((int)x1, (int)y1), color, rounding, rounding_corners_flags);
    }
    void HealthBar(int x, int y, int w, int h, int value, int v_max)
    {
        //RectFilled(x, y, x + w, y + h, ImColor(ESP_Shadow.Value.x, ESP_Shadow.Value.y, ESP_Shadow.Value.z, m_flShadowAlpha), 0.f, 0);
        RectFilled(x - 1, y + 1, x + w + 1, y + h - 1, ImColor(ESP_Shadow.Value.x, ESP_Shadow.Value.y, ESP_Shadow.Value.z, m_flShadowAlpha), 0.f, 0);
        RectFilled(x, y, x + w, y + ((h / float(v_max)) * (float)value), ImColor(min(510 * (v_max - value) / 100, 255), min(510 * value / 100, 255), 25, (int)(255 * m_flGlobalAlpha)), 0.0f, 0);
    }
    void ArmorBar(int x, int y, int w, int h, int value, int v_max)
    {
        RectFilled(x - 1, y + 1, x + w + 1, y + h - 1, ImColor(ESP_Shadow.Value.x, ESP_Shadow.Value.y, ESP_Shadow.Value.z, m_flShadowAlpha), 0.f, 0);
        RectFilled(x, y, x + w, y + h, ImColor(0.f, 0.85f, 1.f, m_flGlobalAlpha), 0.f, 0);
    }
    void Circle(Vector2 pos, float fov_size, ImColor color)
    {
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2((int)pos.x, (int)pos.y), fov_size, color, 100, 0);
    }
    void String(Vector2 pos, ImColor color, const char* text)
    {
        ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), ImVec2((int)pos.x, (int)pos.y), color, text, text + strlen(text), 1024, 0);
    }
    void StringEx(Vector2 pos, ImColor color, float font_size, const char* text)
    {
        ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), font_size, ImVec2((int)pos.x + 1.f, (int)pos.y + 1.f), ImColor(ESP_Shadow.Value.x, ESP_Shadow.Value.y, ESP_Shadow.Value.z, ESP_Shadow.Value.w), text, text + strlen(text), 1024, 0);
        ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), font_size, ImVec2((int)pos.x, (int)pos.y), color, text, text + strlen(text), 1024, 0);
    }
};