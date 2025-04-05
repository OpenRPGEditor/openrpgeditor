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
  MovementRoute(const MovementRoute&);
  MovementRoute& operator=(const MovementRoute&);
  MovementRoute(MovementRoute&&) noexcept;
  MovementRoute& operator=(MovementRoute&&) noexcept;

  std::vector<std::shared_ptr<IEventCommand>>& list();
  const std::vector<std::shared_ptr<IEventCommand>>& list() const;
  void setList(const std::vector<std::shared_ptr<IEventCommand>>& list);
  bool repeat() const;
  void setRepeat(bool repeat);
  bool skippable() const;
  void setSkippable(bool skippable);
  bool wait() const;
  void setWait(bool wait);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  [[nodiscard]] bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_list, [](const auto& cmd) { return cmd && cmd->isModified(); });
  }
  int addCommand(const std::shared_ptr<IEventCommand>& command, int position);

  MovementRoute clone() const { return MovementRoute(*this, 1); }
  bool operator==(const MovementRoute& other) const { return m_list == other.m_list && m_repeat == other.m_repeat && m_skippable == other.m_skippable && m_wait == other.m_wait; }

  rpgmutils::signal<void(MovementRoute*, const std::vector<std::shared_ptr<IEventCommand>>&)>& listModified();
  rpgmutils::signal<void(MovementRoute*, bool)>& repeatModified();
  rpgmutils::signal<void(MovementRoute*, bool)>& skippableModified();
  rpgmutils::signal<void(MovementRoute*, bool)>& waitModified();

private:
  MovementRoute(const MovementRoute& other, int)
  : IModifiable(other)
  , m_repeat(other.m_repeat)
  , m_skippable(other.m_skippable)
  , m_wait(other.m_wait) {
    for (const auto& cmd : other.m_list) {
      m_list.emplace_back(cmd->clone());
    }
  }

  std::vector<std::shared_ptr<IEventCommand>> m_list;
  bool m_repeat;
  bool m_skippable;
  bool m_wait;

  std::optional<std::vector<std::shared_ptr<IEventCommand>>> m_oldlist;
  std::optional<bool> m_oldrepeat;
  std::optional<bool> m_oldskippable;
  std::optional<bool> m_oldwait;

  std::optional<rpgmutils::signal<void(MovementRoute*, const std::vector<std::shared_ptr<IEventCommand>>&)>> m_listModified;
  std::optional<rpgmutils::signal<void(MovementRoute*, bool)>> m_repeatModified;
  std::optional<rpgmutils::signal<void(MovementRoute*, bool)>> m_skippableModified;
  std::optional<rpgmutils::signal<void(MovementRoute*, bool)>> m_waitModified;
};