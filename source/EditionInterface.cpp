#include <cstdio>
#include <filesystem>
#include <imgui.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_dialog.h>
#include "EditionInterface.h"

namespace fs = std::filesystem;

static const char* LAB_FILTER_NAME = "Laberinto files";
static const char* LAB_FILTER_PATTERN = "lab";

static const ImVec4 COLOR_WALL(52.0f / 255.0f, 58.0f / 255.0f, 64.0f / 255.0f, 1.0f);
static const ImVec4 COLOR_FLOOR(206.0f / 255.0f, 212.0f / 255.0f, 218.0f / 255.0f, 1.0f);
static const ImVec4 COLOR_WALL_HOVER(70.0f / 255.0f, 78.0f / 255.0f, 84.0f / 255.0f, 1.0f);
static const ImVec4 COLOR_FLOOR_HOVER(220.0f / 255.0f, 226.0f / 255.0f, 232.0f / 255.0f, 1.0f);

EditionInterface::EditionInterface(SDL_Window* _pWindow, Maze& _oMaze, NavigationManager& _oNav)
    : m_pWindow(_pWindow)
    , m_oMaze(_oMaze)
    , m_oNav(_oNav)
    , m_oMarker(_oNav)
    , m_bUnsavedChanges(false)
    , m_bShowDiscardConfirm(false)
{
    for (unsigned int uY = 0; uY < m_oMaze.m_uHeight; ++uY)
    {
        for (unsigned int uX = 0; uX < m_oMaze.m_uWidth; ++uX)
        {
            m_oMaze.SetCell(uX, uY, CellType::Floor);
        }
    }
}

std::string EditionInterface::GetWindowTitle() const
{
    std::string sTitle = "Editor de Laberintos";

    if (m_sCurrentFilePath.empty())
    {
        sTitle += " - NEW";
    }
    else
    {
        fs::path oPath(m_sCurrentFilePath);
        sTitle += " - " + oPath.filename().string();
    }

    return sTitle;
}

void EditionInterface::Update()
{
    {
        std::lock_guard<std::mutex> oLock(m_oFileMutex);

        if (!m_sPendingOpenPath.empty())
        {
            std::string sPath = std::move(m_sPendingOpenPath);
            DoOpen(sPath);
        }
        if (!m_sPendingSavePath.empty())
        {
            std::string sPath = std::move(m_sPendingSavePath);
            DoSaveAs(sPath);
        }
    }

    if (m_bShowDiscardConfirm)
    {
        ImGui::OpenPopup("Descartar cambios");
        m_bShowDiscardConfirm = false;
    }

    ImGuiViewport* pViewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(pViewport->WorkPos.x + pViewport->WorkSize.x * 0.65f, pViewport->WorkPos.y), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(pViewport->WorkSize.x * 0.35f, pViewport->WorkSize.y), ImGuiCond_FirstUseEver);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(52.0f / 255.0f, 58.0f / 255.0f, 64.0f / 255.0f, 0.85f));
    ImGui::Begin("Editor", nullptr, 0);

    RenderFileButtons();
    ImGui::Separator();
    RenderDimensionsPanel();
    ImGui::Separator();
    RenderCellsGrid();

    ImGui::End();
    ImGui::PopStyleColor();

    if (ImGui::BeginPopupModal("Descartar cambios", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Hay cambios sin guardar. Se descartaran?");
        if (ImGui::Button("Si"))
        {
            ImGui::CloseCurrentPopup();
            SDL_ShowOpenFileDialog(OnOpenCallback, this, m_pWindow, nullptr, 0, nullptr, false);
        }
        ImGui::SameLine();
        if (ImGui::Button("No"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    SDL_SetWindowTitle(m_pWindow, GetWindowTitle().c_str());
}

void EditionInterface::RenderFileButtons()
{
    if (ImGui::Button("Abrir", ImVec2(-1.0f, 0.0f)))
    {
        if (m_bUnsavedChanges)
        {
            m_bShowDiscardConfirm = true;
        }
        else
        {
            SDL_ShowOpenFileDialog(OnOpenCallback, this, m_pWindow, nullptr, 0, nullptr, false);
        }
    }

    if (ImGui::Button("Guardar", ImVec2(-1.0f, 0.0f)))
    {
        DoSave();
    }

    if (ImGui::Button("Guardar como", ImVec2(-1.0f, 0.0f)))
    {
        SDL_DialogFileFilter oFilter = { LAB_FILTER_NAME, LAB_FILTER_PATTERN };
        SDL_ShowSaveFileDialog(OnSaveCallback, this, m_pWindow, &oFilter, 1, nullptr);
    }
}

void EditionInterface::RenderDimensionsPanel()
{
    ImGui::Text("Dimensiones");

    int iWidth = static_cast<int>(m_oMaze.m_uWidth);
    int iHeight = static_cast<int>(m_oMaze.m_uHeight);

    if (ImGui::InputInt("Ancho", &iWidth, 1, 5))
    {
        m_bUnsavedChanges = true;
    }
    if (ImGui::InputInt("Alto", &iHeight, 1, 5))
    {
        m_bUnsavedChanges = true;
    }

    iWidth = (std::max)(0, (std::min)(iWidth, 30));
    iHeight = (std::max)(0, (std::min)(iHeight, 30));

    if (static_cast<unsigned int>(iWidth) != m_oMaze.m_uWidth ||
        static_cast<unsigned int>(iHeight) != m_oMaze.m_uHeight)
    {
        Maze oNewMaze(static_cast<unsigned int>(iWidth), static_cast<unsigned int>(iHeight));

        for (unsigned int uY = 0; uY < oNewMaze.m_uHeight; ++uY)
        {
            for (unsigned int uX = 0; uX < oNewMaze.m_uWidth; ++uX)
            {
                if (uX < m_oMaze.m_uWidth && uY < m_oMaze.m_uHeight)
                {
                    oNewMaze.SetCell(uX, uY, m_oMaze.GetCell(uX, uY));
                }
                else
                {
                    oNewMaze.SetCell(uX, uY, CellType::Floor);
                }
            }
        }

        m_oMaze.m_uWidth = oNewMaze.m_uWidth;
        m_oMaze.m_uHeight = oNewMaze.m_uHeight;
        m_oMaze.m_oCells = std::move(oNewMaze.m_oCells);

        m_oNav.FinishInterpolation();
        m_bUnsavedChanges = true;
    }
}

void EditionInterface::RenderCellsGrid()
{
    ImGui::Text("Celdas");

    const float fCellSize = 24.0f;
    const float fLineWidth = 1.0f;

    ImVec2 oGridSize(
        static_cast<float>(m_oMaze.m_uWidth) * (fCellSize + fLineWidth) + fLineWidth,
        static_cast<float>(m_oMaze.m_uHeight) * (fCellSize + fLineWidth) + fLineWidth);

    ImVec2 oAvailSize = ImGui::GetContentRegionAvail();
    ImVec2 oPanelSize(
        (std::min)(oGridSize.x, oAvailSize.x),
        (std::min)(oGridSize.y, oAvailSize.y));

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::BeginChild("Grid", oPanelSize, 0,
                      ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(fLineWidth, fLineWidth));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + fLineWidth);

    for (unsigned int uY = 0; uY < m_oMaze.m_uHeight; ++uY)
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + fLineWidth);

        for (unsigned int uX = 0; uX < m_oMaze.m_uWidth; ++uX)
        {
            CellType eCell = m_oMaze.GetCell(uX, uY);

            ImGui::PushID(static_cast<int>(uY * m_oMaze.m_uWidth + uX));

            if (eCell == CellType::Wall)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, COLOR_WALL);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_WALL_HOVER);
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, COLOR_FLOOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COLOR_FLOOR_HOVER);
            }

            if (ImGui::Button(" ", ImVec2(fCellSize, fCellSize)))
            {
                CellType eNewType = (eCell == CellType::Wall) ? CellType::Floor : CellType::Wall;
                m_oMaze.SetCell(uX, uY, eNewType);
                m_bUnsavedChanges = true;
                m_oNav.FinishInterpolation();
            }

            if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && eCell == CellType::Floor)
            {
                m_oNav.SetPosition(uX, uY);
            }

            ImGui::PopStyleColor(2);
            ImGui::PopID();

            if (uX + 1 < m_oMaze.m_uWidth)
            {
                ImGui::SameLine(0.0f, -1.0f);
            }
        }
    }

    ImVec2 oChildPos = ImGui::GetWindowPos();
    float fScrollX = ImGui::GetScrollX();
    float fScrollY = ImGui::GetScrollY();
    m_oMarker.Render(ImGui::GetWindowDrawList(), oChildPos.x - fScrollX, oChildPos.y - fScrollY, fCellSize, fLineWidth);

    ImGui::PopStyleVar(3);
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void EditionInterface::DoOpen(const std::string& _sPath)
{
    Maze oLoaded = Maze::Load(_sPath.c_str());

    m_oMaze.m_uWidth = oLoaded.m_uWidth;
    m_oMaze.m_uHeight = oLoaded.m_uHeight;
    m_oMaze.m_oCells = std::move(oLoaded.m_oCells);

    m_sCurrentFilePath = _sPath;
    m_bUnsavedChanges = false;

    m_oNav.FinishInterpolation();
    m_oNav.SetPosition(oLoaded.m_uNavX, oLoaded.m_uNavY);
    m_oNav.SetOrientation(oLoaded.m_eNavOrientation);
}

void EditionInterface::DoSave()
{
    if (m_sCurrentFilePath.empty())
    {
        SDL_DialogFileFilter oFilter = { LAB_FILTER_NAME, LAB_FILTER_PATTERN };
        SDL_ShowSaveFileDialog(OnSaveCallback, this, m_pWindow, &oFilter, 1, nullptr);
        return;
    }

    m_oMaze.m_uNavX = m_oNav.GetX();
    m_oMaze.m_uNavY = m_oNav.GetY();
    m_oMaze.m_eNavOrientation = m_oNav.GetOrientation();
    m_oMaze.Save(m_sCurrentFilePath.c_str());
    m_bUnsavedChanges = false;
}

void EditionInterface::DoSaveAs(const std::string& _sPath)
{
    m_oMaze.m_uNavX = m_oNav.GetX();
    m_oMaze.m_uNavY = m_oNav.GetY();
    m_oMaze.m_eNavOrientation = m_oNav.GetOrientation();
    m_oMaze.Save(_sPath.c_str());
    m_sCurrentFilePath = _sPath;
    m_bUnsavedChanges = false;
}

void SDLCALL EditionInterface::OnOpenCallback(void* _pUserData, const char* const* _sFiles, int /*_iFilterIndex*/)
{
    auto* pSelf = static_cast<EditionInterface*>(_pUserData);

    if (!_sFiles || !_sFiles[0])
    {
        return;
    }

    std::lock_guard<std::mutex> oLock(pSelf->m_oFileMutex);
    pSelf->m_sPendingOpenPath = _sFiles[0];
}

void SDLCALL EditionInterface::OnSaveCallback(void* _pUserData, const char* const* _sFiles, int /*_iFilterIndex*/)
{
    auto* pSelf = static_cast<EditionInterface*>(_pUserData);

    if (!_sFiles || !_sFiles[0])
    {
        return;
    }

    std::string sPath = _sFiles[0];
    if (sPath.find('.') == std::string::npos)
    {
        sPath += ".lab";
    }

    std::lock_guard<std::mutex> oLock(pSelf->m_oFileMutex);
    pSelf->m_sPendingSavePath = std::move(sPath);
}
