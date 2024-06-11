include(FetchContent)

# Dependencies

FetchContent_Declare(
        doctest
        GIT_REPOSITORY "https://github.com/onqtam/doctest.git"
        GIT_TAG v2.4.11)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
        GIT_TAG 10.2.1)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
        GIT_TAG 222dd775fc72229675757f90d887ec57a291dc0e # Branch: docking
)

FetchContent_Declare(
        ImGuiFileDialog
        GIT_REPOSITORY "https://github.com/aiekick/ImGuiFileDialog.git"
        GIT_TAG v0.6.7
)

FetchContent_Declare(
        SDL2
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
        GIT_TAG release-2.30.3)

FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY "https://github.com/Antidote/SDL_image.git"
        GIT_TAG xcf-fix)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
        GIT_TAG v1.14.1)

FetchContent_Declare(json GIT_REPOSITORY "https://github.com/nlohmann/json.git")

FetchContent_Declare(magic_enum
        GIT_REPOSITORY "https://github.com/Neargye/magic_enum.git")

#FetchContent_Declare(liblcf
#        GIT_REPOSITORY "https://github.com/EasyRPG/liblcf.git"
#        GIT_TAG "0.8"
#)
# Settings

# For SDL2 to be able to override options
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(LIBLCF_WITH_ICU OFF)
set(DOCTEST_NO_INSTALL ON)
set(FMT_INSTALL OFF)
set(FMT_STATIC ON)
set(SDL2_DISABLE_SDL2MAIN ON)
set(SDL_STATIC_ENABLED_BY_DEFAULT OFF)
set(BUILD_SHARED_LIBS OFF)
set(SPDLOG_FMT_EXTERNAL ON)
set(SPDLOG_BUILD_SHARED OFF)
set(SPDLOG_ENABLE_PCH ON)

add_subdirectory(imgui-setup)

# Populate

FetchContent_MakeAvailable(
        doctest
        fmt
        imgui
        ImGuiFileDialog
        SDL2
        SDL2_image
        spdlog
        json
        magic_enum
        #liblcf
)


target_include_directories(ImGuiFileDialog PRIVATE ${imgui_SOURCE_DIR})
