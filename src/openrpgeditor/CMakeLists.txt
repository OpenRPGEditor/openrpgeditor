# include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

configure_file(Editor/ProjectInfo.cpp.in Editor/ProjectInfo.cpp @ONLY)

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
        Editor/Application.cpp
        Editor/Application.hpp
        Editor/CommonUI/ActionsEditor.cpp
        Editor/CommonUI/ActionsEditor.hpp
        Editor/CommonUI/CharacterPicker.cpp
        Editor/CommonUI/CharacterPicker.hpp
        Editor/CommonUI/ColorFlashPicker.cpp
        Editor/CommonUI/ColorFlashPicker.hpp
        Editor/CommonUI/ColorTonePicker.cpp
        Editor/CommonUI/ColorTonePicker.hpp
        Editor/CommonUI/CreateNewProjectDialog.cpp
        Editor/CommonUI/CreateNewProjectDialog.hpp
        Editor/CommonUI/EffectsEditor.cpp
        Editor/CommonUI/EffectsEditor.hpp
        Editor/CommonUI/EventCommandEditor.cpp
        Editor/CommonUI/EventCommandEditor.hpp
        Editor/CommonUI/GameWindowColorPicker.cpp
        Editor/CommonUI/GameWindowColorPicker.hpp
        Editor/CommonUI/GridAtlasPicker.cpp
        Editor/CommonUI/GridAtlasPicker.hpp
        Editor/CommonUI/IDialogController.hpp
        Editor/CommonUI/ImagePicker.cpp
        Editor/CommonUI/ImagePicker.hpp
        Editor/CommonUI/ObjectPicker.hpp
        Editor/CommonUI/ObjectPicker.inl
        Editor/CommonUI/TextEditor.cpp
        Editor/CommonUI/TextEditor.hpp
        Editor/CommonUI/TilesetTilePicker.cpp
        Editor/CommonUI/TilesetTilePicker.hpp
        Editor/CommonUI/TraitsEditor.cpp
        Editor/CommonUI/TraitsEditor.hpp
        Editor/CommonUI/VariableSwitchPicker.cpp
        Editor/CommonUI/VariableSwitchPicker.hpp
        Editor/CommonUI/Directory.cpp
        Editor/CommonUI/Directory.hpp
        Editor/CommonUI/FacePicker.cpp
        Editor/CommonUI/FacePicker.hpp
        Editor/DatabaseEditor.cpp
        Editor/DatabaseEditor.hpp
        Editor/DatabaseEditor/DBActorsTab.cpp
        Editor/DatabaseEditor/DBActorsTab.hpp
        Editor/DatabaseEditor/DBAnimationsTab.cpp
        Editor/DatabaseEditor/DBAnimationsTab.hpp
        Editor/DatabaseEditor/DBArmorsTab.cpp
        Editor/DatabaseEditor/DBArmorsTab.hpp
        Editor/DatabaseEditor/DBClassesTab.cpp
        Editor/DatabaseEditor/DBClassesTab.hpp
        Editor/DatabaseEditor/DBCommonEventsTab.cpp
        Editor/DatabaseEditor/DBCommonEventsTab.hpp
        Editor/DatabaseEditor/DBEnemiesTab.cpp
        Editor/DatabaseEditor/DBEnemiesTab.hpp
        Editor/DatabaseEditor/DBGameConstantsTab.cpp
        Editor/DatabaseEditor/DBGameConstantsTab.hpp
        Editor/DatabaseEditor/DBItemsTab.cpp
        Editor/DatabaseEditor/DBItemsTab.hpp
        Editor/DatabaseEditor/DBSkillsTab.cpp
        Editor/DatabaseEditor/DBSkillsTab.hpp
        Editor/DatabaseEditor/DBStatesTab.cpp
        Editor/DatabaseEditor/DBStatesTab.hpp
        Editor/DatabaseEditor/DBSystemTab.cpp
        Editor/DatabaseEditor/DBSystemTab.hpp
        Editor/DatabaseEditor/DBTemplatesTab.cpp
        Editor/DatabaseEditor/DBTemplatesTab.hpp
        Editor/DatabaseEditor/DBTermsTab.cpp
        Editor/DatabaseEditor/DBTermsTab.hpp
        Editor/DatabaseEditor/DBTilesetsTab.cpp
        Editor/DatabaseEditor/DBTilesetsTab.hpp
        Editor/DatabaseEditor/DBTroopsTab.cpp
        Editor/DatabaseEditor/DBTroopsTab.hpp
        Editor/DatabaseEditor/DBTypesTab.cpp
        Editor/DatabaseEditor/DBTypesTab.hpp
        Editor/DatabaseEditor/DBWeaponsTab.cpp
        Editor/DatabaseEditor/DBWeaponsTab.hpp
        Editor/EventCommands/Dialog_AbortBattle.cpp
        Editor/EventCommands/Dialog_AbortBattle.hpp
        Editor/EventCommands/Dialog_BattleProcessing.cpp
        Editor/EventCommands/Dialog_BattleProcessing.hpp
        Editor/EventCommands/Dialog_BreakLoop.cpp
        Editor/EventCommands/Dialog_BreakLoop.hpp
        Editor/EventCommands/Dialog_ChangeActorImages.cpp
        Editor/EventCommands/Dialog_ChangeActorImages.hpp
        Editor/EventCommands/Dialog_ChangeArmors.cpp
        Editor/EventCommands/Dialog_ChangeArmors.hpp
        Editor/EventCommands/Dialog_ChangeBattleBack.cpp
        Editor/EventCommands/Dialog_ChangeBattleBack.hpp
        Editor/EventCommands/Dialog_ChangeBattleBGM.cpp
        Editor/EventCommands/Dialog_ChangeBattleBGM.hpp
        Editor/EventCommands/Dialog_ChangeClass.cpp
        Editor/EventCommands/Dialog_ChangeClass.hpp
        Editor/EventCommands/Dialog_ChangeDefeatME.cpp
        Editor/EventCommands/Dialog_ChangeDefeatME.hpp
        Editor/EventCommands/Dialog_ChangeEncounter.cpp
        Editor/EventCommands/Dialog_ChangeEncounter.hpp
        Editor/EventCommands/Dialog_ChangeEnemyHP.cpp
        Editor/EventCommands/Dialog_ChangeEnemyHP.hpp
        Editor/EventCommands/Dialog_ChangeEnemyMP.cpp
        Editor/EventCommands/Dialog_ChangeEnemyMP.hpp
        Editor/EventCommands/Dialog_ChangeEnemyState.cpp
        Editor/EventCommands/Dialog_ChangeEnemyState.hpp
        Editor/EventCommands/Dialog_ChangeEnemyTP.cpp
        Editor/EventCommands/Dialog_ChangeEnemyTP.hpp
        Editor/EventCommands/Dialog_ChangeEquipment.cpp
        Editor/EventCommands/Dialog_ChangeEquipment.hpp
        Editor/EventCommands/Dialog_ChangeEXP.cpp
        Editor/EventCommands/Dialog_ChangeEXP.hpp
        Editor/EventCommands/Dialog_ChangeFormationAccess.cpp
        Editor/EventCommands/Dialog_ChangeFormationAccess.hpp
        Editor/EventCommands/Dialog_ChangeGold.cpp
        Editor/EventCommands/Dialog_ChangeGold.hpp
        Editor/EventCommands/Dialog_ChangeHP.cpp
        Editor/EventCommands/Dialog_ChangeHP.hpp
        Editor/EventCommands/Dialog_ChangeItems.cpp
        Editor/EventCommands/Dialog_ChangeItems.hpp
        Editor/EventCommands/Dialog_ChangeLevel.cpp
        Editor/EventCommands/Dialog_ChangeLevel.hpp
        Editor/EventCommands/Dialog_ChangeMapDisplayName.cpp
        Editor/EventCommands/Dialog_ChangeMapDisplayName.hpp
        Editor/EventCommands/Dialog_ChangeMenuAccess.cpp
        Editor/EventCommands/Dialog_ChangeMenuAccess.hpp
        Editor/EventCommands/Dialog_ChangeMP.cpp
        Editor/EventCommands/Dialog_ChangeMP.hpp
        Editor/EventCommands/Dialog_ChangeName.cpp
        Editor/EventCommands/Dialog_ChangeName.hpp
        Editor/EventCommands/Dialog_ChangeNickname.cpp
        Editor/EventCommands/Dialog_ChangeNickname.hpp
        Editor/EventCommands/Dialog_ChangeParallax.cpp
        Editor/EventCommands/Dialog_ChangeParallax.hpp
        Editor/EventCommands/Dialog_ChangeParameter.cpp
        Editor/EventCommands/Dialog_ChangeParameter.hpp
        Editor/EventCommands/Dialog_ChangePartyMember.cpp
        Editor/EventCommands/Dialog_ChangePartyMember.hpp
        Editor/EventCommands/Dialog_ChangePlayerFollowers.cpp
        Editor/EventCommands/Dialog_ChangePlayerFollowers.hpp
        Editor/EventCommands/Dialog_ChangePluginCommand.cpp
        Editor/EventCommands/Dialog_ChangePluginCommand.hpp
        Editor/EventCommands/Dialog_ChangeProfile.cpp
        Editor/EventCommands/Dialog_ChangeProfile.hpp
        Editor/EventCommands/Dialog_ChangeSaveAccess.cpp
        Editor/EventCommands/Dialog_ChangeSaveAccess.hpp
        Editor/EventCommands/Dialog_ChangeSkill.cpp
        Editor/EventCommands/Dialog_ChangeSkill.hpp
        Editor/EventCommands/Dialog_ChangeState.cpp
        Editor/EventCommands/Dialog_ChangeState.hpp
        Editor/EventCommands/Dialog_ChangeTileset.cpp
        Editor/EventCommands/Dialog_ChangeTileset.hpp
        Editor/EventCommands/Dialog_ChangeTP.cpp
        Editor/EventCommands/Dialog_ChangeTP.hpp
        Editor/EventCommands/Dialog_ChangeTransparency.cpp
        Editor/EventCommands/Dialog_ChangeTransparency.hpp
        Editor/EventCommands/Dialog_ChangeVehicleBGM.cpp
        Editor/EventCommands/Dialog_ChangeVehicleBGM.hpp
        Editor/EventCommands/Dialog_ChangeVehicleImage.cpp
        Editor/EventCommands/Dialog_ChangeVehicleImage.hpp
        Editor/EventCommands/Dialog_ChangeVictoryME.cpp
        Editor/EventCommands/Dialog_ChangeVictoryME.hpp
        Editor/EventCommands/Dialog_ChangeWeapons.cpp
        Editor/EventCommands/Dialog_ChangeWeapons.hpp
        Editor/EventCommands/Dialog_ChangeWindowColor.cpp
        Editor/EventCommands/Dialog_ChangeWindowColor.hpp
        Editor/EventCommands/Dialog_Comment.cpp
        Editor/EventCommands/Dialog_Comment.hpp
        Editor/EventCommands/Dialog_CommonEvent.cpp
        Editor/EventCommands/Dialog_CommonEvent.hpp
        Editor/EventCommands/Dialog_ConditionalBranch.cpp
        Editor/EventCommands/Dialog_ConditionalBranch.hpp
        Editor/EventCommands/Dialog_ControlSelfSwitch.cpp
        Editor/EventCommands/Dialog_ControlSelfSwitch.hpp
        Editor/EventCommands/Dialog_ControlSwitches.cpp
        Editor/EventCommands/Dialog_ControlSwitches.hpp
        Editor/EventCommands/Dialog_ControlVariables.cpp
        Editor/EventCommands/Dialog_ControlVariables.hpp
        Editor/EventCommands/Dialog_EnemyAppear.cpp
        Editor/EventCommands/Dialog_EnemyAppear.hpp
        Editor/EventCommands/Dialog_EnemyRecoverAll.cpp
        Editor/EventCommands/Dialog_EnemyRecoverAll.hpp
        Editor/EventCommands/Dialog_EnemyTransform.cpp
        Editor/EventCommands/Dialog_EnemyTransform.hpp
        Editor/EventCommands/Dialog_EraseEvent.cpp
        Editor/EventCommands/Dialog_EraseEvent.hpp
        Editor/EventCommands/Dialog_ErasePicture.cpp
        Editor/EventCommands/Dialog_ErasePicture.hpp
        Editor/EventCommands/Dialog_ExitEventProcessing.cpp
        Editor/EventCommands/Dialog_ExitEventProcessing.hpp
        Editor/EventCommands/Dialog_FadeinScreen.cpp
        Editor/EventCommands/Dialog_FadeinScreen.hpp
        Editor/EventCommands/Dialog_FadeoutBGM.cpp
        Editor/EventCommands/Dialog_FadeoutBGM.hpp
        Editor/EventCommands/Dialog_FadeoutBGS.cpp
        Editor/EventCommands/Dialog_FadeoutBGS.hpp
        Editor/EventCommands/Dialog_FadeoutScreen.cpp
        Editor/EventCommands/Dialog_FadeoutScreen.hpp
        Editor/EventCommands/Dialog_FlashScreen.cpp
        Editor/EventCommands/Dialog_FlashScreen.hpp
        Editor/EventCommands/Dialog_GameData.cpp
        Editor/EventCommands/Dialog_GameData.hpp
        Editor/EventCommands/Dialog_GameOver.cpp
        Editor/EventCommands/Dialog_GameOver.hpp
        Editor/EventCommands/Dialog_GatherFollowers.cpp
        Editor/EventCommands/Dialog_GatherFollowers.hpp
        Editor/EventCommands/Dialog_GetOnOffVehicle.cpp
        Editor/EventCommands/Dialog_GetOnOffVehicle.hpp
        Editor/EventCommands/Dialog_InputNumber.cpp
        Editor/EventCommands/Dialog_InputNumber.hpp
        Editor/EventCommands/Dialog_JumpToLabel.cpp
        Editor/EventCommands/Dialog_JumpToLabel.hpp
        Editor/EventCommands/Dialog_Label.cpp
        Editor/EventCommands/Dialog_Label.hpp
        Editor/EventCommands/Dialog_Loop.cpp
        Editor/EventCommands/Dialog_Loop.hpp
        Editor/EventCommands/Dialog_MovePicture.cpp
        Editor/EventCommands/Dialog_MovePicture.hpp
        Editor/EventCommands/Dialog_NameInputProcessing.cpp
        Editor/EventCommands/Dialog_NameInputProcessing.hpp
        Editor/EventCommands/Dialog_OpenMenuScreen.cpp
        Editor/EventCommands/Dialog_OpenMenuScreen.hpp
        Editor/EventCommands/Dialog_OpenSaveScreen.cpp
        Editor/EventCommands/Dialog_OpenSaveScreen.hpp
        Editor/EventCommands/Dialog_PlayBGM.cpp
        Editor/EventCommands/Dialog_PlayBGM.hpp
        Editor/EventCommands/Dialog_PlayBGS.cpp
        Editor/EventCommands/Dialog_PlayBGS.hpp
        Editor/EventCommands/Dialog_PlayME.cpp
        Editor/EventCommands/Dialog_PlayME.hpp
        Editor/EventCommands/Dialog_PlayMovie.cpp
        Editor/EventCommands/Dialog_PlayMovie.hpp
        Editor/EventCommands/Dialog_PlaySE.cpp
        Editor/EventCommands/Dialog_PlaySE.hpp
        Editor/EventCommands/Dialog_RecoverAll.cpp
        Editor/EventCommands/Dialog_RecoverAll.hpp
        Editor/EventCommands/Dialog_ReplayBGM.cpp
        Editor/EventCommands/Dialog_ReplayBGM.hpp
        Editor/EventCommands/Dialog_ReturnToTitle.cpp
        Editor/EventCommands/Dialog_ReturnToTitle.hpp
        Editor/EventCommands/Dialog_RotatePicture.cpp
        Editor/EventCommands/Dialog_RotatePicture.hpp
        Editor/EventCommands/Dialog_SaveBGM.cpp
        Editor/EventCommands/Dialog_SaveBGM.hpp
        Editor/EventCommands/Dialog_Script.cpp
        Editor/EventCommands/Dialog_Script.hpp
        Editor/EventCommands/Dialog_ScrollMap.cpp
        Editor/EventCommands/Dialog_ScrollMap.hpp
        Editor/EventCommands/Dialog_SelectItem.cpp
        Editor/EventCommands/Dialog_SelectItem.hpp
        Editor/EventCommands/Dialog_SetEventLocation.cpp
        Editor/EventCommands/Dialog_SetEventLocation.hpp
        Editor/EventCommands/Dialog_SetMovementRoute.cpp
        Editor/EventCommands/Dialog_SetMovementRoute.hpp
        Editor/EventCommands/Dialog_SetVehicleLocation.cpp
        Editor/EventCommands/Dialog_SetVehicleLocation.hpp
        Editor/EventCommands/Dialog_SetWeatherEffect.cpp
        Editor/EventCommands/Dialog_SetWeatherEffect.hpp
        Editor/EventCommands/Dialog_ShakeScreen.cpp
        Editor/EventCommands/Dialog_ShakeScreen.hpp
        Editor/EventCommands/Dialog_ShopProcessing.cpp
        Editor/EventCommands/Dialog_ShopProcessing.hpp
        Editor/EventCommands/Dialog_ShopProcessing_Goods.cpp
        Editor/EventCommands/Dialog_ShopProcessing_Goods.hpp
        Editor/EventCommands/Dialog_ShowAnimation.cpp
        Editor/EventCommands/Dialog_ShowAnimation.hpp
        Editor/EventCommands/Dialog_ShowBalloonIcon.cpp
        Editor/EventCommands/Dialog_ShowBalloonIcon.hpp
        Editor/EventCommands/Dialog_ShowBattleAnimation.cpp
        Editor/EventCommands/Dialog_ShowBattleAnimation.hpp
        Editor/EventCommands/Dialog_ShowChoice.cpp
        Editor/EventCommands/Dialog_ShowChoice.hpp
        Editor/EventCommands/Dialog_ShowPicture.cpp
        Editor/EventCommands/Dialog_ShowPicture.hpp
        Editor/EventCommands/Dialog_ShowScrollingText.cpp
        Editor/EventCommands/Dialog_ShowScrollingText.hpp
        Editor/EventCommands/Dialog_ShowText.cpp
        Editor/EventCommands/Dialog_ShowText.hpp
        Editor/EventCommands/Dialog_StopSE.cpp
        Editor/EventCommands/Dialog_StopSE.hpp
        Editor/EventCommands/Dialog_ControlTimer.cpp
        Editor/EventCommands/Dialog_ControlTimer.hpp
        Editor/EventCommands/Dialog_TintPicture.cpp
        Editor/EventCommands/Dialog_TintPicture.hpp
        Editor/EventCommands/Dialog_TintScreen.cpp
        Editor/EventCommands/Dialog_TintScreen.hpp
        Editor/EventCommands/Dialog_TransferPlayer.cpp
        Editor/EventCommands/Dialog_TransferPlayer.hpp
        Editor/EventCommands/Dialog_Wait.cpp
        Editor/EventCommands/Dialog_Wait.hpp
        Editor/EventCommands/IEventDialogController.hpp
        Editor/EventCommands/MovementRoute/Dialog_MovementChangeBlendMode.cpp
        Editor/EventCommands/MovementRoute/Dialog_MovementChangeBlendMode.hpp
        Editor/EventCommands/MovementRoute/Dialog_MovementChangeFrequency.cpp
        Editor/EventCommands/MovementRoute/Dialog_MovementChangeFrequency.hpp
        Editor/EventCommands/MovementRoute/Dialog_MovementChangeImage.cpp
        Editor/EventCommands/MovementRoute/Dialog_MovementChangeImage.hpp
        Editor/EventCommands/MovementRoute/Dialog_MovementChangeOpacity.cpp
        Editor/EventCommands/MovementRoute/Dialog_MovementChangeOpacity.hpp
        Editor/EventCommands/MovementRoute/Dialog_MovementChangeSpeed.cpp
        Editor/EventCommands/MovementRoute/Dialog_MovementChangeSpeed.hpp
        Editor/EventCommands/MovementRoute/Dialog_MovementJump.cpp
        Editor/EventCommands/MovementRoute/Dialog_MovementJump.hpp
        Editor/EventCommands/MovementRoute/Dialog_MovementScript.cpp
        Editor/EventCommands/MovementRoute/Dialog_MovementScript.hpp
        Editor/EventCommands/MovementRoute/Dialog_MovementSwitchOFF.cpp
        Editor/EventCommands/MovementRoute/Dialog_MovementSwitchOFF.hpp
        Editor/EventCommands/MovementRoute/Dialog_MovementSwitchON.cpp
        Editor/EventCommands/MovementRoute/Dialog_MovementSwitchON.hpp
        Editor/EventCommands/MovementRoute/Dialog_MovementWait.cpp
        Editor/EventCommands/MovementRoute/Dialog_MovementWait.hpp
        Editor/EventEditor.cpp
        Editor/EventEditor.hpp
        Editor/EventEditor/EVPage.cpp
        Editor/EventEditor/EVPage.hpp
        Editor/EventListView.cpp
        Editor/EventListView.hpp
        Editor/FirstBootWizard.cpp
        Editor/FirstBootWizard.hpp
        Editor/FirstBootWizard/IWizardPage.hpp
        Editor/FirstBootWizard/ProjectLocationPage.cpp
        Editor/FirstBootWizard/ProjectLocationPage.hpp
        Editor/FirstBootWizard/RPGMakerLocationAndVersionPage.cpp
        Editor/FirstBootWizard/RPGMakerLocationAndVersionPage.hpp
        Editor/FirstBootWizard/UISettingsPage.cpp
        Editor/FirstBootWizard/UISettingsPage.hpp
        Editor/FirstBootWizard/WelcomePage.cpp
        Editor/FirstBootWizard/WelcomePage.hpp
        Editor/Graphics/CharacterSheet.cpp
        Editor/Graphics/CharacterSheet.hpp
        Editor/Graphics/CheckerboardTexture.cpp
        Editor/Graphics/CheckerboardTexture.hpp
        Editor/Graphics/CheckeredCompositeTexture.cpp
        Editor/Graphics/CheckeredCompositeTexture.hpp
        Editor/Graphics/ColorTonePreview.cpp
        Editor/Graphics/ColorTonePreview.hpp
        Editor/Graphics/FaceSheet.cpp
        Editor/Graphics/FaceSheet.hpp
        Editor/Graphics/GameWindowBackground.cpp
        Editor/Graphics/GameWindowBackground.hpp
        Editor/Graphics/IconSheet.cpp
        Editor/Graphics/IconSheet.hpp
        Editor/Graphics/Image.cpp
        Editor/Graphics/Image.hpp
        Editor/Graphics/image_loader.cpp
        Editor/Graphics/image_loader.h
        Editor/Graphics/RenderImage.cpp
        Editor/Graphics/RenderImage.hpp
        Editor/Graphics/SideViewBattlerSheet.cpp
        Editor/Graphics/SideViewBattlerSheet.hpp
        Editor/Graphics/SimpleRect.hpp
        Editor/Graphics/stb_image.h
        Editor/Graphics/Texture.cpp
        Editor/Graphics/Texture.hpp
        Editor/Graphics/ParallaxTexture.cpp
        Editor/Graphics/ParallaxTexture.hpp
        Editor/ImGuiExt/ImGuiNotify.hpp
        Editor/ImGuiExt/ImGuiParsedText.cpp
        Editor/ImGuiExt/ImGuiParsedText.hpp
        Editor/ImGuiExt/ImGuiUtils.cpp
        Editor/ImGuiExt/ImGuiUtils.hpp
        Editor/ImGuiExt/TextParser.cpp
        Editor/ImGuiExt/TextParser.hpp
        Editor/Log.cpp
        Editor/Log.hpp
        Editor/MapEditor.cpp
        Editor/MapEditor.hpp
        Editor/MapEditor/MapCursor.cpp
        Editor/MapEditor/MapCursor.hpp
        Editor/MapEditor/MapEvent.cpp
        Editor/MapEditor/MapEvent.hpp
        Editor/MapEditor/MapProperties.cpp
        Editor/MapEditor/MapProperties.hpp
        Editor/MapListView.cpp
        Editor/MapListView.hpp
        Editor/MainWindow.cpp
        Editor/MainWindow.hpp
        Editor/ResourceManager.cpp
        Editor/ResourceManager.hpp
        Editor/Resources.hpp
        Editor/Settings.cpp
        Editor/Settings.hpp
        Editor/SettingsDialog.cpp
        Editor/SettingsDialog.hpp
        Editor/SettingsDialog/GeneralSettingsTab.cpp
        Editor/SettingsDialog/GeneralSettingsTab.hpp
        Editor/SettingsDialog/RPGMakerLocationAndVersionTab.cpp
        Editor/SettingsDialog/RPGMakerLocationAndVersionTab.hpp
        Editor/SettingsDialog/ISettingsTab.hpp
        Editor/Sound.cpp
        Editor/Sound.hpp
        Editor/TemplateEditor/Dialog/TemplateName.cpp
        Editor/TemplateEditor/Dialog/TemplateName.hpp
        Editor/TemplateEditor/Dialog/TemplatesEvent.cpp
        Editor/TemplateEditor/Dialog/TemplatesEvent.hpp
        Editor/TemplateEditor/TemplatesCommand.cpp
        Editor/TemplateEditor/TemplatesCommand.hpp
        Editor/TemplateEditor/TemplatesEventProperties.cpp
        Editor/TemplateEditor/TemplatesEventProperties.hpp
        Editor/TemplateEditor/TemplatesTint.cpp
        Editor/TemplateEditor/TemplatesTint.hpp
        Editor/TroopsEventEditor/TroopsEVEditor.cpp
        Editor/TroopsEventEditor/TroopsEVEditor.hpp
        Editor/TroopsEventEditor/TroopsEVPage.cpp
        Editor/TroopsEventEditor/TroopsEVPage.hpp
        Editor/Tilemap/ITileView.cpp
        Editor/Tilemap/ITileView.hpp
        Editor/UndoStack.cpp
        Editor/UndoStack.hpp
        Editor/Window.cpp
        Editor/Window.hpp
        Editor/Tilemap/TilePalette.cpp
        Editor/Tilemap/TilePalette.hpp
        Editor/Tilemap/TileRenderHelper.cpp
        Editor/Tilemap/TileRenderHelper.hpp
        Editor/Graphics/TileMarker.cpp
        Editor/Graphics/TileMarker.hpp
        Editor/SettingsDialog/UISettingsTab.cpp
        Editor/SettingsDialog/UISettingsTab.hpp
        Editor/EditorPlugin/EditorPluginManager.cpp
        Editor/EditorPlugin/EditorPluginManager.hpp
        Editor/DirectoryMonitor.cpp
        Editor/DirectoryMonitor.hpp
        Editor/Script/ScriptEngine.cpp
        Editor/Script/ScriptEngine.hpp
        Editor/EditorPlugin/EditorPlugin.cpp
        Editor/EditorPlugin/EditorPlugin.hpp
        Editor/Script/ByteCodeStream.cpp
        Editor/Script/ByteCodeStream.hpp
        Editor/Script/Bindings.cpp
        Editor/Script/Bindings.hpp
        Editor/Tilemap/TilemapView.cpp
        Editor/Tilemap/TilemapView.hpp
        Editor/CommonUI/SoundPicker.cpp
        Editor/CommonUI/SoundPicker.hpp
        Editor/ImGuiExt/imgui-knobs.h
        Editor/ImGuiExt/imgui-knobs.cpp
        Editor/ImGuiExt/imgui_neo_internal.h
        Editor/ImGuiExt/imgui_neo_internal.cpp
        Editor/ImGuiExt/imgui_neo_sequencer.h
        Editor/ImGuiExt/imgui_neo_sequencer.cpp
        Editor/DatabaseEditor/DBDocTab.cpp
        Editor/DatabaseEditor/DBDocTab.hpp
        Editor/EventSearcher.cpp
        Editor/EventSearcher.hpp
        Editor/ReferenceSearch.cpp
        Editor/ReferenceSearch.hpp
        Editor/DatabaseEditor/DBMappingsTab.cpp
        Editor/DatabaseEditor/DBMappingsTab.hpp
        Editor/CommonUI/Preview.cpp
        Editor/CommonUI/Preview.hpp
        Editor/Interpreter.cpp
        Editor/Interpreter.hpp
        Editor/LibLCF.cpp
        Editor/LibLCF.hpp
        Editor/DatabaseEditor/DBLocaleTab.cpp
        Editor/DatabaseEditor/DBLocaleTab.hpp

        Editor/Script/ImGui/ImGuiMath.cpp
        Editor/Script/ImGui/ImGuiMath.hpp
        Editor/Script/ImGui/ImGuiCoreOperations.cpp
        Editor/Script/ImGui/ImGuiCoreOperations.hpp
        Editor/LCF_Importer/LCF_Importer.cpp
        Editor/LCF_Importer/LCF_Importer.hpp
        Editor/LCF_Importer/LCF_Mapping.cpp
        Editor/LCF_Importer/LCF_Mapping.hpp
        Editor/EventCommands/MovementRoute/Dialogue_MovementPlaySE.cpp
        Editor/EventCommands/MovementRoute/Dialogue_MovementPlaySE.hpp
        Editor/CommonUI/GroupBox.cpp
        Editor/CommonUI/GroupBox.hpp
        Editor/CommonUI/AudioEditor.cpp
        Editor/CommonUI/AudioEditor.hpp
        Editor/CommonUI/EventTilePicker.cpp
        Editor/CommonUI/EventTilePicker.hpp
        Editor/FirstBootWizard/ExperimentalWarning.cpp
        Editor/FirstBootWizard/ExperimentalWarning.hpp
        Editor/AboutDialog.cpp
        Editor/AboutDialog.hpp
        Editor/ApplicationTheme.cpp
        Editor/ApplicationTheme.hpp

        Editor/Managers/DownloadManager.cpp
        Editor/Managers/DownloadManager.hpp
        Editor/Managers/ApplicationThemeManager.cpp
        Editor/Managers/ApplicationThemeManager.hpp
        Editor/Managers/MapToolsManager.hpp
        Editor/Managers/MapToolsManager.cpp
        Editor/Managers/NWJSVersionManager.cpp
        Editor/Managers/NWJSVersionManager.hpp
        Editor/Managers/SettingsManager.cpp
        Editor/Managers/SettingsManager.hpp
        Editor/Managers/ArchiveJob.cpp
        Editor/Managers/ArchiveJob.hpp
        Editor/Managers/ArchiveManager.cpp
        Editor/Managers/ArchiveManager.hpp
        
        Editor/StopWatch.cpp
        Editor/StopWatch.hpp
        Editor/Process.cpp
        Editor/Process.hpp
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
        spdlog::spdlog_header_only
        PUBLIC
        # CommonUI
        SDL3::SDL3
        SDL3_image::SDL3_image
        imgui-setup
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
        magic_enum
        archive
)
target_compile_definitions(orecore PUBLIC GETTEXT_DOMAIN="${GETTEXT_DOMAIN}" GETTEXT_OUTPUT_DIR="${GETTEXT_OUTPUT_DIR}" PUBLIC ORE_DISABLE_SCRIPTING=0)

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

add_executable(${NAME} WIN32 MACOSX_BUNDLE Editor/Main.cpp Editor/ProjectInfo.cpp
        Editor/ProjectInfo.hpp
        Editor/MapTools/IMapTool.hpp
        Editor/MapTools/PenMapTool.hpp
        Editor/MapTools/PenMapTool.cpp)

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
        spdlog::spdlog_header_only
        PUBLIC
        #       CommonUI
        orecore
        SDL3::SDL3
        SDL3_image::SDL3_image
        imgui-setup
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
        cpptrace::cpptrace
)

# Needed for shared library builds on windows:  copy cpptrace.dll to the same directory as the
# executable for your_target
if (WIN32)
    add_custom_command(
            TARGET ${NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:cpptrace::cpptrace>
            $<TARGET_FILE_DIR:${NAME}>
    )
endif ()
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