#ifndef EDITIONINTERFACE_H
#define EDITIONINTERFACE_H

#include <string>
#include <mutex>
#include <SDL3/SDL_video.h>
#include "Maze.h"
#include "NavigationMarker.h"

class EditionInterface
{
public:
    EditionInterface(SDL_Window* _pWindow, Maze& _oMaze, NavigationManager& _oNav);
    ~EditionInterface() = default;

    void Update();
    std::string GetWindowTitle() const;

private:
    SDL_Window* m_pWindow;
    Maze& m_oMaze;
    NavigationManager& m_oNav;
    NavigationMarker m_oMarker;
    std::string m_sCurrentFilePath;
    bool m_bUnsavedChanges;

    mutable std::mutex m_oFileMutex;
    std::string m_sPendingOpenPath;
    std::string m_sPendingSavePath;

    std::string m_sPendingConfirmPath;
    bool m_bShowDiscardConfirm;

    void RenderFileButtons();
    void RenderDimensionsPanel();
    void RenderCellsGrid();

    void DoOpen(const std::string& _sPath);
    void DoSave();
    void DoSaveAs(const std::string& _sPath);

    static void SDLCALL OnOpenCallback(void* _pUserData, const char* const* _sFiles, int _iFilterIndex);
    static void SDLCALL OnSaveCallback(void* _pUserData, const char* const* _sFiles, int _iFilterIndex);
};

#endif
