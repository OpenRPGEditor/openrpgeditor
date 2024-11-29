cmake_minimum_required(VERSION 3.22)

include(cmake/AppleBuild.cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

add_definitions(-DJSON_DIAGNOSTICS=1)
project(
        OpenRPGEditor
        DESCRIPTION "A drop-in replacement for RPG Maker MV."
        VERSION 0.0.0
        LANGUAGES CXX C)

set(PROJECT_COMPANY_NAME "OpenRPGEditor Team")
set(PROJECT_COMPANY_NAMESPACE "io.gitgud.antidote.openrgpeditor") # Reverse domain name notation

include(cmake/StandardProjectSettings.cmake)
include(GNUInstallDirs)

# Link this "library" to use the warnings specified in CompilerWarnings.cmake.
add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)

#enable_testing()

add_subdirectory(packaging)
add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(extern/angelscript/angelscript/projects/cmake)