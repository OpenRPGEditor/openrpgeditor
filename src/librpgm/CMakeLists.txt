set(NAME "librpgm")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

add_library(
        ${NAME} STATIC
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
        Database/Messages.hpp Database/Messages.cpp
        Database/Terms.hpp Database/Terms.cpp
        Database/System.hpp Database/System.cpp
        Database/MapInfos.hpp Database/MapInfos.cpp
        Database/Globals.hpp Database/Globals.cpp
        Database/Vehicle.hpp Database/Vehicle.cpp
        Database/Audio.hpp Database/Audio.cpp
        Database/Event.hpp Database/Event.cpp
        Database/EventPage.hpp Database/EventPage.cpp
        Database/Battler.hpp Database/Battler.cpp
        Database/Map.hpp Database/Map.cpp
        Database/CommandParser.hpp Database/CommandParser.cpp
        Database/MovementRoute.hpp Database/MovementRoute.cpp

        # Editor Classes
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} nlohmann)
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(${NAME} PRIVATE project_warnings magic_enum imgui ImGuiFileDialog
        SDL2::SDL2 nlohmann_json::nlohmann_json fmt::fmt-header-only spdlog::spdlog_header_only lcf)
