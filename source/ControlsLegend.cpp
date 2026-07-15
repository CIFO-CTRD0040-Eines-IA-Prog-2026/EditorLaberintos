#include <imgui.h>
#include "ControlsLegend.h"

static void BulletLegendLine(const char* _sLabel, const char* _sDescription)
{
    ImGui::Bullet();
    ImGui::SameLine();
    float fIndent = ImGui::GetCursorPosX();
    float fWrapPos = fIndent + ImGui::GetContentRegionAvail().x;
    ImGui::PushTextWrapPos(fWrapPos);
    ImGui::TextColored(ImVec4(224.0f / 255.0f, 49.0f / 255.0f, 49.0f / 255.0f, 1.0f), "%s:", _sLabel);
    ImGui::SetCursorPosX(fIndent);
    ImGui::Text("%s", _sDescription);
    ImGui::PopTextWrapPos();
}

ControlsLegend::ControlsLegend()
    : m_bVisible(true)
{
}

void ControlsLegend::Toggle()
{
    m_bVisible = !m_bVisible;
}

void ControlsLegend::Render()
{
    if (!m_bVisible)
    {
        return;
    }

    ImGuiViewport* pViewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(
        ImVec2(pViewport->WorkPos.x + 10.0f,
               pViewport->WorkPos.y + 10.0f),
        ImGuiCond_FirstUseEver,
        ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.85f);

    float fMaxWidth = pViewport->WorkSize.x * 0.25f;
    ImGui::SetNextWindowSizeConstraints(ImVec2(100.0f, 100.0f), ImVec2(fMaxWidth, FLT_MAX));

    if (!ImGui::Begin("Controles", nullptr,
                      ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }

    ImGui::TextColored(ImVec4(224.0f / 255.0f, 49.0f / 255.0f, 49.0f / 255.0f, 1.0f), "Controles edicion celdas:");
    BulletLegendLine("Click izquierdo", "Intercambia el tipo entre pared y suelo");
    BulletLegendLine("Click derecho", "Indica la posicion de la camara");

    ImGui::Separator();

    ImGui::TextColored(ImVec4(224.0f / 255.0f, 49.0f / 255.0f, 49.0f / 255.0f, 1.0f), "Controles navegacion:");
    BulletLegendLine("Flechas arriba y abajo", "Avanzar/retroceder celda en la direccion actual");
    BulletLegendLine("Flechas izquierda y derecha", "Girar 90\xc2\xba a izquierda/derecha");
    BulletLegendLine("Pulsando SHIFT", "Las flechas de cursor mueven de forma absoluta, independientemente de la direccion");

    ImGui::End();
}
