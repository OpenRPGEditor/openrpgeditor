set(NAME "librpgm")

#include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

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
        Database/Animations.hpp Database/Animations.cpp
        Database/Animation.hpp Database/Animation.cpp
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
        Database/ColorFormatter.hpp Database/ColorFormatter.cpp
        Database/CommandParser.hpp Database/CommandParser.cpp
        Database/MovementRoute.hpp Database/MovementRoute.cpp
        Database/RPGEquations.hpp Database/RPGEquations.cpp
        Database/Database.hpp Database/Database.cpp
        Database/EventCommands/IEventCommand.hpp
        Database/EventCommands/UnhandledEvent.hpp Database/EventCommands/UnhandledEvent.cpp
        Database/EventCommands/EventDummy.hpp Database/EventCommands/EventDummy.cpp
        Database/EventCommands/ShowText.hpp Database/EventCommands/ShowText.cpp
        Database/EventCommands/ShowChoice.hpp Database/EventCommands/ShowChoice.cpp
        Database/EventCommands/InputNumber.hpp Database/EventCommands/InputNumber.cpp
        Database/EventCommands/SelectItem.hpp Database/EventCommands/SelectItem.cpp
        Database/EventCommands/ScrollingText.hpp Database/EventCommands/ScrollingText.cpp
        Database/EventCommands/Comment.hpp Database/EventCommands/Comment.cpp
        Database/EventCommands/ConditionalBranch.hpp Database/EventCommands/ConditionalBranch.cpp
        Database/EventCommands/Loop.hpp Database/EventCommands/Loop.cpp
        Database/EventCommands/RepeatAbove.hpp Database/EventCommands/RepeatAbove.cpp
        Database/EventCommands/BreakLoop.hpp Database/EventCommands/BreakLoop.cpp
        Database/EventCommands/ExitEventProcessing.hpp Database/EventCommands/ExitEventProcessing.cpp
        Database/EventCommands/CommonEvent.hpp Database/EventCommands/CommonEvent.cpp
        Database/EventCommands/Label.hpp Database/EventCommands/Label.cpp
        Database/EventCommands/JumpToLabel.hpp Database/EventCommands/JumpToLabel.cpp
        Database/EventCommands/ControlSwitches.hpp Database/EventCommands/ControlSwitches.cpp
        Database/EventCommands/ControlVariables.hpp Database/EventCommands/ControlVariables.cpp
        Database/EventCommands/ControlSelfSwitch.hpp Database/EventCommands/ControlSelfSwitch.cpp
        Database/EventCommands/ControlTimer.hpp Database/EventCommands/ControlTimer.cpp
        Database/EventCommands/ChangeGold.hpp Database/EventCommands/ChangeGold.cpp
        Database/EventCommands/ChangeItems.hpp Database/EventCommands/ChangeItems.cpp
        Database/EventCommands/ChangeWeapons.hpp Database/EventCommands/ChangeWeapons.cpp
        Database/EventCommands/ChangeArmors.hpp Database/EventCommands/ChangeArmors.cpp
        Database/EventCommands/ChangePartyMember.hpp Database/EventCommands/ChangePartyMember.cpp
        Database/EventCommands/ChangeBattleBGM.hpp Database/EventCommands/ChangeBattleBGM.cpp
        Database/EventCommands/ChangeVictoryME.hpp Database/EventCommands/ChangeVictoryME.cpp
        Database/EventCommands/ChangeSaveAccess.hpp Database/EventCommands/ChangeSaveAccess.cpp
        Database/EventCommands/ChangeMenuAccess.hpp Database/EventCommands/ChangeMenuAccess.cpp
        Database/EventCommands/ChangeEncounterDisable.hpp Database/EventCommands/ChangeEncounterDisable.cpp
        Database/EventCommands/ChangeFormationAccess.hpp Database/EventCommands/ChangeFormationAccess.cpp
        Database/EventCommands/ChangeWindowColor.hpp Database/EventCommands/ChangeWindowColor.cpp
        Database/EventCommands/ChangeDefeatME.hpp Database/EventCommands/ChangeDefeatME.cpp
        Database/EventCommands/ChangeVehicleBGM.hpp Database/EventCommands/ChangeVehicleBGM.cpp
        Database/EventCommands/TransferPlayer.hpp Database/EventCommands/TransferPlayer.cpp
        Database/EventCommands/SetVehicleLocation.hpp Database/EventCommands/SetVehicleLocation.cpp
        Database/EventCommands/SetEventLocation.hpp Database/EventCommands/SetEventLocation.cpp
        Database/EventCommands/ErasePicture.hpp Database/EventCommands/ErasePicture.cpp
        Database/EventCommands/MovePicture.hpp Database/EventCommands/MovePicture.cpp
        Database/EventCommands/RotatePicture.hpp Database/EventCommands/RotatePicture.cpp
        Database/EventCommands/TintPicture.hpp Database/EventCommands/TintPicture.cpp
        Database/EventCommands/SetWeatherEffect.hpp Database/EventCommands/SetWeatherEffect.cpp
        Database/EventCommands/Plugin.hpp Database/EventCommands/Plugin.cpp
        Database/EventCommands/ScrollMap.hpp Database/EventCommands/ScrollMap.cpp
        Database/EventCommands/ChangeMapDisplayName.hpp Database/EventCommands/ChangeMapDisplayName.cpp
        Database/EventCommands/ChangeTileset.hpp Database/EventCommands/ChangeTileset.cpp
        Database/EventCommands/ChangeBattleback.hpp Database/EventCommands/ChangeBattleback.cpp
        Database/EventCommands/GetLocationInfo.hpp Database/EventCommands/GetLocationInfo.cpp
        Database/EventCommands/BattleProcessing.hpp Database/EventCommands/BattleProcessing.cpp
        Database/EventCommands/ShopProcessing.cpp
        Database/EventCommands/ShopProcessing.hpp
        Database/EventCommands/NameInput.cpp
        Database/EventCommands/NameInput.hpp
        Database/EventCommands/ChangeHP.cpp
        Database/EventCommands/ChangeHP.hpp
        Database/EventCommands/ChangeMP.cpp
        Database/EventCommands/ChangeMP.hpp
        Database/EventCommands/ChangeTP.cpp
        Database/EventCommands/ChangeTP.hpp
        Database/EventCommands/ChangeEXP.cpp
        Database/EventCommands/ChangeEXP.hpp
        Database/EventCommands/ChangeLevel.cpp
        Database/EventCommands/ChangeLevel.hpp
        Database/EventCommands/ChangeParameter.cpp
        Database/EventCommands/ChangeParameter.hpp
        Database/EventCommands/RecoverAll.cpp
        Database/EventCommands/RecoverAll.hpp
        Database/EventCommands/ChangeName.cpp
        Database/EventCommands/ChangeName.hpp
        Database/EventCommands/ChangeClass.cpp
        Database/EventCommands/ChangeClass.hpp
        Database/EventCommands/ChangeState.cpp
        Database/EventCommands/ChangeState.hpp
        Database/EventCommands/ChangeSkill.cpp
        Database/EventCommands/ChangeSkill.hpp
        Database/EventCommands/ChangeEquipment.cpp
        Database/EventCommands/ChangeEquipment.hpp
        Database/EventCommands/GameOver.cpp
        Database/EventCommands/GameOver.hpp
        Database/EventCommands/ReturnToTitle.cpp
        Database/EventCommands/ReturnToTitle.hpp
        Database/EventCommands/OpenMenu.cpp
        Database/EventCommands/OpenMenu.hpp
        Database/EventCommands/OpenSave.cpp
        Database/EventCommands/OpenSave.hpp
        Database/EventCommands/AbortBattle.cpp
        Database/EventCommands/AbortBattle.hpp
        Database/EventCommands/EnemyRecoverAll.cpp
        Database/EventCommands/EnemyRecoverAll.hpp
        Database/EventCommands/EnemyAppear.cpp
        Database/EventCommands/EnemyAppear.hpp
        Database/EventCommands/EnemyTransform.cpp
        Database/EventCommands/EnemyTransform.hpp
        Database/EventCommands/ChangeEnemyHP.cpp
        Database/EventCommands/ChangeEnemyHP.hpp
        Database/EventCommands/ChangeEnemyMP.cpp
        Database/EventCommands/ChangeEnemyMP.hpp
        Database/EventCommands/ChangeEnemyTP.cpp
        Database/EventCommands/ChangeEnemyTP.hpp
        Database/EventCommands/ChangeEnemyState.cpp
        Database/EventCommands/ChangeEnemyState.hpp
        Database/EventCommands/ForceAction.cpp
        Database/EventCommands/ForceAction.hpp
        Database/EventCommands/ShowBattleAnimation.cpp
        Database/EventCommands/ShowBattleAnimation.hpp
        Database/EventCommands/ChangeVehicleImage.cpp
        Database/EventCommands/ChangeVehicleImage.hpp
        Database/EventCommands/ChangeActorImage.cpp
        Database/EventCommands/ChangeActorImage.hpp
        Database/EventCommands/ChangeProfile.cpp
        Database/EventCommands/ChangeProfile.hpp
        Database/EventCommands/ChangeNickname.cpp
        Database/EventCommands/ChangeNickname.hpp
        Database/EventCommands/ChangePlayerFollowers.cpp
        Database/EventCommands/ChangePlayerFollowers.hpp
        Database/EventCommands/GatherFollowers.cpp
        Database/EventCommands/GatherFollowers.hpp
        Database/EventCommands/FadeoutScreen.cpp
        Database/EventCommands/FadeoutScreen.hpp
        Database/EventCommands/FadeinScreen.cpp
        Database/EventCommands/FadeinScreen.hpp
        Database/EventCommands/TintScreen.cpp
        Database/EventCommands/TintScreen.hpp
        Database/EventCommands/FlashScreen.cpp
        Database/EventCommands/FlashScreen.hpp
        Database/EventCommands/ShakeScreen.cpp
        Database/EventCommands/ShakeScreen.hpp
        Database/EventCommands/SetMovementRoute.cpp
        Database/EventCommands/SetMovementRoute.hpp
        Database/EventCommands/GetOnOffVehicle.cpp
        Database/EventCommands/GetOnOffVehicle.hpp
        Database/EventCommands/ChangeTransparency.cpp
        Database/EventCommands/ChangeTransparency.hpp
        Database/EventCommands/ShowAnimation.cpp
        Database/EventCommands/ShowAnimation.hpp
        Database/EventCommands/ShowBalloonIcon.cpp
        Database/EventCommands/ShowBalloonIcon.hpp
        Database/EventCommands/EraseEvent.cpp
        Database/EventCommands/EraseEvent.hpp
        Database/EventCommands/Script.cpp
        Database/EventCommands/Script.hpp
        Database/EventCommands/Wait.cpp
        Database/EventCommands/Wait.hpp
        Database/EventCommands/PlaySE.cpp
        Database/EventCommands/PlaySE.hpp
        Database/EventCommands/PlayME.cpp
        Database/EventCommands/PlayME.hpp
        Database/EventCommands/PlayBGS.cpp
        Database/EventCommands/PlayBGS.hpp
        Database/EventCommands/PlayBGM.cpp
        Database/EventCommands/PlayBGM.hpp
        Database/EventCommands/PlayMovie.cpp
        Database/EventCommands/PlayMovie.hpp
        Database/EventCommands/SaveBGM.cpp
        Database/EventCommands/SaveBGM.hpp
        Database/EventCommands/ResumeBGM.cpp
        Database/EventCommands/ResumeBGM.hpp
        Database/EventCommands/FadeoutBGM.cpp
        Database/EventCommands/FadeoutBGM.hpp
        Database/EventCommands/FadeoutBGS.cpp
        Database/EventCommands/FadeoutBGS.hpp
        Database/EventCommands/StopSE.cpp
        Database/EventCommands/StopSE.hpp
        Database/EventCommands/ShowPicture.cpp
        Database/EventCommands/ShowPicture.hpp
        Database/EventCommands/End.cpp
        Database/EventCommands/End.hpp
        Database/EventCommands/MovementRoute/MoveDown.cpp
        Database/EventCommands/MovementRoute/MoveDown.hpp
        Database/EventCommands/MovementRoute/MoveRight.cpp
        Database/EventCommands/MovementRoute/MoveRight.hpp
        Database/EventCommands/MovementRoute/MoveUp.cpp
        Database/EventCommands/MovementRoute/MoveUp.hpp
        Database/EventCommands/MovementRoute/MoveLeft.cpp
        Database/EventCommands/MovementRoute/MoveLeft.hpp
        Database/EventCommands/MovementRoute/MoveLowerLeft.cpp
        Database/EventCommands/MovementRoute/MoveLowerLeft.hpp
        Database/EventCommands/MovementRoute/MoveLowerRight.cpp
        Database/EventCommands/MovementRoute/MoveLowerRight.hpp
        Database/EventCommands/MovementRoute/MoveUpperLeft.cpp
        Database/EventCommands/MovementRoute/MoveUpperLeft.hpp
        Database/EventCommands/MovementRoute/MoveUpperRight.cpp
        Database/EventCommands/MovementRoute/MoveUpperRight.hpp
        Database/EventCommands/MovementRoute/MoveAtRandom.cpp
        Database/EventCommands/MovementRoute/MoveAtRandom.hpp
        Database/EventCommands/MovementRoute/MoveTowardPlayer.cpp
        Database/EventCommands/MovementRoute/MoveTowardPlayer.hpp
        Database/EventCommands/MovementRoute/MoveAwayFromPlayer.cpp
        Database/EventCommands/MovementRoute/MoveAwayFromPlayer.hpp
        Database/EventCommands/MovementRoute/Move1StepForward.cpp
        Database/EventCommands/MovementRoute/Move1StepForward.hpp
        Database/EventCommands/MovementRoute/Move1StepBackward.cpp
        Database/EventCommands/MovementRoute/Move1StepBackward.hpp
        Database/EventCommands/MovementRoute/Jump.cpp
        Database/EventCommands/MovementRoute/Jump.hpp
        Database/EventCommands/MovementRoute/Wait.cpp
        Database/EventCommands/MovementRoute/Wait.hpp
        Database/EventCommands/MovementRoute/TurnDown.cpp
        Database/EventCommands/MovementRoute/TurnDown.hpp
        Database/EventCommands/MovementRoute/TurnLeft.cpp
        Database/EventCommands/MovementRoute/TurnLeft.hpp
        Database/EventCommands/MovementRoute/TurnRight.cpp
        Database/EventCommands/MovementRoute/TurnRight.hpp
        Database/EventCommands/MovementRoute/TurnUp.cpp
        Database/EventCommands/MovementRoute/TurnUp.hpp
        Database/EventCommands/MovementRoute/Turn90DegRight.cpp
        Database/EventCommands/MovementRoute/Turn90DegRight.hpp
        Database/EventCommands/MovementRoute/Turn90DegLeft.cpp
        Database/EventCommands/MovementRoute/Turn90DegLeft.hpp
        Database/EventCommands/MovementRoute/Turn180Deg.cpp
        Database/EventCommands/MovementRoute/Turn180Deg.hpp
        Database/EventCommands/MovementRoute/Turn180DegLeftOrRight.cpp
        Database/EventCommands/MovementRoute/Turn180DegLeftOrRight.hpp
        Database/EventCommands/MovementRoute/TurnAtRandom.cpp
        Database/EventCommands/MovementRoute/TurnAtRandom.hpp
        Database/EventCommands/MovementRoute/TurnTowardPlayer.cpp
        Database/EventCommands/MovementRoute/TurnTowardPlayer.hpp
        Database/EventCommands/MovementRoute/AwayFromPlayer.cpp
        Database/EventCommands/MovementRoute/AwayFromPlayer.hpp
        Database/EventCommands/MovementRoute/SwitchON.cpp
        Database/EventCommands/MovementRoute/SwitchON.hpp
        Database/EventCommands/MovementRoute/SwitchOFF.cpp
        Database/EventCommands/MovementRoute/SwitchOFF.hpp
        Database/EventCommands/MovementRoute/Speed.cpp
        Database/EventCommands/MovementRoute/Speed.hpp
        Database/EventCommands/MovementRoute/Frequency.cpp
        Database/EventCommands/MovementRoute/Frequency.hpp
        Database/EventCommands/MovementRoute/WalkingAnimationON.cpp
        Database/EventCommands/MovementRoute/WalkingAnimationON.hpp
        Database/EventCommands/MovementRoute/WalkingAnimationOFF.cpp
        Database/EventCommands/MovementRoute/WalkingAnimationOFF.hpp
        Database/EventCommands/MovementRoute/SteppingAnimationON.cpp
        Database/EventCommands/MovementRoute/SteppingAnimationON.hpp
        Database/EventCommands/MovementRoute/SteppingAnimationOFF.cpp
        Database/EventCommands/MovementRoute/SteppingAnimationOFF.hpp
        Database/EventCommands/MovementRoute/DirectionFixON.cpp
        Database/EventCommands/MovementRoute/DirectionFixON.hpp
        Database/EventCommands/MovementRoute/DirectionFixOFF.cpp
        Database/EventCommands/MovementRoute/DirectionFixOFF.hpp
        Database/EventCommands/MovementRoute/ThroughON.cpp
        Database/EventCommands/MovementRoute/ThroughON.hpp
        Database/EventCommands/MovementRoute/ThroughOFF.cpp
        Database/EventCommands/MovementRoute/ThroughOFF.hpp
        Database/EventCommands/MovementRoute/TransparentON.cpp
        Database/EventCommands/MovementRoute/TransparentON.hpp
        Database/EventCommands/MovementRoute/TransparentOFF.cpp
        Database/EventCommands/MovementRoute/TransparentOFF.hpp
        Database/EventCommands/MovementRoute/ChangeImage.cpp
        Database/EventCommands/MovementRoute/ChangeImage.hpp
        Database/EventCommands/MovementRoute/ChangeOpacity.cpp
        Database/EventCommands/MovementRoute/ChangeOpacity.hpp
        Database/EventCommands/MovementRoute/BlendMode.cpp
        Database/EventCommands/MovementRoute/BlendMode.hpp
        Database/EventCommands/MovementRoute/PlaySE.cpp
        Database/EventCommands/MovementRoute/PlaySE.hpp
        Database/EventCommands/MovementRoute/Script.cpp
        Database/EventCommands/MovementRoute/Script.hpp
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} nlohmann)
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(${NAME} PRIVATE project_warnings magic_enum imgui ImGuiFileDialog
        SDL2::SDL2 nlohmann_json::nlohmann_json fmt::fmt-header-only spdlog::spdlog_header_only)
