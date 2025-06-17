#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Armors.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/ShopProcessing.hpp"
#include "Database/Items.hpp"
#include "Database/Weapons.hpp"

struct Dialog_ShopProcessing_Goods : IEventDialogController {
  Dialog_ShopProcessing_Goods() = delete;
  explicit Dialog_ShopProcessing_Goods(const std::string& name, const std::shared_ptr<ShopProcessingGoodCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ShopProcessingGoodCommand());
    }
    m_typeSelection = static_cast<int>(m_command->type);
    m_priceOperation = static_cast<int>(m_command->priceType);
    // ID
    if (m_command->type == ShopType::Armor) {
      m_armorSelection = m_command->id;
    } else if (m_command->type == ShopType::Weapon) {
      m_weaponSelection = m_command->id;
    } else {
      m_itemSelection = m_command->id;
    }
    // Price
    if (m_priceOperation == 0) {
      if (m_command->type == ShopType::Armor) {
        m_priceConstant = Database::instance()->armors.armor(m_armorSelection)->price();
      } else if (m_command->type == ShopType::Weapon) {
        m_priceConstant = Database::instance()->weapons.weapon(m_weaponSelection)->price();
      } else {
        m_priceConstant = Database::instance()->items.item(m_itemSelection)->price();
      }
    } else {
      m_priceConstant = m_command->price;
    }
  }
  explicit Dialog_ShopProcessing_Goods(const std::string& name, int id, const int price, const int shopType, const int priceType)
  : IEventDialogController(name) {
    m_command.reset(new ShopProcessingGoodCommand());
    if (id < 1)
      id = 1;

    m_typeSelection = shopType;
    m_priceOperation = priceType;
    // ID
    if (shopType == 2) {
      m_armorSelection = id;
    } else if (shopType == 1) {
      m_weaponSelection = id;
    } else if (shopType == 0) {
      m_itemSelection = id;
    }

    // Price
    if (priceType == 0) {
      if (shopType == 2) {
        m_priceConstant = Database::instance()->armors.armor(m_armorSelection)->price();
      } else if (shopType == 1) {
        m_priceConstant = Database::instance()->weapons.weapon(m_weaponSelection)->price();
      } else if (shopType == 0) {
        m_priceConstant = Database::instance()->items.item(m_itemSelection)->price();
      }
    } else {
      m_priceConstant = price;
    }
  }

  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_typeSelection{0};
  int m_itemSelection{1};
  int m_weaponSelection{1};
  int m_armorSelection{1};
  int m_priceOperation{0};
  int m_priceConstant{0};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Item>> m_itemPicker;
  std::optional<ObjectPicker<Weapon>> m_weaponPicker;
  std::optional<ObjectPicker<Armor>> m_armorPicker;
  std::shared_ptr<ShopProcessingGoodCommand> m_command;
};