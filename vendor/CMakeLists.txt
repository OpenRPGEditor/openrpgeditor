include(FetchContent)

# Dependencies

FetchContent_Declare(
        fmt
        GIT_REPOSITORY "https://github.com/fmtlib/fmt.git"
        GIT_TAG 10.2.1)

FetchContent_Declare(
        imgui
        #GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
        GIT_REPOSITORY "https://github.com/Antidote/imgui"
        #GIT_TAG docking # Branch: docking
        GIT_TAG docking-layout
)

#FetchContent_Declare(
#        ImGuiFileDialog
#        GIT_REPOSITORY "https://github.com/aiekick/ImGuiFileDialog.git"
#        GIT_TAG v0.6.7
#)

FetchContent_Declare(
        clip
        GIT_REPOSITORY "https://github.com/dacap/clip.git"
        GIT_TAG main
)

FetchContent_Declare(
        implot
        GIT_REPOSITORY "https://github.com/epezent/implot.git"
        GIT_TAG master
)

FetchContent_Declare(
        SDL3
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
        GIT_TAG release-3.2.12)

FetchContent_Declare(
        SDL3_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
        GIT_TAG release-3.2.x)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
        GIT_TAG v1.15.2)

FetchContent_Declare(json
        GIT_REPOSITORY "https://github.com/nlohmann/json.git"
)

FetchContent_Declare(magic_enum
        #GIT_REPOSITORY "https://github.com/Neargye/magic_enum.git"
        GIT_REPOSITORY "https://github.com/Antidote/magic_enum"
        GIT_TAG adl_info_definition_failure
)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2)
FetchContent_MakeAvailable(
        SFML
)

if (NOT TARGET freetype)
    FetchContent_Declare(freetype2
            GIT_REPOSITORY "https://github.com/freetype/freetype"
            GIT_TAG VER-2-13-3
    )
    FetchContent_MakeAvailable(
            freetype2
    )
endif ()

FetchContent_Declare(curl
        GIT_REPOSITORY "https://github.com/curl/curl.git"
        GIT_TAG curl-8_10_1
)

FetchContent_Declare(cpuid
        GIT_REPOSITORY "https://github.com/anrieff/libcpuid.git"
        GIT_TAG v0.7.0
)

FetchContent_Declare(athena
        GIT_REPOSITORY "https://github.com/libAthena/athena.git"
        GIT_TAG master
)

FetchContent_Declare(liblcf
        GIT_REPOSITORY "https://github.com/EasyRPG/liblcf.git"
)

FetchContent_Declare(moloader
        GIT_REPOSITORY "https://github.com/Antidote/moloader.git"
        GIT_TAG main
)
# Settings

# For SDL2 to be able to override options
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(BUILD_STATIC_LIBRARIES ON)
set(BUILD_SHARED_LIBRARIES OFF)
set(BUILD_CURL_EXE OFF)
set(DOCTEST_NO_INSTALL ON)
set(FMT_INSTALL OFF)
set(FMT_STATIC ON)
set(SDL3_DISABLE_SDL2MAIN ON)
set(SDL_STATIC_ENABLED_BY_DEFAULT OFF)
set(BUILD_SHARED_LIBS OFF)
set(SPDLOG_FMT_EXTERNAL ON)
set(SPDLOG_BUILD_SHARED OFF)
set(SPDLOG_ENABLE_PCH ON)
set(SDLIMAGE_VENDORED OFF)
set(LIBLCF_WITH_XML OFF)
set(JSON_ImplicitConversions OFF)
set(LIBLCF_WITH_INI OFF)
if (APPLE)
    set(LIBLCF_WITH_ICU OFF)
endif ()
# Populate

FetchContent_MakeAvailable(
        fmt
        SDL3
        SDL3_image
        spdlog
        json
        magic_enum
        clip
        curl
        liblcf
        imgui
        implot
        cpuid
        athena
        moloader
)

add_subdirectory(imgui-setup)

#target_include_directories(ImGuiFileDialog PRIVATE ${imgui_SOURCE_DIR})
#target_compile_definitions(ImGuiFileDialog PRIVATE USE_THUMBNAILS DONT_DEFINE_AGAIN__STB_IMAGE_IMPLEMENTATION)
#target_compile_options(ImGuiFileDialog PRIVATE "-Wno-deprecated-enum-enum-conversion")