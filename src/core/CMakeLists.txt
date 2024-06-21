set(NAME "Core")

#include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(
        ${NAME} STATIC
        Core/Log.hpp Core/Log.cpp
        Core/Debug/Instrumentor.hpp
        Core/Application.hpp Core/Application.cpp
        Core/Window.hpp Core/Window.cpp
        Core/Project.hpp Core/Project.cpp
        Core/stb_image.h Core/image_loader.h Core/image_loader.c
        Core/ResourceManager.hpp Core/ResourceManager.cpp
        Core/Texture.hpp Core/Texture.cpp
        Core/ImGuiUtils.hpp Core/ImGuiUtils.cpp
        Core/TilesetPicker.hpp Core/TilesetPicker.cpp

        Core/MapListView.hpp Core/MapListView.cpp
        Core/EventListView.hpp Core/EventListView.cpp

        Core/MapEditor.hpp Core/MapEditor.cpp
        Core/MapEditor/MapEvent.hpp Core/MapEditor/MapEvent.cpp
        Core/MapEditor/MapCursor.hpp Core/MapEditor/MapCursor.cpp
        Core/MapEditor/MapRenderer.hpp Core/MapEditor/MapRenderer.cpp

        Core/EventEditor.cpp Core/EventEditor.cpp
        Core/EventEditor/EVPage.hpp Core/EventEditor/EVPage.cpp
        Core/DatabaseEditor.cpp Core/DatabaseEditor.hpp
        Core/DatabaseEditor/DBActorsTab.hpp Core/DatabaseEditor/DBActorsTab.cpp
        Core/DatabaseEditor/DBClassesTab.hpp Core/DatabaseEditor/DBClassesTab.cpp
        Core/DatabaseEditor/DBSkillsTab.hpp Core/DatabaseEditor/DBSkillsTab.cpp
        Core/DatabaseEditor/DBItemsTab.hpp Core/DatabaseEditor/DBItemsTab.cpp
        Core/DatabaseEditor/DBWeaponsTab.hpp Core/DatabaseEditor/DBWeaponsTab.cpp
        Core/DatabaseEditor/DBArmorsTab.hpp Core/DatabaseEditor/DBArmorsTab.cpp
        Core/DatabaseEditor/DBEnemiesTab.hpp Core/DatabaseEditor/DBEnemiesTab.cpp
        Core/DatabaseEditor/DBTroopsTab.hpp Core/DatabaseEditor/DBTroopsTab.cpp
        Core/DatabaseEditor/DBStatesTab.hpp Core/DatabaseEditor/DBStatesTab.cpp
        Core/DatabaseEditor/DBAnimationsTab.hpp Core/DatabaseEditor/DBAnimationsTab.cpp
        Core/DatabaseEditor/DBTilesetsTab.hpp Core/DatabaseEditor/DBTilesetsTab.cpp
        Core/DatabaseEditor/DBCommonEventsTab.hpp Core/DatabaseEditor/DBCommonEventsTab.cpp
        Core/DatabaseEditor/DBSystemTab.hpp Core/DatabaseEditor/DBSystemTab.cpp

        Core/CommonUI/TraitsEditor.hpp Core/CommonUI/TraitsEditor.cpp
        Core/CommonUI/ObjectPicker.hpp Core/CommonUI/ObjectPicker.inl
        Core/CommonUI/VariableSwitchPicker.hpp Core/CommonUI/VariableSwitchPicker.cpp
        Core/CommonUI/EventCommandEditor.hpp Core/CommonUI/EventCommandEditor.cpp
        Core/CommonUI/TextEditor.hpp Core/CommonUI/TextEditor.cpp
        Core/UndoStack.hpp Core/UndoStack.cpp

        Core/CheckerboardTexture.hpp Core/CheckerboardTexture.cpp
        Core/CharacterSheet.hpp Core/CharacterSheet.cpp
        Core/Settings.hpp Core/Settings.cpp
        Core/ImGuiParsedText.hpp Core/ImGuiParsedText.cpp
        Core/TextParser.hpp Core/TextParser.cpp
        Core/Resources.hpp
        Core/DPIHandler.hpp
        Core/EventCommands/IDialogController.hpp
        Core/EventCommands/Dialog_ControlSwitches.cpp
        Core/EventCommands/Dialog_ControlSwitches.hpp
)

# Define set of OS specific files to include
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(${NAME} PRIVATE Platform/Windows/Resources.cpp
            Platform/Windows/DPIHandler.cpp)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_sources(${NAME} PRIVATE Platform/Mac/Resources.cpp
            Platform/Mac/DPIHandler.cpp)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(${NAME} PRIVATE Platform/Linux/Resources.cpp
            Platform/Linux/DPIHandler.cpp)
endif ()

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(
        ${NAME}
        PRIVATE project_warnings
        PUBLIC fmt::fmt-header-only spdlog::spdlog_header_only
        SDL2::SDL2
        imgui
        ImGuiFileDialog
        magic_enum
        nlohmann_json::nlohmann_json
        Settings
        librpgm
        freetype
        z
)

#add_subdirectory(Tests)
