set(NAME "librpgm")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(
  ${NAME} STATIC
  Database/CommonEvents.hpp
  Database/CommonEvents.cpp
  Database/Animations.hpp
  Database/Animations.cpp
  Database/Animation.hpp
  Database/Animation.cpp
  Database/System.hpp
  Database/System.cpp
  Database/Tilesets.hpp
  Database/Tilesets.cpp
  Database/Actors.hpp
  Database/Actors.cpp
  # Image Loader
  stb_image.h
  image_loader.h
  image_loader.c
        Database/DatabaseEditor.cpp
        Database/DatabaseEditor.hpp
        Database/Project.cpp
        Database/Project.hpp
        Database/MapInfos.cpp
        Database/MapInfos.hpp
        Database/Map.cpp
        Database/Map.hpp)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(${NAME} PRIVATE project_warnings magic_enum imgui ImGuiFileDialog
                                      SDL2::SDL2)
