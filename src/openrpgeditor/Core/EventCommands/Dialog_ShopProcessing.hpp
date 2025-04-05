#pragma once
#include "Core/EventCommands/Dialog_ShopProcessing_Goods.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Database/EventCommands/ShopProcessing.hpp"

struct Dialog_ShopProcessing : IEventDialogController {
  Dialog_ShopProcessing() = delete;
  explicit Dialog_ShopProcessing(const std::string& name, const std::shared_ptr<ShopProcessingCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ShopProcessingCommand());
    }
    m_id = command->id;
    m_type = static_cast<int>(command->type);
    m_priceType = static_cast<int>(command->priceType);
    m_price = command->price;

    m_goods = command->goods;
    m_purchaseOnly = command->purchaseOnly;
  }

  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_goods_selection{0};
  std::vector<std::shared_ptr<ShopProcessingGoodCommand>> m_goods;
  bool m_purchaseOnly;

  int m_type;
  int m_priceType;
  int m_id;
  int m_price;
  int m_selection_type;

  bool m_confirmed{false};
  std::shared_ptr<ShopProcessingCommand> command;

  std::optional<Dialog_ShopProcessing_Goods> goodsDialog;
  std::tuple<bool, bool> result;
};