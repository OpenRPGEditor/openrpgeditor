#pragma once
#include "Database/Globals.hpp"
#include "Database/Audio.hpp"
#include "nlohmann/json.hpp"

#include <memory>

struct IEventCommand {
  int indent;
  virtual ~IEventCommand() = default;
  [[nodiscard]] virtual EventCode code() const = 0;
};

struct EventDummy : IEventCommand {
  ~EventDummy() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Event_Dummy; }
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
  std::vector<std::shared_ptr<NextCommentCommand>> text;
};

struct ConditionalBranchCommand : IEventCommand {
  ConditionalBranchCommand() {}
  ~ConditionalBranchCommand() override {}
  [[nodiscard]] EventCode code() const override { return EventCode::Comment; }
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
      FacingDirection facing;
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
  FacingDirection direction;
};

struct PlaySECommand : IEventCommand {
  ~PlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE; }
};

struct EndCommand : IEventCommand {
  ~EndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End; }
};

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