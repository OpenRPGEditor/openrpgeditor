#pragma once
#include "Database/EventCommands/ShopProcessing.hpp"
#include "Editor/EventCommands/Dialog_ShopProcessing_Goods.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"
#include "Editor/Log.hpp"

struct Dialog_ShopProcessing : IEventDialogController {
  Dialog_ShopProcessing() = delete;
  explicit Dialog_ShopProcessing(const std::string& name, const std::shared_ptr<ShopProcessingCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ShopProcessingCommand());
    } else {
      const auto& good = m_goods.emplace_back(new ShopProcessingGoodCommand());
      good->id = m_command->id;
      good->type = m_command->type;
      good->price = m_command->price;
      good->priceType = m_command->priceType;
      for (const auto& g : m_command->goods) {
        m_goods.emplace_back(std::dynamic_pointer_cast<ShopProcessingGoodCommand>(g->clone()));
      }
    }
    m_purchaseOnly = m_command->purchaseOnly;
  }

  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_selection{0};
  std::vector<std::shared_ptr<ShopProcessingGoodCommand>> m_goods;
  bool m_purchaseOnly;

  bool m_confirmed{false};
  std::shared_ptr<ShopProcessingCommand> m_command;
  std::optional<Dialog_ShopProcessing_Goods> m_goodsDialog;
};