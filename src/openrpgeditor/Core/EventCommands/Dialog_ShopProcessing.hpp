#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ShopProcessing.hpp"
#include "Core/Log.hpp"

struct Project;
struct Dialog_ShopProcessing : IEventDialogController {
  Dialog_ShopProcessing() = delete;
  explicit Dialog_ShopProcessing(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ShopProcessingCommand());
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_goods_selection;
  std::vector<std::shared_ptr<ShopProcessingGoodCommand>> m_goods;

  bool m_confirmed{false};
  std::shared_ptr<ShopProcessingCommand> command;
  std::tuple<bool, bool> result;
};