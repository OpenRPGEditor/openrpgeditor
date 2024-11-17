#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/EventCommands/ShopProcessing.hpp"
#include "Database/Items.hpp"
#include "Database/Weapons.hpp"
#include "Database/Armors.hpp"
#include "Database/Database.hpp"

struct Dialog_ShopProcessing_Goods : IEventDialogController {
  Dialog_ShopProcessing_Goods() = delete;
  explicit Dialog_ShopProcessing_Goods(const std::string& name,
                                       const std::shared_ptr<ShopProcessingGoodCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ShopProcessingGoodCommand());
    }
    m_type_selection = static_cast<int>(command->type);
    m_price_operation = static_cast<int>(command->priceType);
    // ID
    if (command->type == ShopType::Armor) {
      m_armor_selection = command->id;
    } else if (command->type == ShopType::Weapon) {
      m_weapon_selection = command->id;
    } else {
      m_item_selection = command->id;
    }
    // Price
    if (m_price_operation == 0) {
      if (command->type == ShopType::Armor) {
        m_price_constant = Database::instance()->armors.armor(m_armor_selection)->price;
      } else if (command->type == ShopType::Weapon) {
        m_price_constant = Database::instance()->weapons.weapon(m_weapon_selection)->price;
      } else {
        m_price_constant = Database::instance()->items.item(m_item_selection)->price;
      }
    }
    else {
      m_price_constant = command->price;
    }
  }
  explicit Dialog_ShopProcessing_Goods(const std::string& name, int id, int price, int shopType, int priceType)
  : IEventDialogController(name) {
    command.reset(new ShopProcessingGoodCommand());
    if (id < 1)
      id = 1;

    m_type_selection = shopType;
    m_price_operation = priceType;
    // ID
    if (shopType == 2) {
      m_armor_selection = id;
    } else if (shopType == 1) {
      m_weapon_selection = id;
    } else if (shopType == 0) {
      m_item_selection = id;
    }
    
    // Price
    if (priceType == 0) {
      if (shopType == 2) {
        m_price_constant = Database::instance()->armors.armor(m_armor_selection)->price;
      } else if (shopType == 1) {
        m_price_constant = Database::instance()->weapons.weapon(m_weapon_selection)->price;
      } else if (shopType == 0) {
        m_price_constant = Database::instance()->items.item(m_item_selection)->price;
      }
    }
    else {
      m_price_constant = price;
    }
  }

  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_type_selection{0};
  int m_item_selection{1};
  int m_weapon_selection{1};
  int m_armor_selection{1};
  int m_price_operation{0};
  int m_price_constant{0};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Item>> item_picker;
  std::optional<ObjectPicker<Weapon>> weapon_picker;
  std::optional<ObjectPicker<Armor>> armor_picker;
  std::shared_ptr<ShopProcessingGoodCommand> command;
  std::tuple<bool, bool> result;
};