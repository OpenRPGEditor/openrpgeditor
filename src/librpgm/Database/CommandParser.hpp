#pragma once
#include "Database/Globals.hpp"
#include "Database/Audio.hpp"
#include "Database/MovementRoute.hpp"
#include "nlohmann/json.hpp"

#include <memory>

struct IEventCommand {
  std::optional<int> indent{};
  virtual ~IEventCommand() = default;
  [[nodiscard]] virtual EventCode code() const = 0;
  [[nodiscard]] virtual std::string stringRep() const {
    return std::string(indent ? *indent * 4 : 0, ' ') + DecodeEnumName(code());
  }
};

struct UnhandledEventCommand : IEventCommand {
  ~UnhandledEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return m_code; }

  std::optional<int> indent{};
  EventCode m_code{};
  nlohmann::json data;
};

struct EventDummy : IEventCommand {
  ~EventDummy() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Event_Dummy; }
  [[nodiscard]] std::string stringRep() const override { return std::string(indent ? *indent * 4 : 0, ' ') + "◇"; }
};

struct NextTextCommand : IEventCommand {
  ~NextTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Text; }

  std::string text;
};

struct ShowTextCommand : IEventCommand {
  ~ShowTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  std::string faceImage;
  int faceIndex;
  TextBackground background;
  TextWindowPosition position;

  std::vector<std::shared_ptr<NextTextCommand>> text;
};

struct ShowChoiceCommand : IEventCommand {
  ~ShowChoiceCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  TextBackground background;
  ChoiceWindowPosition positionType;
  std::vector<std::string> choices;
  int cancelType;
  int defaultType;
};

struct WhenSelectedCommand : IEventCommand {
  ~WhenSelectedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Selected; }

  int param1;
  std::string choice;
};

struct WhenCancelCommand : IEventCommand {
  ~WhenCancelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Cancel; }
};

struct ShowChoicesEndCommand : IEventCommand {
  ~ShowChoicesEndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End_de_ShowChoices; }
};

struct InputNumberCommand : IEventCommand {
  ~InputNumberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Input_Number; }
};

struct SelectItemCommand : IEventCommand {
  ~SelectItemCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Select_Item; }
};

struct NextScrollingTextCommand : IEventCommand {
  ~NextScrollingTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Scrolling_Text; }

  std::string text;
};

struct ShowScrollTextCommand : IEventCommand {
  ~ShowScrollTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Scrolling_Text; }

  int speed;
  bool noFast;
  std::vector<std::shared_ptr<NextScrollingTextCommand>> text;
};

struct NextCommentCommand : IEventCommand {
  ~NextCommentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Comment; }

  std::string text;
};
struct CommentCommand : IEventCommand {
  ~CommentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Comment; }
  std::string text;
  std::vector<std::shared_ptr<NextCommentCommand>> nextComments;
  [[nodiscard]] std::string stringRep() const override {
    std::string ret = std::string(indent ? *indent : 0, '\t') + "◇Comment: " + text;
    for (const auto& t : nextComments) {
      if (!ret.empty()) {
        ret += "\n";
      }
      ret += std::string(indent ? *indent : 0, '\t') + " :" + std::string(((t->indent ? *t->indent : 0) + 1), '\t') +
             " : " + t->text;
    }

    return ret;
  }
};

struct ConditionalBranchCommand : IEventCommand {
  ConditionalBranchCommand() {}
  ~ConditionalBranchCommand() override {}
  [[nodiscard]] EventCode code() const override { return EventCode::Conditional_Branch; }
  ConditionType type{};
  union {
    struct {
      int switchIdx;
      int checkIfOn;
    } globalSwitch;
    struct {
      int id;
      VariableComparisonSource source;
      VariableComparisonType comparison;
      union {
        int constant;
        int otherId;
      };
    } variable;
    struct {
      SwitchControl checkIfOn;
    } selfSwitch;
    struct {
      TimerComparisonType comparison;
      int sec;
    } timer;
    struct {
      int id;
      ActorConditionType type;
      int checkId;
    } actor;
    struct {
      int id;
      EnemyConditionType type;
      int stateId; // For "State" condition
    } enemy;
    struct {
      int id;
      Direction facing;
    } character;
    struct {
      int id;
    } vehicle;
    struct {
      GoldComaprisonType type;
      int value;
    } gold;
    struct {
      int id;
    } item;
    /* For Weapon and Armor conditions */
    struct {
      int equipId;
      bool includeEquipment;
    } equip;
    int raw{};
  };
  std::string button;
  std::string selfSw; // A, B, C, D
  std::string name;
  std::string script;
};

struct ElseCommand : IEventCommand {
  ~ElseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Else; }
};

struct LoopCommand : IEventCommand {
  ~LoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Loop; }
};

struct RepeatAboveCommand : IEventCommand {
  ~RepeatAboveCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Repeat_Above; }
};

struct BreakLoopCommand : IEventCommand {
  ~BreakLoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Break_Loop; }
};

struct ExitEventProecessingCommand : IEventCommand {
  ~ExitEventProecessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Exit_Event_Processing; }
};

struct CommonEventCommand : IEventCommand {
  ~CommonEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Common_Event; }
  int event;
};

struct LabelCommand : IEventCommand {
  ~LabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Label; }
  std::string label;
};

struct JumpToLabelCommand : IEventCommand {
  ~JumpToLabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump_To_Label; }
  std::string label;
};

struct ControlSwitches : IEventCommand {
  ~ControlSwitches() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Switches; }
  int start;
  int end;
  SwitchControl turnOff;
};

struct ControlVariables : IEventCommand {
  ControlVariables() {}
  ~ControlVariables() override{};
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Variables; }
  int start{};
  int end{};
  VariableControlOperation operation{};
  VariableControlOperand operand{};
  union {
    int constant;
    int variable;
    struct {
      int min;
      int max;
    } random;
    struct {
      GameDataSource source;
      union {
        ActorDataSource actorSource;
        EnemyDataSource enemySource;
        CharacterDataSource characterSource;
        OtherDataSource otherSource;
        int rawSource;
      };
      int value;
    } gameData;
    int raw{};
  };
  std::string script{};
};

struct ControlSelfSwitch : IEventCommand {
  ~ControlSelfSwitch() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Self_Switch; }

  std::string selfSw; // A, B, C, D
  SwitchControl turnOff;
};

struct ControlTimer : IEventCommand {
  ~ControlTimer() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Self_Switch; }
  TimerControl control;
  int seconds;
};

struct ChangeGoldCommmand : IEventCommand {
  ~ChangeGoldCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Gold; }
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
};

struct ChangeItemsCommmand : IEventCommand {
  ~ChangeItemsCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Items; }
  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
};

struct ChangeWeaponsCommmand : IEventCommand {
  ~ChangeWeaponsCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Weapons; }
  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  bool includeEquipment;
};

struct ChangeArmorsCommmand : IEventCommand {
  ~ChangeArmorsCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Armors; }
  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  bool includeEquipment;
};

struct ChangePartyMemberCommand : IEventCommand {
  ~ChangePartyMemberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Party_Member; }
  int member;
  PartyMemberOperation operation;
  bool initialize;
};
struct ChangeBattleBGMCommand : IEventCommand {
  ~ChangeBattleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Battle_BGM; }
  Audio bgm;
};

struct ChangeVictoryMECommand : IEventCommand {
  ~ChangeVictoryMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Victory_ME; }
  Audio me;
};

struct ChangeSaveAccessCommand : IEventCommand {
  ~ChangeSaveAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Save_Access; }
  AccessMode access;
};

struct ChangeMenuAccessCommand : IEventCommand {
  ~ChangeMenuAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Menu_Access; }
  AccessMode access;
};

struct ChangeEncounterDisableCommand : IEventCommand {
  ~ChangeEncounterDisableCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Encounter_Disable; }
  AccessMode access;
};

struct ChangeFormationAccessCommand : IEventCommand {
  ~ChangeFormationAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Formation_Access; }
  AccessMode access;
};

struct ChangeWindowColorCommand : IEventCommand {
  ~ChangeWindowColorCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Window_Color; }
  int r;
  int g;
  int b;
};

struct ChangeDefeatMECommand : IEventCommand {
  ~ChangeDefeatMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Defeat_ME; }
  Audio me;
};

struct ChangeVehicleMECommand : IEventCommand {
  ~ChangeVehicleMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vechicle_BGM; }
  Audio me;
};

struct TransferPlayerCommand : IEventCommand {
  ~TransferPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transfer_Player; }
  TransferMode mode;
  int mapId;
  int x;
  int y;
};

struct SetVehicleLocationCommand : IEventCommand {
  ~SetVehicleLocationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Vehicle_Location; }
  TransferMode mode;
  int mapId;
  int x;
  int y;
};
struct SetEventLocationCommand : IEventCommand {
  ~SetEventLocationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Event_Location; }
  TransferMode mode;
  int mapId;
  int x;
  int y;
  Direction direction;
};

struct ScrollMapCommand : IEventCommand {
  ~ScrollMapCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Scroll_Map; }
  Direction direction;
  int distance;
  MovementSpeed speed;
};

struct SetMovementRouteCommand : IEventCommand {
  ~SetMovementRouteCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Movement_Route; }
  int character;
  MovementRoute route;
  std::vector<std::shared_ptr<IEventCommand>> editNodes;
};

struct MovementRouteStepCommand : IEventCommand {
  ~MovementRouteStepCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Movement_Route_Step; }
  int character;
  std::shared_ptr<IEventCommand> step;
};

struct GetOnOffVehicleCommand : IEventCommand {
  ~GetOnOffVehicleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Get_On_Off_Vehicle; }
};

struct ChangeTransparencyCommand : IEventCommand {
  ~ChangeTransparencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Transparency; }
  int transparency;
};

struct ShowAnimationCommand : IEventCommand {
  ~ShowAnimationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Animation; }
  int character;
  int animation;
  bool waitForCompletion;
};

struct EraseEventCommand : IEventCommand {
  ~EraseEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Erase_Event; }
};

struct ScriptCommand : IEventCommand {
  ~ScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Script; }
  std::string script;
  std::vector<std::shared_ptr<IEventCommand>> moreScript;
};

struct NextScriptCommand : IEventCommand {
  ~NextScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Script; }
  std::string script;
};

struct PlaySECommand : IEventCommand {
  ~PlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE; }
};

struct EndCommand : IEventCommand {
  ~EndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End; }
};

// START MOVEMENTROUTE

struct MovementMoveDownCommand : IEventCommand {
  ~MovementMoveDownCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Down; }
};

struct MovementMoveLeftCommand : IEventCommand {
  ~MovementMoveLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Left; }
};

struct MovementMoveRightCommand : IEventCommand {
  ~MovementMoveRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Right; }
};

struct MovementMoveUpCommand : IEventCommand {
  ~MovementMoveUpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Up; }
};

struct MovementMoveLowerLeftommand : IEventCommand {
  ~MovementMoveLowerLeftommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Lower_Left; }
};
struct MovementMoveLowerRightCommand : IEventCommand {
  ~MovementMoveLowerRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Lower_Right; }
};
struct MovementMoveUpperLeftCommand : IEventCommand {
  ~MovementMoveUpperLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Upper_Left; }
};
struct MovementMoveUpperRightCommand : IEventCommand {
  ~MovementMoveUpperRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Upper_Right; }
};
struct MovementMoveAtRandomCommand : IEventCommand {
  ~MovementMoveAtRandomCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_at_Random; }
};
struct MovementMoveTowardPlayerCommand : IEventCommand {
  ~MovementMoveTowardPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_toward_Player; }
};
struct MovementMoveAwayFromPlayerCommand : IEventCommand {
  ~MovementMoveAwayFromPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_away_from_Player; }
};
struct MovementMove1StepFowardCommand : IEventCommand {
  ~MovementMove1StepFowardCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::_1_Step_Forward; }
};
struct MovementMove1StepBackwardCommand : IEventCommand {
  ~MovementMove1StepBackwardCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::_1_Step_Backward; }
};
struct MovementJumpCommand : IEventCommand {
  ~MovementJumpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump; }
  int x;
  int y;
};
struct MovementWaitCommand : IEventCommand {
  ~MovementWaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait_del_; }
  int duration;
};

struct MovementTurnDownCommand : IEventCommand {
  ~MovementTurnDownCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Down; }
};

struct MovementTurnLeftCommand : IEventCommand {
  ~MovementTurnLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Left; }
};
struct MovementTurnRightCommand : IEventCommand {
  ~MovementTurnRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Right; }
};
struct MovementTurnUpCommand : IEventCommand {
  ~MovementTurnUpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_Up; }
};
struct MovementTurn90DegRightCommand : IEventCommand {
  ~MovementTurn90DegRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Left; }
};
struct MovementTurn90DegLeftCommand : IEventCommand {
  ~MovementTurn90DegLeftCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Right; }
};

struct MovementTurn180DegCommand : IEventCommand {
  ~MovementTurn180DegCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_180_deg; }
};
struct MovementTurn90DegLeftOrRightCommand : IEventCommand {
  ~MovementTurn90DegLeftOrRightCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_90_deg_Left_or_Right; }
};
struct MovementTurnAtRandomCommand : IEventCommand {
  ~MovementTurnAtRandomCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_at_Random; }
};
struct MovementTurnTowardPlayerCommand : IEventCommand {
  ~MovementTurnTowardPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_toward_Player; }
};

struct MovementAwayFromPlayerCommand : IEventCommand {
  ~MovementAwayFromPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Turn_away_from_Player; }
};
struct MovementSwitchONCommand : IEventCommand {
  ~MovementSwitchONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_ON; }
  int id;
};

struct MovementSwitchOFFCommand : IEventCommand {
  ~MovementSwitchOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_OFF; }
  int id;
};

struct MovementSpeedCommand : IEventCommand {
  ~MovementSpeedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Speed; }
  int speed;
};

struct MovementFrequencyCommand : IEventCommand {
  ~MovementFrequencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
  int frequency;
};

struct MovementWalkingAnimationONCommand : IEventCommand {
  ~MovementWalkingAnimationONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};

struct MovementWalkingAnimationOFFCommand : IEventCommand {
  ~MovementWalkingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};

struct MovementSteppingAnimationONCommand : IEventCommand {
  ~MovementSteppingAnimationONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};

struct MovementSteppingAnimationOFFCommand : IEventCommand {
  ~MovementSteppingAnimationOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
};
struct MovementDirectionFixONCommand : IEventCommand {
  ~MovementDirectionFixONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Direction_Fix_ON; }
};

struct MovementDirectionFixOFFCommand : IEventCommand {
  ~MovementDirectionFixOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Direction_Fix_OFF; }
};

struct MovementThroughONCommand : IEventCommand {
  ~MovementThroughONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Through_ON; }
};
struct MovementThroughOFFCommand : IEventCommand {
  ~MovementThroughOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Through_OFF; }
};

struct MovementTransparentONCommand : IEventCommand {
  ~MovementTransparentONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transparent_ON; }
};

struct MovementTransparentOFFCommand : IEventCommand {
  ~MovementTransparentOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transparent_OFF; }
};
struct MovementChangeImageCommand : IEventCommand {
  ~MovementChangeImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Image; }

  std::string image;
  int character;
};

struct MovementChangeOpacityCommand : IEventCommand {
  ~MovementChangeOpacityCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Opacity; }

  int opacity;
};

struct MovementChangeBlendModeCommand : IEventCommand {
  ~MovementChangeBlendModeCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Blend_Mode; }

  Blend mode;
};

struct MovementPlaySECommand : IEventCommand {
  ~MovementPlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE_de_Movement; }
  Audio se;
};

struct MovementScriptCommand : IEventCommand {
  ~MovementScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Script_de_Movement; }
  std::string script;
};

// END MOVEMENTROUTE
struct CommandParser {
  nlohmann::json parser;
  int index = 0;

  std::vector<std::shared_ptr<IEventCommand>> parse(const nlohmann::json& data);
  void serialize(nlohmann::json& data, const std::vector<std::shared_ptr<IEventCommand>>& list);

  EventCode nextEventCommand() {
    EventCode ret;
    parser[index + 1].at("code").get_to(ret);
    return ret;
  }

  nlohmann::json currentCommand() { return parser[index]; }
};