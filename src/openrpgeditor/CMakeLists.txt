set(NAME "OpenRPGEditor")

#include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

configure_file(App/ProjectInfo.cpp.in App/ProjectInfo.cpp @ONLY)

add_library(orecore STATIC
        Core/Log.hpp Core/Log.cpp
        Core/Debug/Instrumentor.hpp
        Core/Application.hpp Core/Application.cpp
        Core/Window.hpp Core/Window.cpp
        Core/Project.hpp Core/Project.cpp
        Core/stb_image.h Core/image_loader.h Core/image_loader.c
        Core/ResourceManager.hpp Core/ResourceManager.cpp
        Core/Texture.hpp Core/Texture.cpp
        Core/ImGuiUtils.hpp Core/ImGuiUtils.cpp
        Core/TilesetPicker.hpp Core/TilesetPicker.cpp

        Core/MapListView.hpp Core/MapListView.cpp
        Core/EventListView.hpp Core/EventListView.cpp

        Core/MapEditor.hpp Core/MapEditor.cpp
        Core/MapEditor/MapEvent.hpp Core/MapEditor/MapEvent.cpp
        Core/MapEditor/MapCursor.hpp Core/MapEditor/MapCursor.cpp
        Core/MapEditor/MapRenderer.hpp Core/MapEditor/MapRenderer.cpp

        Core/EventEditor.cpp Core/EventEditor.cpp
        Core/EventEditor/EVPage.hpp Core/EventEditor/EVPage.cpp
        Core/DatabaseEditor.cpp Core/DatabaseEditor.hpp
        Core/DatabaseEditor/DBActorsTab.hpp Core/DatabaseEditor/DBActorsTab.cpp
        Core/DatabaseEditor/DBClassesTab.hpp Core/DatabaseEditor/DBClassesTab.cpp
        Core/DatabaseEditor/DBSkillsTab.hpp Core/DatabaseEditor/DBSkillsTab.cpp
        Core/DatabaseEditor/DBItemsTab.hpp Core/DatabaseEditor/DBItemsTab.cpp
        Core/DatabaseEditor/DBWeaponsTab.hpp Core/DatabaseEditor/DBWeaponsTab.cpp
        Core/DatabaseEditor/DBArmorsTab.hpp Core/DatabaseEditor/DBArmorsTab.cpp
        Core/DatabaseEditor/DBEnemiesTab.hpp Core/DatabaseEditor/DBEnemiesTab.cpp
        Core/DatabaseEditor/DBTroopsTab.hpp Core/DatabaseEditor/DBTroopsTab.cpp
        Core/DatabaseEditor/DBStatesTab.hpp Core/DatabaseEditor/DBStatesTab.cpp
        Core/DatabaseEditor/DBAnimationsTab.hpp Core/DatabaseEditor/DBAnimationsTab.cpp
        Core/DatabaseEditor/DBTilesetsTab.hpp Core/DatabaseEditor/DBTilesetsTab.cpp
        Core/DatabaseEditor/DBCommonEventsTab.hpp Core/DatabaseEditor/DBCommonEventsTab.cpp
        Core/DatabaseEditor/DBSystemTab.hpp Core/DatabaseEditor/DBSystemTab.cpp

        Core/CommonUI/TraitsEditor.hpp Core/CommonUI/TraitsEditor.cpp
        Core/CommonUI/ObjectPicker.hpp Core/CommonUI/ObjectPicker.inl
        Core/CommonUI/VariableSwitchPicker.hpp Core/CommonUI/VariableSwitchPicker.cpp
        Core/CommonUI/EventCommandEditor.hpp Core/CommonUI/EventCommandEditor.cpp
        Core/CommonUI/TextEditor.hpp Core/CommonUI/TextEditor.cpp
        Core/UndoStack.hpp Core/UndoStack.cpp

        Core/CheckerboardTexture.hpp Core/CheckerboardTexture.cpp
        Core/CharacterSheet.hpp Core/CharacterSheet.cpp
        Core/Settings.hpp Core/Settings.cpp
        Core/ImGuiParsedText.hpp Core/ImGuiParsedText.cpp
        Core/TextParser.hpp Core/TextParser.cpp
        Core/Resources.hpp
        Core/DPIHandler.hpp
        Core/EventCommands/IEventDialogController.hpp
        Core/EventCommands/Dialog_ControlSwitches.cpp
        Core/EventCommands/Dialog_ControlSwitches.hpp
        Core/EventCommands/Dialog_ControlVariables.cpp
        Core/EventCommands/Dialog_ControlVariables.hpp
        Core/EventCommands/Dialog_GameData.cpp
        Core/EventCommands/Dialog_GameData.hpp
        Core/EventCommands/Dialog_CommonEvent.cpp
        Core/EventCommands/Dialog_CommonEvent.hpp
        Core/EventCommands/Dialog_Script.cpp
        Core/EventCommands/Dialog_Script.hpp
        Core/DatabaseEditor/DBGameConstantsTab.cpp
        Core/DatabaseEditor/DBGameConstantsTab.hpp
        Core/EventCommands/Dialog_Comment.cpp
        Core/EventCommands/Dialog_Comment.hpp
        Core/EventCommands/Dialog_ChangeEncounter.cpp
        Core/EventCommands/Dialog_ChangeEncounter.hpp
        Core/EventCommands/Dialog_ChangeSaveAccess.cpp
        Core/EventCommands/Dialog_ChangeSaveAccess.hpp
        Core/EventCommands/Dialog_ChangeMenuAccess.cpp
        Core/EventCommands/Dialog_ChangeMenuAccess.hpp
        Core/EventCommands/Dialog_ChangeFormationAccess.cpp
        Core/EventCommands/Dialog_ChangeFormationAccess.hpp
        Core/EventCommands/Dialog_GameOver.hpp
        Core/EventCommands/Dialog_OpenSaveScreen.hpp
        Core/EventCommands/Dialog_GameOver.cpp
        Core/EventCommands/Dialog_OpenSaveScreen.cpp
        Core/EventCommands/Dialog_ReturnToTitle.cpp
        Core/EventCommands/Dialog_ReturnToTitle.hpp
        Core/EventCommands/Dialog_SaveBGM.cpp
        Core/EventCommands/Dialog_SaveBGM.hpp
        Core/EventCommands/Dialog_ReplayBGM.cpp
        Core/EventCommands/Dialog_ReplayBGM.hpp
        Core/EventCommands/Dialog_StopSE.cpp
        Core/EventCommands/Dialog_StopSE.hpp
        Core/EventCommands/Dialog_Loop.cpp
        Core/EventCommands/Dialog_Loop.hpp
        Core/EventCommands/Dialog_EraseEvent.cpp
        Core/EventCommands/Dialog_EraseEvent.hpp
        Core/EventCommands/Dialog_BreakLoop.cpp
        Core/EventCommands/Dialog_BreakLoop.hpp
        Core/EventCommands/Dialog_ExitEventProcessing.cpp
        Core/EventCommands/Dialog_ExitEventProcessing.hpp
        Core/EventCommands/Dialog_GetOnOffVehicle.cpp
        Core/EventCommands/Dialog_GetOnOffVehicle.hpp
        Core/EventCommands/Dialog_AbortBattle.cpp
        Core/EventCommands/Dialog_AbortBattle.hpp
        Core/EventCommands/Dialog_GatherFollowers.cpp
        Core/EventCommands/Dialog_GatherFollowers.hpp
        Core/EventCommands/Dialog_OpenMenuScreen.cpp
        Core/EventCommands/Dialog_OpenMenuScreen.hpp
        Core/ImGuiNotify.hpp
        Core/EventCommands/Dialog_PlayBGM.cpp
        Core/EventCommands/Dialog_PlayBGM.hpp
        Core/EventCommands/Dialog_ChangeBattleBGM.cpp
        Core/EventCommands/Dialog_ChangeBattleBGM.hpp
        Core/EventCommands/Dialog_ChangeVehicleBGM.cpp
        Core/EventCommands/Dialog_ChangeVehicleBGM.hpp
        Core/EventCommands/Dialog_ChangeVictoryME.cpp
        Core/EventCommands/Dialog_ChangeVictoryME.hpp
        Core/EventCommands/Dialog_ChangeDefeatME.cpp
        Core/EventCommands/Dialog_ChangeDefeatME.hpp
        Core/EventCommands/Dialog_PlayBGS.cpp
        Core/EventCommands/Dialog_PlayBGS.hpp
        Core/EventCommands/Dialog_PlaySE.cpp
        Core/EventCommands/Dialog_PlaySE.hpp
        Core/EventCommands/Dialog_PlayME.cpp
        Core/EventCommands/Dialog_PlayME.hpp
        Core/EventCommands/Dialog_ChangeBattleBack.cpp
        Core/EventCommands/Dialog_ChangeBattleBack.hpp
        Core/EventCommands/Dialog_ChangeWindowColor.cpp
        Core/EventCommands/Dialog_ChangeWindowColor.hpp
        Core/EventCommands/Dialog_ChangeGold.cpp
        Core/EventCommands/Dialog_ChangeGold.hpp
        Core/EventCommands/Dialog_ChangeEnemyState.cpp
        Core/EventCommands/Dialog_ChangeEnemyState.hpp
        Core/EventCommands/Dialog_RecoverAll.cpp
        Core/EventCommands/Dialog_RecoverAll.hpp
        Core/EventCommands/Dialog_ChangePartyMember.cpp
        Core/EventCommands/Dialog_ChangePartyMember.hpp
        Core/EventCommands/Dialog_EnemyRecoverAll.cpp
        Core/EventCommands/Dialog_EnemyRecoverAll.hpp
        Core/EventCommands/Dialog_ChangeNickname.cpp
        Core/EventCommands/Dialog_ChangeNickname.hpp
        Core/EventCommands/Dialog_EnemyAppear.cpp
        Core/EventCommands/Dialog_EnemyAppear.hpp
        Core/EventCommands/Dialog_ChangeProfile.cpp
        Core/EventCommands/Dialog_ChangeProfile.hpp
        Core/EventCommands/Dialog_EnemyTransform.cpp
        Core/EventCommands/Dialog_EnemyTransform.hpp
        Core/EventCommands/Dialog_ChangeEquipment.cpp
        Core/EventCommands/Dialog_ChangeEquipment.hpp
        Core/EventCommands/Dialog_ChangeName.cpp
        Core/EventCommands/Dialog_ChangeName.hpp
        Core/EventCommands/Dialog_ChangeClass.cpp
        Core/EventCommands/Dialog_ChangeClass.hpp
        Core/EventCommands/Dialog_PlayMovie.cpp
        Core/EventCommands/Dialog_PlayMovie.hpp
        Core/EventCommands/Dialog_Wait.cpp
        Core/EventCommands/Dialog_Wait.hpp
        Core/EventCommands/Dialog_TimerControl.cpp
        Core/EventCommands/Dialog_TimerControl.hpp
        Core/EventCommands/Dialog_Label.cpp
        Core/EventCommands/Dialog_Label.hpp
        Core/EventCommands/Dialog_JumpToLabel.cpp
        Core/EventCommands/Dialog_JumpToLabel.hpp
        Core/EventCommands/Dialog_ShowAnimation.cpp
        Core/EventCommands/Dialog_ShowAnimation.hpp
        Core/EventCommands/Dialog_ShowBattleAnimation.cpp
        Core/EventCommands/Dialog_ShowBattleAnimation.hpp
        Core/EventCommands/Dialog_ShowBalloonIcon.cpp
        Core/EventCommands/Dialog_ShowBalloonIcon.hpp
        Core/EventCommands/Dialog_InputNumber.cpp
        Core/EventCommands/Dialog_InputNumber.hpp
        Core/EventCommands/Dialog_SelectItem.cpp
        Core/EventCommands/Dialog_SelectItem.hpp
        Core/EventCommands/Dialog_NameInputProcessing.cpp
        Core/EventCommands/Dialog_NameInputProcessing.hpp
        Core/EventCommands/Dialog_ScrollMap.cpp
        Core/EventCommands/Dialog_ScrollMap.hpp
        Core/EventCommands/Dialog_RotatePicture.cpp
        Core/EventCommands/Dialog_RotatePicture.hpp
        Core/EventCommands/Dialog_ErasePicture.cpp
        Core/EventCommands/Dialog_ErasePicture.hpp
        Core/EventCommands/Dialog_ChangeTileset.cpp
        Core/EventCommands/Dialog_ChangeTileset.hpp
        Core/EventCommands/Dialog_FadeoutBGM.cpp
        Core/EventCommands/Dialog_FadeoutBGM.hpp
        Core/EventCommands/Dialog_FadeoutBGS.cpp
        Core/EventCommands/Dialog_FadeoutBGS.hpp
        Core/EventCommands/Dialog_ControlSelfSwitch.cpp
        Core/EventCommands/Dialog_ControlSelfSwitch.hpp
        Core/EventCommands/Dialog_ChangeTransparency.cpp
        Core/EventCommands/Dialog_ChangeTransparency.hpp
        Core/EventCommands/Dialog_TintScreen.cpp
        Core/EventCommands/Dialog_TintScreen.hpp
        Core/EventCommands/Dialog_SetWeatherEffect.cpp
        Core/EventCommands/Dialog_SetWeatherEffect.hpp
        Core/EventCommands/Dialog_FlashScreen.cpp
        Core/EventCommands/Dialog_FlashScreen.hpp
        Core/EventCommands/Dialog_ShakeScreen.cpp
        Core/EventCommands/Dialog_ShakeScreen.hpp
        Core/EventCommands/Dialog_TintPicture.cpp
        Core/EventCommands/Dialog_TintPicture.hpp
        Core/EventCommands/Dialog_FadeinScreen.cpp
        Core/EventCommands/Dialog_FadeinScreen.hpp
        Core/EventCommands/Dialog_FadeoutScreen.cpp
        Core/EventCommands/Dialog_FadeoutScreen.hpp
        Core/EventCommands/Dialog_ChangeMapDisplayName.cpp
        Core/EventCommands/Dialog_ChangeMapDisplayName.hpp
        Core/EventCommands/Dialog_ChangePlayerFollowers.cpp
        Core/EventCommands/Dialog_ChangePlayerFollowers.hpp
        Core/EventCommands/Dialog_ShowPicture.cpp
        Core/EventCommands/Dialog_ShowPicture.hpp
        Core/CommonUI/CharacterPicker.cpp
        Core/CommonUI/CharacterPicker.hpp
        Core/EventCommands/Dialog_MovePicture.cpp
        Core/EventCommands/Dialog_MovePicture.hpp
        Core/EventCommands/Dialog_ChangeParallax.cpp
        Core/EventCommands/Dialog_ChangeParallax.hpp
        Core/EventCommands/Dialog_BattleProcessing.cpp
        Core/EventCommands/Dialog_BattleProcessing.hpp
        Core/EventCommands/Dialog_ChangeSkill.cpp
        Core/EventCommands/Dialog_ChangeSkill.hpp
        Core/EventCommands/Dialog_ChangeHP.cpp
        Core/EventCommands/Dialog_ChangeHP.hpp
        Core/CommonUI/IDialogController.hpp
        Core/EventCommands/Dialog_ChangeEnemyHP.cpp
        Core/EventCommands/Dialog_ChangeEnemyHP.hpp
        Core/EventCommands/Dialog_ChangeMP.cpp
        Core/EventCommands/Dialog_ChangeMP.hpp
        Core/EventCommands/Dialog_ChangeEnemyMP.cpp
        Core/EventCommands/Dialog_ChangeEnemyMP.hpp
        Core/EventCommands/Dialog_ChangeTP.cpp
        Core/EventCommands/Dialog_ChangeTP.hpp
        Core/EventCommands/Dialog_ChangeEnemyTP.cpp
        Core/EventCommands/Dialog_ChangeEnemyTP.hpp
        Core/EventCommands/Dialog_ChangeEXP.cpp
        Core/EventCommands/Dialog_ChangeEXP.hpp
        Core/EventCommands/Dialog_ChangeArmors.cpp
        Core/EventCommands/Dialog_ChangeArmors.hpp
        Core/EventCommands/Dialog_ChangeWeapons.cpp
        Core/EventCommands/Dialog_ChangeWeapons.hpp
        Core/EventCommands/Dialog_ChangeItems.cpp
        Core/EventCommands/Dialog_ChangeItems.hpp
        Core/EventCommands/Dialog_ChangeState.cpp
        Core/EventCommands/Dialog_ChangeState.hpp
        Core/EventCommands/Dialog_ChangeLevel.cpp
        Core/EventCommands/Dialog_ChangeLevel.hpp
        Core/EventCommands/Dialog_ChangeParameter.cpp
        Core/EventCommands/Dialog_ChangeParameter.hpp
        Core/EventCommands/Dialog_TransferPlayer.cpp
        Core/EventCommands/Dialog_TransferPlayer.hpp
        Core/EventCommands/Dialog_SetEventLocation.cpp
        Core/EventCommands/Dialog_SetEventLocation.hpp
        Core/EventCommands/Dialog_SetVehicleLocation.cpp
        Core/EventCommands/Dialog_SetVehicleLocation.hpp
        Core/EventCommands/Dialog_SetMovementRoute.cpp
        Core/EventCommands/Dialog_SetMovementRoute.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementScript.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementScript.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementJump.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementJump.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementWait.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementWait.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementSwitchON.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementSwitchON.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementSwitchOFF.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementSwitchOFF.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeFrequency.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeFrequency.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeSpeed.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeSpeed.hpp

        Core/SideViewBattlerSheet.cpp
        Core/SideViewBattlerSheet.hpp
        Core/FaceSheet.cpp
        Core/FaceSheet.hpp
)

target_include_directories(orecore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SFML_SOURCE_DIR}/include/SFML  ${CLIP_INCLUDES})
target_compile_features(orecore PRIVATE cxx_std_20)
target_link_libraries(
        orecore
        PRIVATE project_warnings
        PUBLIC fmt::fmt-header-only spdlog::spdlog_header_only
        SDL2::SDL2
        imgui
        nfd
        magic_enum
        nlohmann_json::nlohmann_json
        librpgm
        freetype
        z
        clip
        sfml-audio
)

# Define set of OS specific files to include
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(orecore PRIVATE Platform/Windows/Resources.cpp
            Platform/Windows/DPIHandler.cpp)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_sources(orecore PRIVATE Platform/Mac/Resources.cpp
            Platform/Mac/DPIHandler.cpp)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(orecore PRIVATE Platform/Linux/Resources.cpp
            Platform/Linux/DPIHandler.cpp)
endif ()

add_executable(${NAME} WIN32 MACOSX_BUNDLE
        App/Main.cpp
        App/ProjectInfo.hpp App/ProjectInfo.cpp
)



include(${PROJECT_SOURCE_DIR}/src/openrpgeditor/cmake/AppAssets.cmake)
include(${PROJECT_SOURCE_DIR}/src/openrpgeditor/cmake/Packaging.cmake)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SFML_SOURCE_DIR}/include/SFML ${CLIP_INCLUDES})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(
        ${NAME}
        PRIVATE project_warnings
        PUBLIC fmt::fmt-header-only spdlog::spdlog_header_only
        orecore
        SDL2::SDL2
        imgui
        nfd
        magic_enum
        nlohmann_json::nlohmann_json
        librpgm
        librpgmeventcmds
        librpgmmovementroutecmds
        freetype
        z
        sfml-audio
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebugInfo")
    add_compile_definitions(
            ZLIB_DEBUG
    )
endif ()
