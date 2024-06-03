set(NAME "librpgm")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(
        ${NAME} STATIC
        # Image Loader
        stb_image.h image_loader.h image_loader.c
        # RPGMV Classes
        Database/Trait.hpp Database/Trait.hpp
        Database/Actors.hpp Database/Actors.cpp
        Database/Classes.hpp Database/Classes.cpp
        Database/Skills.hpp Database/Skills.cpp
        Database/Items.hpp Database/Items.cpp
        Database/Weapons.hpp Database/Weapons.cpp
        Database/Armors.hpp Database/Armors.cpp
        Database/Enemies.hpp Database/Enemies.cpp
        Database/Troops.hpp Database/Troops.cpp
        Database/States.hpp Database/States.cpp
        Database/Animations.hpp Database/Animations.cpp Database/Animation.hpp Database/Animation.cpp
        Database/Tilesets.hpp Database/Tilesets.cpp
        Database/CommonEvents.hpp Database/CommonEvents.cpp
        Database/System.hpp Database/System.cpp
        Database/Types.hpp Database/Types.cpp
        Database/Terms.hpp Database/Terms.cpp
        Database/MapInfos.hpp Database/MapInfos.cpp
        Database/RPGEnums.hpp

        # Runtime classes
        Runtime/Map.hpp Runtime/Map.cpp


        # Editor Classes
        Runtime/Project.hpp Runtime/Project.cpp
        Runtime/DatabaseEditor.cpp Runtime/DatabaseEditor.hpp
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(${NAME} PRIVATE project_warnings magic_enum imgui ImGuiFileDialog
        SDL2::SDL2)
