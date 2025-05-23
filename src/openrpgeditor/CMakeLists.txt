# include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

configure_file(App/ProjectInfo.cpp.in App/ProjectInfo.cpp @ONLY)

add_subdirectory(Math)

#add_library(CommonUI INTERFACE #STATIC
#        Core/CommonUI/IWidget.hpp
#)

#target_include_directories(
#        CommonUI
#        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
#        ${SFML_SOURCE_DIR}/include/SFML
#        ${clip_SOURCE_DIR}
#        ${athena_SOURCE_DIR}/include
#        ${cpuid_SOURCE_DIR}
#        ${CMAKE_SOURCE_DIR}/extern/angelscript/angelscript/include
#        ${CMAKE_SOURCE_DIR}/extern/inih
#        ${CMAKE_SOURCE_DIR}/extern/angelscript/add_on
#        "${CMAKE_SOURCE_DIR}/extern/nano-signal-slot"
#)
#target_compile_features(CommonUI PRIVATE cxx_std_23)
#target_link_libraries(
#        CommonUI
#        PUBLIC fmt::fmt-header-only
#        spdlog::spdlog
#        SDL3::SDL3
#        SDL3_image::SDL3_image
#        imgui-setup
#        magic_enum
#        nlohmann_json::nlohmann_json
#        librpgm
#        freetype
#        clip
#        sfml-audio
#        oremath
#        libcurl
#        cpuid
#        angelscript
#        angelscript_addons
#        athena-core
#        lcf
#        inih
#)

add_library(
        orecore STATIC
        Core/Application.cpp
        Core/Application.hpp
        Core/CommonUI/ActionsEditor.cpp
        Core/CommonUI/ActionsEditor.hpp
        Core/CommonUI/CharacterPicker.cpp
        Core/CommonUI/CharacterPicker.hpp
        Core/CommonUI/ColorFlashPicker.cpp
        Core/CommonUI/ColorFlashPicker.hpp
        Core/CommonUI/ColorTonePicker.cpp
        Core/CommonUI/ColorTonePicker.hpp
        Core/CommonUI/CreateNewProjectDialog.cpp
        Core/CommonUI/CreateNewProjectDialog.hpp
        Core/CommonUI/EffectsEditor.cpp
        Core/CommonUI/EffectsEditor.hpp
        Core/CommonUI/EventCommandEditor.cpp
        Core/CommonUI/EventCommandEditor.hpp
        Core/CommonUI/GameWindowColorPicker.cpp
        Core/CommonUI/GameWindowColorPicker.hpp
        Core/CommonUI/GridAtlasPicker.cpp
        Core/CommonUI/GridAtlasPicker.hpp
        Core/CommonUI/IDialogController.hpp
        Core/CommonUI/ImagePicker.cpp
        Core/CommonUI/ImagePicker.hpp
        Core/CommonUI/ObjectPicker.hpp
        Core/CommonUI/ObjectPicker.inl
        Core/CommonUI/TextEditor.cpp
        Core/CommonUI/TextEditor.hpp
        Core/CommonUI/TilesetPicker.cpp
        Core/CommonUI/TilesetPicker.hpp
        Core/CommonUI/TraitsEditor.cpp
        Core/CommonUI/TraitsEditor.hpp
        Core/CommonUI/VariableSwitchPicker.cpp
        Core/CommonUI/VariableSwitchPicker.hpp
        Core/CommonUI/Directory.cpp
        Core/CommonUI/Directory.hpp
        Core/CommonUI/FacePicker.cpp
        Core/CommonUI/FacePicker.hpp
        Core/DatabaseEditor.cpp
        Core/DatabaseEditor.hpp
        Core/DatabaseEditor/DBActorsTab.cpp
        Core/DatabaseEditor/DBActorsTab.hpp
        Core/DatabaseEditor/DBAnimationsTab.cpp
        Core/DatabaseEditor/DBAnimationsTab.hpp
        Core/DatabaseEditor/DBArmorsTab.cpp
        Core/DatabaseEditor/DBArmorsTab.hpp
        Core/DatabaseEditor/DBClassesTab.cpp
        Core/DatabaseEditor/DBClassesTab.hpp
        Core/DatabaseEditor/DBCommonEventsTab.cpp
        Core/DatabaseEditor/DBCommonEventsTab.hpp
        Core/DatabaseEditor/DBEnemiesTab.cpp
        Core/DatabaseEditor/DBEnemiesTab.hpp
        Core/DatabaseEditor/DBGameConstantsTab.cpp
        Core/DatabaseEditor/DBGameConstantsTab.hpp
        Core/DatabaseEditor/DBItemsTab.cpp
        Core/DatabaseEditor/DBItemsTab.hpp
        Core/DatabaseEditor/DBSkillsTab.cpp
        Core/DatabaseEditor/DBSkillsTab.hpp
        Core/DatabaseEditor/DBStatesTab.cpp
        Core/DatabaseEditor/DBStatesTab.hpp
        Core/DatabaseEditor/DBSystemTab.cpp
        Core/DatabaseEditor/DBSystemTab.hpp
        Core/DatabaseEditor/DBTemplatesTab.cpp
        Core/DatabaseEditor/DBTemplatesTab.hpp
        Core/DatabaseEditor/DBTermsTab.cpp
        Core/DatabaseEditor/DBTermsTab.hpp
        Core/DatabaseEditor/DBTilesetsTab.cpp
        Core/DatabaseEditor/DBTilesetsTab.hpp
        Core/DatabaseEditor/DBTroopsTab.cpp
        Core/DatabaseEditor/DBTroopsTab.hpp
        Core/DatabaseEditor/DBTypesTab.cpp
        Core/DatabaseEditor/DBTypesTab.hpp
        Core/DatabaseEditor/DBWeaponsTab.cpp
        Core/DatabaseEditor/DBWeaponsTab.hpp
        Core/Debug/Instrumentor.hpp
        Core/DownloadManager.cpp
        Core/DownloadManager.cpp
        Core/DownloadManager.hpp
        Core/DownloadManager.hpp
        Core/EventCommands/Dialog_AbortBattle.cpp
        Core/EventCommands/Dialog_AbortBattle.hpp
        Core/EventCommands/Dialog_BattleProcessing.cpp
        Core/EventCommands/Dialog_BattleProcessing.hpp
        Core/EventCommands/Dialog_BreakLoop.cpp
        Core/EventCommands/Dialog_BreakLoop.hpp
        Core/EventCommands/Dialog_ChangeActorImages.cpp
        Core/EventCommands/Dialog_ChangeActorImages.hpp
        Core/EventCommands/Dialog_ChangeArmors.cpp
        Core/EventCommands/Dialog_ChangeArmors.hpp
        Core/EventCommands/Dialog_ChangeBattleBack.cpp
        Core/EventCommands/Dialog_ChangeBattleBack.hpp
        Core/EventCommands/Dialog_ChangeBattleBGM.cpp
        Core/EventCommands/Dialog_ChangeBattleBGM.hpp
        Core/EventCommands/Dialog_ChangeClass.cpp
        Core/EventCommands/Dialog_ChangeClass.hpp
        Core/EventCommands/Dialog_ChangeDefeatME.cpp
        Core/EventCommands/Dialog_ChangeDefeatME.hpp
        Core/EventCommands/Dialog_ChangeEncounter.cpp
        Core/EventCommands/Dialog_ChangeEncounter.hpp
        Core/EventCommands/Dialog_ChangeEnemyHP.cpp
        Core/EventCommands/Dialog_ChangeEnemyHP.hpp
        Core/EventCommands/Dialog_ChangeEnemyMP.cpp
        Core/EventCommands/Dialog_ChangeEnemyMP.hpp
        Core/EventCommands/Dialog_ChangeEnemyState.cpp
        Core/EventCommands/Dialog_ChangeEnemyState.hpp
        Core/EventCommands/Dialog_ChangeEnemyTP.cpp
        Core/EventCommands/Dialog_ChangeEnemyTP.hpp
        Core/EventCommands/Dialog_ChangeEquipment.cpp
        Core/EventCommands/Dialog_ChangeEquipment.hpp
        Core/EventCommands/Dialog_ChangeEXP.cpp
        Core/EventCommands/Dialog_ChangeEXP.hpp
        Core/EventCommands/Dialog_ChangeFormationAccess.cpp
        Core/EventCommands/Dialog_ChangeFormationAccess.hpp
        Core/EventCommands/Dialog_ChangeGold.cpp
        Core/EventCommands/Dialog_ChangeGold.hpp
        Core/EventCommands/Dialog_ChangeHP.cpp
        Core/EventCommands/Dialog_ChangeHP.hpp
        Core/EventCommands/Dialog_ChangeItems.cpp
        Core/EventCommands/Dialog_ChangeItems.hpp
        Core/EventCommands/Dialog_ChangeLevel.cpp
        Core/EventCommands/Dialog_ChangeLevel.hpp
        Core/EventCommands/Dialog_ChangeMapDisplayName.cpp
        Core/EventCommands/Dialog_ChangeMapDisplayName.hpp
        Core/EventCommands/Dialog_ChangeMenuAccess.cpp
        Core/EventCommands/Dialog_ChangeMenuAccess.hpp
        Core/EventCommands/Dialog_ChangeMP.cpp
        Core/EventCommands/Dialog_ChangeMP.hpp
        Core/EventCommands/Dialog_ChangeName.cpp
        Core/EventCommands/Dialog_ChangeName.hpp
        Core/EventCommands/Dialog_ChangeNickname.cpp
        Core/EventCommands/Dialog_ChangeNickname.hpp
        Core/EventCommands/Dialog_ChangeParallax.cpp
        Core/EventCommands/Dialog_ChangeParallax.hpp
        Core/EventCommands/Dialog_ChangeParameter.cpp
        Core/EventCommands/Dialog_ChangeParameter.hpp
        Core/EventCommands/Dialog_ChangePartyMember.cpp
        Core/EventCommands/Dialog_ChangePartyMember.hpp
        Core/EventCommands/Dialog_ChangePlayerFollowers.cpp
        Core/EventCommands/Dialog_ChangePlayerFollowers.hpp
        Core/EventCommands/Dialog_ChangePluginCommand.cpp
        Core/EventCommands/Dialog_ChangePluginCommand.hpp
        Core/EventCommands/Dialog_ChangeProfile.cpp
        Core/EventCommands/Dialog_ChangeProfile.hpp
        Core/EventCommands/Dialog_ChangeSaveAccess.cpp
        Core/EventCommands/Dialog_ChangeSaveAccess.hpp
        Core/EventCommands/Dialog_ChangeSkill.cpp
        Core/EventCommands/Dialog_ChangeSkill.hpp
        Core/EventCommands/Dialog_ChangeState.cpp
        Core/EventCommands/Dialog_ChangeState.hpp
        Core/EventCommands/Dialog_ChangeTileset.cpp
        Core/EventCommands/Dialog_ChangeTileset.hpp
        Core/EventCommands/Dialog_ChangeTP.cpp
        Core/EventCommands/Dialog_ChangeTP.hpp
        Core/EventCommands/Dialog_ChangeTransparency.cpp
        Core/EventCommands/Dialog_ChangeTransparency.hpp
        Core/EventCommands/Dialog_ChangeVehicleBGM.cpp
        Core/EventCommands/Dialog_ChangeVehicleBGM.hpp
        Core/EventCommands/Dialog_ChangeVehicleImage.cpp
        Core/EventCommands/Dialog_ChangeVehicleImage.hpp
        Core/EventCommands/Dialog_ChangeVictoryME.cpp
        Core/EventCommands/Dialog_ChangeVictoryME.hpp
        Core/EventCommands/Dialog_ChangeWeapons.cpp
        Core/EventCommands/Dialog_ChangeWeapons.hpp
        Core/EventCommands/Dialog_ChangeWindowColor.cpp
        Core/EventCommands/Dialog_ChangeWindowColor.hpp
        Core/EventCommands/Dialog_Comment.cpp
        Core/EventCommands/Dialog_Comment.hpp
        Core/EventCommands/Dialog_CommonEvent.cpp
        Core/EventCommands/Dialog_CommonEvent.hpp
        Core/EventCommands/Dialog_ConditionalBranch.cpp
        Core/EventCommands/Dialog_ConditionalBranch.hpp
        Core/EventCommands/Dialog_ControlSelfSwitch.cpp
        Core/EventCommands/Dialog_ControlSelfSwitch.hpp
        Core/EventCommands/Dialog_ControlSwitches.cpp
        Core/EventCommands/Dialog_ControlSwitches.hpp
        Core/EventCommands/Dialog_ControlVariables.cpp
        Core/EventCommands/Dialog_ControlVariables.hpp
        Core/EventCommands/Dialog_EnemyAppear.cpp
        Core/EventCommands/Dialog_EnemyAppear.hpp
        Core/EventCommands/Dialog_EnemyRecoverAll.cpp
        Core/EventCommands/Dialog_EnemyRecoverAll.hpp
        Core/EventCommands/Dialog_EnemyTransform.cpp
        Core/EventCommands/Dialog_EnemyTransform.hpp
        Core/EventCommands/Dialog_EraseEvent.cpp
        Core/EventCommands/Dialog_EraseEvent.hpp
        Core/EventCommands/Dialog_ErasePicture.cpp
        Core/EventCommands/Dialog_ErasePicture.hpp
        Core/EventCommands/Dialog_ExitEventProcessing.cpp
        Core/EventCommands/Dialog_ExitEventProcessing.hpp
        Core/EventCommands/Dialog_FadeinScreen.cpp
        Core/EventCommands/Dialog_FadeinScreen.hpp
        Core/EventCommands/Dialog_FadeoutBGM.cpp
        Core/EventCommands/Dialog_FadeoutBGM.hpp
        Core/EventCommands/Dialog_FadeoutBGS.cpp
        Core/EventCommands/Dialog_FadeoutBGS.hpp
        Core/EventCommands/Dialog_FadeoutScreen.cpp
        Core/EventCommands/Dialog_FadeoutScreen.hpp
        Core/EventCommands/Dialog_FlashScreen.cpp
        Core/EventCommands/Dialog_FlashScreen.hpp
        Core/EventCommands/Dialog_GameData.cpp
        Core/EventCommands/Dialog_GameData.hpp
        Core/EventCommands/Dialog_GameOver.cpp
        Core/EventCommands/Dialog_GameOver.hpp
        Core/EventCommands/Dialog_GatherFollowers.cpp
        Core/EventCommands/Dialog_GatherFollowers.hpp
        Core/EventCommands/Dialog_GetOnOffVehicle.cpp
        Core/EventCommands/Dialog_GetOnOffVehicle.hpp
        Core/EventCommands/Dialog_InputNumber.cpp
        Core/EventCommands/Dialog_InputNumber.hpp
        Core/EventCommands/Dialog_JumpToLabel.cpp
        Core/EventCommands/Dialog_JumpToLabel.hpp
        Core/EventCommands/Dialog_Label.cpp
        Core/EventCommands/Dialog_Label.hpp
        Core/EventCommands/Dialog_Loop.cpp
        Core/EventCommands/Dialog_Loop.hpp
        Core/EventCommands/Dialog_MovePicture.cpp
        Core/EventCommands/Dialog_MovePicture.hpp
        Core/EventCommands/Dialog_NameInputProcessing.cpp
        Core/EventCommands/Dialog_NameInputProcessing.hpp
        Core/EventCommands/Dialog_OpenMenuScreen.cpp
        Core/EventCommands/Dialog_OpenMenuScreen.hpp
        Core/EventCommands/Dialog_OpenSaveScreen.cpp
        Core/EventCommands/Dialog_OpenSaveScreen.hpp
        Core/EventCommands/Dialog_PlayBGM.cpp
        Core/EventCommands/Dialog_PlayBGM.hpp
        Core/EventCommands/Dialog_PlayBGS.cpp
        Core/EventCommands/Dialog_PlayBGS.hpp
        Core/EventCommands/Dialog_PlayME.cpp
        Core/EventCommands/Dialog_PlayME.hpp
        Core/EventCommands/Dialog_PlayMovie.cpp
        Core/EventCommands/Dialog_PlayMovie.hpp
        Core/EventCommands/Dialog_PlaySE.cpp
        Core/EventCommands/Dialog_PlaySE.hpp
        Core/EventCommands/Dialog_RecoverAll.cpp
        Core/EventCommands/Dialog_RecoverAll.hpp
        Core/EventCommands/Dialog_ReplayBGM.cpp
        Core/EventCommands/Dialog_ReplayBGM.hpp
        Core/EventCommands/Dialog_ReturnToTitle.cpp
        Core/EventCommands/Dialog_ReturnToTitle.hpp
        Core/EventCommands/Dialog_RotatePicture.cpp
        Core/EventCommands/Dialog_RotatePicture.hpp
        Core/EventCommands/Dialog_SaveBGM.cpp
        Core/EventCommands/Dialog_SaveBGM.hpp
        Core/EventCommands/Dialog_Script.cpp
        Core/EventCommands/Dialog_Script.hpp
        Core/EventCommands/Dialog_ScrollMap.cpp
        Core/EventCommands/Dialog_ScrollMap.hpp
        Core/EventCommands/Dialog_SelectItem.cpp
        Core/EventCommands/Dialog_SelectItem.hpp
        Core/EventCommands/Dialog_SetEventLocation.cpp
        Core/EventCommands/Dialog_SetEventLocation.hpp
        Core/EventCommands/Dialog_SetMovementRoute.cpp
        Core/EventCommands/Dialog_SetMovementRoute.hpp
        Core/EventCommands/Dialog_SetVehicleLocation.cpp
        Core/EventCommands/Dialog_SetVehicleLocation.hpp
        Core/EventCommands/Dialog_SetWeatherEffect.cpp
        Core/EventCommands/Dialog_SetWeatherEffect.hpp
        Core/EventCommands/Dialog_ShakeScreen.cpp
        Core/EventCommands/Dialog_ShakeScreen.hpp
        Core/EventCommands/Dialog_ShopProcessing.cpp
        Core/EventCommands/Dialog_ShopProcessing.hpp
        Core/EventCommands/Dialog_ShopProcessing_Goods.cpp
        Core/EventCommands/Dialog_ShopProcessing_Goods.hpp
        Core/EventCommands/Dialog_ShowAnimation.cpp
        Core/EventCommands/Dialog_ShowAnimation.hpp
        Core/EventCommands/Dialog_ShowBalloonIcon.cpp
        Core/EventCommands/Dialog_ShowBalloonIcon.hpp
        Core/EventCommands/Dialog_ShowBattleAnimation.cpp
        Core/EventCommands/Dialog_ShowBattleAnimation.hpp
        Core/EventCommands/Dialog_ShowChoice.cpp
        Core/EventCommands/Dialog_ShowChoice.hpp
        Core/EventCommands/Dialog_ShowPicture.cpp
        Core/EventCommands/Dialog_ShowPicture.hpp
        Core/EventCommands/Dialog_ShowScrollingText.cpp
        Core/EventCommands/Dialog_ShowScrollingText.hpp
        Core/EventCommands/Dialog_ShowText.cpp
        Core/EventCommands/Dialog_ShowText.hpp
        Core/EventCommands/Dialog_StopSE.cpp
        Core/EventCommands/Dialog_StopSE.hpp
        Core/EventCommands/Dialog_ControlTimer.cpp
        Core/EventCommands/Dialog_ControlTimer.hpp
        Core/EventCommands/Dialog_TintPicture.cpp
        Core/EventCommands/Dialog_TintPicture.hpp
        Core/EventCommands/Dialog_TintScreen.cpp
        Core/EventCommands/Dialog_TintScreen.hpp
        Core/EventCommands/Dialog_TransferPlayer.cpp
        Core/EventCommands/Dialog_TransferPlayer.hpp
        Core/EventCommands/Dialog_Wait.cpp
        Core/EventCommands/Dialog_Wait.hpp
        Core/EventCommands/IEventDialogController.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeBlendMode.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeBlendMode.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeFrequency.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeFrequency.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeImage.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeImage.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeOpacity.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeOpacity.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeSpeed.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementChangeSpeed.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementJump.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementJump.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementScript.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementScript.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementSwitchOFF.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementSwitchOFF.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementSwitchON.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementSwitchON.hpp
        Core/EventCommands/MovementRoute/Dialog_MovementWait.cpp
        Core/EventCommands/MovementRoute/Dialog_MovementWait.hpp
        Core/EventEditor.cpp
        Core/EventEditor.hpp
        Core/EventEditor/EVPage.cpp
        Core/EventEditor/EVPage.hpp
        Core/EventListView.cpp
        Core/EventListView.hpp
        Core/FirstBootWizard.cpp
        Core/FirstBootWizard.hpp
        Core/FirstBootWizard/IWizardPage.hpp
        Core/FirstBootWizard/ProjectLocationPage.cpp
        Core/FirstBootWizard/ProjectLocationPage.hpp
        Core/FirstBootWizard/RPGMakerLocationAndVersionPage.cpp
        Core/FirstBootWizard/RPGMakerLocationAndVersionPage.hpp
        Core/FirstBootWizard/UISettingsPage.cpp
        Core/FirstBootWizard/UISettingsPage.hpp
        Core/FirstBootWizard/WelcomePage.cpp
        Core/FirstBootWizard/WelcomePage.hpp
        Core/Graphics/CharacterSheet.cpp
        Core/Graphics/CharacterSheet.hpp
        Core/Graphics/CheckerboardTexture.cpp
        Core/Graphics/CheckerboardTexture.hpp
        Core/Graphics/CheckeredCompositeTexture.cpp
        Core/Graphics/CheckeredCompositeTexture.hpp
        Core/Graphics/ColorTonePreview.cpp
        Core/Graphics/ColorTonePreview.hpp
        Core/Graphics/FaceSheet.cpp
        Core/Graphics/FaceSheet.hpp
        Core/Graphics/GameWindowBackground.cpp
        Core/Graphics/GameWindowBackground.hpp
        Core/Graphics/IconSheet.cpp
        Core/Graphics/IconSheet.hpp
        Core/Graphics/Image.cpp
        Core/Graphics/Image.hpp
        Core/Graphics/image_loader.cpp
        Core/Graphics/image_loader.h
        Core/Graphics/RenderImage.cpp
        Core/Graphics/RenderImage.hpp
        Core/Graphics/SideViewBattlerSheet.cpp
        Core/Graphics/SideViewBattlerSheet.hpp
        Core/Graphics/SimpleRect.hpp
        Core/Graphics/stb_image.h
        Core/Graphics/Texture.cpp
        Core/Graphics/Texture.hpp
        Core/Graphics/ParallaxTexture.cpp
        Core/Graphics/ParallaxTexture.hpp
        Core/ImGuiExt/ImGuiNotify.hpp
        Core/ImGuiExt/ImGuiParsedText.cpp
        Core/ImGuiExt/ImGuiParsedText.hpp
        Core/ImGuiExt/ImGuiUtils.cpp
        Core/ImGuiExt/ImGuiUtils.hpp
        Core/ImGuiExt/TextParser.cpp
        Core/ImGuiExt/TextParser.hpp
        Core/Log.cpp
        Core/Log.hpp
        Core/MapEditor.cpp
        Core/MapEditor.hpp
        Core/MapEditor/MapCursor.cpp
        Core/MapEditor/MapCursor.hpp
        Core/MapEditor/MapEvent.cpp
        Core/MapEditor/MapEvent.hpp
        Core/MapEditor/MapProperties.cpp
        Core/MapEditor/MapProperties.hpp
        Core/MapEditor/MapRenderer.cpp
        Core/MapEditor/MapRenderer.hpp
        Core/MapListView.cpp
        Core/MapListView.hpp
        Core/NWJSVersionManager.cpp
        Core/NWJSVersionManager.hpp
        Core/MainWindow.cpp
        Core/MainWindow.hpp
        Core/ResourceManager.cpp
        Core/ResourceManager.hpp
        Core/Resources.hpp
        Core/Settings.cpp
        Core/Settings.hpp
        Core/SettingsDialog.cpp
        Core/SettingsDialog.hpp
        Core/SettingsDialog/GeneralSettingsTab.cpp
        Core/SettingsDialog/GeneralSettingsTab.hpp
        Core/SettingsDialog/RPGMakerLocationAndVersionTab.cpp
        Core/SettingsDialog/RPGMakerLocationAndVersionTab.hpp
        Core/SettingsDialog/ISettingsTab.hpp
        Core/Sound.cpp
        Core/Sound.hpp
        Core/TemplateEditor/Dialog/TemplateName.cpp
        Core/TemplateEditor/Dialog/TemplateName.hpp
        Core/TemplateEditor/Dialog/TemplatesEvent.cpp
        Core/TemplateEditor/Dialog/TemplatesEvent.hpp
        Core/TemplateEditor/TemplatesCommand.cpp
        Core/TemplateEditor/TemplatesCommand.hpp
        Core/TemplateEditor/TemplatesEventProperties.cpp
        Core/TemplateEditor/TemplatesEventProperties.hpp
        Core/TemplateEditor/TemplatesTint.cpp
        Core/TemplateEditor/TemplatesTint.hpp
        Core/TroopsEventEditor/TroopsEVEditor.cpp
        Core/TroopsEventEditor/TroopsEVEditor.hpp
        Core/TroopsEventEditor/TroopsEVPage.cpp
        Core/TroopsEventEditor/TroopsEVPage.hpp
        Core/Tilemap/ITileView.cpp
        Core/Tilemap/ITileView.hpp
        Core/UndoStack.cpp
        Core/UndoStack.hpp
        Core/Window.cpp
        Core/Window.hpp
        Core/Tilemap/TilePalette.cpp
        Core/Tilemap/TilePalette.hpp
        Core/Tilemap/TileRenderHelper.cpp
        Core/Tilemap/TileRenderHelper.hpp
        Core/Graphics/TileMarker.cpp
        Core/Graphics/TileMarker.hpp
        Core/SettingsDialog/UISettingsTab.cpp
        Core/SettingsDialog/UISettingsTab.hpp
        Core/EditorPlugin/EditorPluginManager.cpp
        Core/EditorPlugin/EditorPluginManager.hpp
        Core/DirectoryMonitor.cpp
        Core/DirectoryMonitor.hpp
        Core/Script/ScriptEngine.cpp
        Core/Script/ScriptEngine.hpp
        Core/EditorPlugin/EditorPlugin.cpp
        Core/EditorPlugin/EditorPlugin.hpp
        Core/Script/ByteCodeStream.cpp
        Core/Script/ByteCodeStream.hpp
        Core/Script/Bindings.cpp
        Core/Script/Bindings.hpp
        Core/Tilemap/TilemapView.cpp
        Core/Tilemap/TilemapView.hpp
        Core/CommonUI/SoundPicker.cpp
        Core/CommonUI/SoundPicker.hpp
        Core/ImGuiExt/imgui-knobs.h
        Core/ImGuiExt/imgui-knobs.cpp
        Core/ImGuiExt/imgui_neo_internal.h
        Core/ImGuiExt/imgui_neo_internal.cpp
        Core/ImGuiExt/imgui_neo_sequencer.h
        Core/ImGuiExt/imgui_neo_sequencer.cpp
        Core/DatabaseEditor/DBDocTab.cpp
        Core/DatabaseEditor/DBDocTab.hpp
        Core/EventSearcher.cpp
        Core/EventSearcher.hpp
        Core/ReferenceSearch.cpp
        Core/ReferenceSearch.hpp
        Core/DatabaseEditor/DBMappingsTab.cpp
        Core/DatabaseEditor/DBMappingsTab.hpp
        Core/CommonUI/Preview.cpp
        Core/CommonUI/Preview.hpp
        Core/Interpreter.cpp
        Core/Interpreter.hpp
        Core/LibLCF.cpp
        Core/LibLCF.hpp
        Core/DatabaseEditor/DBLocaleTab.cpp
        Core/DatabaseEditor/DBLocaleTab.hpp

        Core/Script/ImGui/ImGuiMath.cpp
        Core/Script/ImGui/ImGuiMath.hpp
        Core/Script/ImGui/ImGuiCoreOperations.cpp
        Core/Script/ImGui/ImGuiCoreOperations.hpp
        Core/LCF_Importer/LCF_Importer.cpp
        Core/LCF_Importer/LCF_Importer.hpp
        Core/LCF_Importer/LCF_Mapping.cpp
        Core/LCF_Importer/LCF_Mapping.hpp
        Core/EventCommands/MovementRoute/Dialogue_MovementPlaySE.cpp
        Core/EventCommands/MovementRoute/Dialogue_MovementPlaySE.hpp
        Core/CommonUI/GroupBox.cpp
        Core/CommonUI/GroupBox.hpp
        Core/CommonUI/AudioEditor.cpp
        Core/CommonUI/AudioEditor.hpp
        Core/CommonUI/EventTilePicker.cpp
        Core/CommonUI/EventTilePicker.hpp
)

target_include_directories(
        orecore
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${SFML_SOURCE_DIR}/include/SFML
        ${clip_SOURCE_DIR}
        ${athena_SOURCE_DIR}/include
        ${cpuid_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/extern/angelscript/angelscript/include
        ${CMAKE_SOURCE_DIR}/extern/inih
        ${CMAKE_SOURCE_DIR}/extern/angelscript/add_on
        "${CMAKE_SOURCE_DIR}/extern/nano-signal-slot"
)
target_compile_features(orecore PRIVATE cxx_std_23)
target_link_libraries(
        orecore
        PRIVATE
        fmt::fmt-header-only
        spdlog::spdlog_header_only
        PUBLIC
        # CommonUI
        SDL3::SDL3
        SDL3_image::SDL3_image
        imgui-setup
        magic_enum
        nlohmann_json::nlohmann_json
        librpgm
        freetype
        clip
        sfml-audio
        oremath
        libcurl
        cpuid
        angelscript
        angelscript_addons
        athena-core
        lcf
        inih
)
target_compile_definitions(orecore PUBLIC GETTEXT_DOMAIN="${GETTEXT_DOMAIN}" GETTEXT_OUTPUT_DIR="${GETTEXT_OUTPUT_DIR}")

# Define set of OS specific files to include
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(orecore PRIVATE Platform/Windows/Resources.cpp)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_sources(orecore PRIVATE Platform/Mac/Resources.cpp)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(orecore PRIVATE Platform/Linux/Resources.cpp)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GCC")
    target_compile_options(orecore PRIVATE -Wno-deprecated-enum-enum-conversion)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "CLANG")
    target_compile_options(orecore PRIVATE -Wno-deprecated-enum-enum-conversion)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(orecore PRIVATE /utf-8)
endif ()

add_executable(${NAME} WIN32 MACOSX_BUNDLE App/Main.cpp App/ProjectInfo.cpp
        App/ProjectInfo.hpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GCC")
    target_compile_options(${NAME} PRIVATE -Wno-deprecated-enum-enum-conversion)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "CLANG")
    target_compile_options(${NAME} PRIVATE -Wno-deprecated-enum-enum-conversion)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${NAME} PRIVATE /utf-8)
endif ()

include(${PROJECT_SOURCE_DIR}/src/openrpgeditor/cmake/AppAssets.cmake)
include(${PROJECT_SOURCE_DIR}/src/openrpgeditor/cmake/Packaging.cmake)

target_include_directories(${NAME} PUBLIC)
target_include_directories(
        ${NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${SFML_SOURCE_DIR}/include/SFML
        ${cpuid_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/extern/inih
        ${CMAKE_SOURCE_DIR}/extern/angelscript/angelscript/include
        ${CMAKE_SOURCE_DIR}/extern/angelscript/add_on
        "${CMAKE_SOURCE_DIR}/extern/nano-signal-slot")

target_compile_features(${NAME} PRIVATE cxx_std_23)
target_link_libraries(
        ${NAME}
        PRIVATE
        fmt::fmt-header-only
        spdlog::spdlog_header_only
        PUBLIC
        #       CommonUI
        orecore
        SDL3::SDL3
        SDL3_image::SDL3_image
        imgui-setup
        magic_enum
        nlohmann_json::nlohmann_json
        librpgm
        librpgmeventcmds
        librpgmmovementroutecmds
        freetype
        sfml-audio
        cpuid
        angelscript
        angelscript_addons
        athena-core
        inih
)
target_compile_definitions(${NAME} PUBLIC GETTEXT_DOMAIN="${GETTEXT_DOMAIN}" GETTEXT_OUTPUT_DIR="${GETTEXT_OUTPUT_DIR}")

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL
        "RelWithDebugInfo")
    add_compile_definitions(ZLIB_DEBUG)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_definitions(_DEBUG DEBUG)
endif ()

if (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)
endif ()