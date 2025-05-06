#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Class.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/ControlVariables.hpp"
#include "Database/Items.hpp"

struct Dialog_GameData : IEventDialogController {
  Dialog_GameData() = delete;
  explicit Dialog_GameData(const std::string& name, const int type, const int rawSource, const int value = 0, const int troopId = 0)
  : IEventDialogController(name)
  , m_troopId(troopId)
  , m_type(type) {
    switch (static_cast<GameDataType>(m_type)) {
    case GameDataType::Item:
      m_itemValue = value;
      break;
    case GameDataType::Weapon:
      m_weaponValue = value;
      break;
    case GameDataType::Armor:
      m_armorValue = value;
      break;
    case GameDataType::Actor:
      m_actorSource = rawSource;
      m_actorValue = value;
      break;
    case GameDataType::Enemy:
      m_enemySource = rawSource;
      m_enemyValue = value;
      break;
    case GameDataType::Character:
      m_characterSource = rawSource;
      m_characterValue = value;
      break;
    case GameDataType::Party:
      m_partyValue = value;
      break;
    case GameDataType::Other:
      m_otherValue = value;
      break;
    }
    m_command.gameData.value = 0;
  }
  std::tuple<bool, bool> draw() override;

  ControlVariables& getData() { return m_command; }
  const ControlVariables& getData() const { return m_command; }

private:
  void drawPickers();
  int m_troopId{0};
  bool m_confirmed{false};

  int m_type;

  int m_itemValue{1};
  int m_weaponValue{1};
  int m_armorValue{1};
  int m_actorValue{1};
  int m_actorSource{0};
  int m_enemyValue{0};
  int m_enemySource{0};
  int m_characterValue{-1};
  int m_characterSource{0};
  int m_partyValue{0};
  int m_otherValue{0};

  static constexpr auto ActorData = magic_enum::enum_values<ActorDataSource>();
  static constexpr auto EnemyData = magic_enum::enum_values<EnemyDataSource>();
  static constexpr auto CharacterData = magic_enum::enum_values<CharacterDataSource>();
  static constexpr auto OtherSource = magic_enum::enum_values<OtherDataSource>();

  ControlVariables m_command;
  std::optional<ObjectPicker<Item>> m_itemPicker;
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::optional<ObjectPicker<Armor>> m_armorPicker;
  std::optional<ObjectPicker<Weapon>> m_weaponPicker;
};