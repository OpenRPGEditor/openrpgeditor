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

find_package(Gettext REQUIRED)
find_package(Intl REQUIRED)
include(cmake/Gettext_helpers.cmake)
include(cmake/Getsourcefilepaths.cmake)

set(GETTEXT_DOMAIN OpenRPGEditor)
set(GETTEXT_TARGET i18n)
set(GETTEXT_OUTPUT_DIR i18n/locales)
set(GETTEXT_LANGUAGES
        en de fr es ja ru
)


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

file(GLOB_RECURSE SOURCE_FILES
        src/*.cpp
        src/*.c
        src/*.hpp
        src/*.h
        src/*.inl
)

set(PROJECT_SOURCES)
foreach (abs_path IN LISTS SOURCE_FILES)
    file(RELATIVE_PATH rel_path "${CMAKE_SOURCE_DIR}" "${abs_path}")
    list(APPEND PROJECT_SOURCES "${rel_path}")
endforeach ()

configure_gettext(
        DOMAIN ${GETTEXT_DOMAIN}
        TARGET_NAME ${GETTEXT_TARGET}
        SOURCES ${PROJECT_SOURCES}
        POTFILE_DESTINATION ${GETTEXT_OUTPUT_DIR}
        XGETTEXT_ARGS
        "--keyword=tr"
        "--keyword=trFormat"
        "--keyword=trNOOP"
        "--keyword=trENUM"
        "--add-comments=TRANSLATORS:"
        "--package-name=OpenRPGEditor"
        "--package-version=\"0.0.1\""
        "--msgid-bugs-address=\"<https://gitgud.io/antidote/openrgpeditor/issues>\""
        "--copyright-holder=\"OpenRPGEditor Team\""
        "--language=c++"
        LANGUAGES ${GETTEXT_LANGUAGES}
        BUILD_DESTINATION $<TARGET_FILE_DIR:${NAME}>/${GETTEXT_OUTPUT_DIR}
        ALL
)

target_link_libraries(librpgm PRIVATE
        Intl::Intl
)
target_include_directories(librpgm PUBLIC
        i18n
)
target_compile_definitions(librpgm PUBLIC GETTEXT_DOMAIN="${GETTEXT_DOMAIN}" GETTEXT_OUTPUT_DIR="${GETTEXT_OUTPUT_DIR}")


target_link_libraries(librpgmeventcmds PRIVATE
        Intl::Intl
)
target_compile_definitions(librpgmeventcmds PUBLIC GETTEXT_DOMAIN="${GETTEXT_DOMAIN}" GETTEXT_OUTPUT_DIR="${GETTEXT_OUTPUT_DIR}")


target_include_directories(librpgmeventcmds PUBLIC
        i18n
)

target_link_libraries(librpgmmovementroutecmds PRIVATE
        Intl::Intl
)
target_include_directories(librpgmmovementroutecmds PUBLIC
        i18n
)
target_compile_definitions(librpgmmovementroutecmds PUBLIC GETTEXT_DOMAIN="${GETTEXT_DOMAIN}" GETTEXT_OUTPUT_DIR="${GETTEXT_OUTPUT_DIR}")

add_dependencies(${NAME} i18n)