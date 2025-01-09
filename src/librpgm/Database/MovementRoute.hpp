#pragma once

#include "nlohmann/json.hpp"
#include <Database/EventCommands/IEventCommand.hpp>
#include <memory>

struct IEventCommand;
class MovementRoute final : public IModifiable {
public:
  friend void to_json(nlohmann::ordered_json& to, const MovementRoute& route);
  friend void from_json(const nlohmann::ordered_json& to, MovementRoute& route);
  MovementRoute() = default;
  MovementRoute(const MovementRoute&) = default;
  MovementRoute& operator=(const MovementRoute&) = default;
  MovementRoute(MovementRoute&&) = default;
  MovementRoute& operator=(MovementRoute&&) = default;

  std::vector<std::shared_ptr<IEventCommand>> list;
  bool repeat;
  bool skippable;
  bool wait;

  [[nodiscard]] bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(list, [](const auto& cmd) { return cmd && cmd->isModified(); });
  }
  int addCommand(const std::shared_ptr<IEventCommand>& command, int position) {
    if (list.size() == 1) {
      position = 0;
    }
    if (position < 0)
      position = 0;
    auto select = list.insert(list.begin() + position, command);
    setHasChanges(true);
    return select - list.begin();
  }

  MovementRoute clone() const { return MovementRoute(*this, 1); }

  bool operator==(const MovementRoute& other) const { return list == other.list && repeat == other.repeat && skippable == other.skippable && wait == other.wait; }

private:
  MovementRoute(const MovementRoute& other, int) : IModifiable(other), repeat(other.repeat), skippable(other.skippable), wait(other.wait) {
    for (const auto& cmd : other.list) {
      list.emplace_back(cmd->clone());
    }
  }
};