#pragma once

#include "nlohmann/json.hpp"
#include <Database/EventCommands/IEventCommand.hpp>
#include <memory>

struct IEventCommand;
struct MovementRoute {
  friend void to_json(nlohmann::ordered_json& to, const MovementRoute& route);
  friend void from_json(const nlohmann::ordered_json& to, MovementRoute& route);

  std::vector<std::shared_ptr<IEventCommand>> list;
  bool repeat;
  bool skippable;
  bool wait;

  [[nodiscard]] bool isModified() const {
    m_isDirty |= std::ranges::any_of(list, [](const auto& cmd) { return cmd && cmd->isModified(); });

    return m_isDirty;
  }
  int addCommand(const std::shared_ptr<IEventCommand>& command, int position) {
    if (list.size() == 1) {
      position = 0;
    }
    if (position < 0)
      position = 0;
    auto select = list.insert(list.begin() + position, command);
    m_isDirty = true;
    return select - list.begin();
  }

  mutable bool m_isDirty{false};
};