cmake_minimum_required(VERSION 3.22)

include(cmake/AppleBuild.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

project(
  OpenRPGMaker
  DESCRIPTION "A drop-in replacement for RPG Maker MV."
  VERSION 1.0.0
  LANGUAGES CXX C)

set(PROJECT_COMPANY_NAME "VHMV Team")
set(PROJECT_COMPANY_NAMESPACE "com.vhmv") # Reverse domain name notation

include(cmake/StandardProjectSettings.cmake)
include(GNUInstallDirs)

# Link this "library" to use the warnings specified in CompilerWarnings.cmake.
add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)

enable_testing()

add_subdirectory(packaging)
add_subdirectory(vendor)
add_subdirectory(src)
