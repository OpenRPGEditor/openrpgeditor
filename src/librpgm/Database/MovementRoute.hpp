#pragma once

#include "nlohmann/json.hpp"
#include <memory>
#include <Database/EventCommands/IEventCommand.hpp>

struct IEventCommand;
struct MovementRoute {
  friend void to_json(nlohmann::json& to, const MovementRoute& route);
  friend void from_json(const nlohmann::json& to, MovementRoute& route);

  std::vector<std::shared_ptr<IEventCommand>> list;
  bool repeat;
  bool skippable;
  bool wait;

  [[nodiscard]] bool isDirty() const {
    m_isDirty |= std::any_of(list.begin(), list.end(),[](const auto& cmd) {
      return cmd && cmd->isDirty();
    });

    return m_isDirty;
  }
  void addCommand(const std::shared_ptr<IEventCommand>& command, int position) {

    auto select = list.insert(list.begin() + position, command);
    //m_selectedCommand = select - list->begin();

    //list.push_back(command);
  }

  mutable bool m_isDirty{false};
};