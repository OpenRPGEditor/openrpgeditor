set(NAME "Core")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

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
        Core/MapRenderer.hpp Core/MapRenderer.cpp
        Core/Resources.hpp
        Core/DPIHandler.hpp)

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
        librpgm)

add_subdirectory(Tests)
