cmake_minimum_required(VERSION 3.22)

include(cmake/AppleBuild.cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

add_definitions(-DJSON_DIAGNOSTICS=1)
set(NAME "OpenRPGEditor")

project(
        ${NAME}
        DESCRIPTION "A drop-in replacement for RPG Maker MV."
        VERSION 0.0.0
        LANGUAGES CXX C)

set(GETTEXT_DOMAIN openrpgeditor)
set(GETTEXT_OUTPUT_DIR locales)

set(PROJECT_COMPANY_NAME "OpenRPGEditor Team")
set(PROJECT_COMPANY_NAMESPACE "io.gitgud.antidote.openrgpeditor") # Reverse
# domain name
# notation

include(cmake/StandardProjectSettings.cmake)
include(GNUInstallDirs)

# Link this "library" to use the warnings specified in CompilerWarnings.cmake.
include(cmake/CompilerWarnings.cmake)

# enable_testing()

add_subdirectory(packaging)
add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(extern/angelscript/angelscript/projects/cmake)
add_subdirectory(extern/angelscript_addon_cmake)
add_subdirectory(i18n)

target_link_libraries(librpgm PRIVATE
        moloader::moloader
)
target_include_directories(librpgm PUBLIC
        i18n
        ${moloader_SOURCE_DIR}/include
)
target_compile_definitions(librpgm PUBLIC GETTEXT_DOMAIN="${GETTEXT_DOMAIN}" GETTEXT_OUTPUT_DIR="${GETTEXT_OUTPUT_DIR}")


target_link_libraries(librpgmeventcmds PRIVATE
        moloader::moloader
)
target_compile_definitions(librpgmeventcmds PUBLIC GETTEXT_DOMAIN="${GETTEXT_DOMAIN}" GETTEXT_OUTPUT_DIR="${GETTEXT_OUTPUT_DIR}")


target_include_directories(librpgmeventcmds PUBLIC
        i18n
        ${moloader_SOURCE_DIR}/include
)

target_link_libraries(librpgmmovementroutecmds PRIVATE
        moloader::moloader
)
target_include_directories(librpgmmovementroutecmds PUBLIC
        i18n
        ${moloader_SOURCE_DIR}/include
)
target_compile_definitions(librpgmmovementroutecmds PUBLIC GETTEXT_DOMAIN="${GETTEXT_DOMAIN}" GETTEXT_OUTPUT_DIR="${GETTEXT_OUTPUT_DIR}")
