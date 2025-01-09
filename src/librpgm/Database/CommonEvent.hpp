#pragma once

#include "Database/CommandParser.hpp"
#include "Database/Globals.hpp"
#include "EventCommands/EventDummy.hpp"

class CommonEvent final : public IModifiable {
  friend class CommonEventsSerializer;
  friend void to_json(nlohmann::ordered_json& to, const CommonEvent& event);
  friend void from_json(const nlohmann::ordered_json& from, CommonEvent& event);

public:
  CommonEvent() = default;
  CommonEvent(const CommonEvent& other);
  CommonEvent& operator=(const CommonEvent& other);
  CommonEvent(CommonEvent&& other) noexcept;
  CommonEvent& operator=(CommonEvent&& other) noexcept;

  [[nodiscard]] int id() const;
  void setId(int id);

  std::vector<std::shared_ptr<IEventCommand>>& commands();
  const std::vector<std::shared_ptr<IEventCommand>>& commands() const;
  void setCommands(const std::vector<std::shared_ptr<IEventCommand>>& commands);

  const std::string& name() const;
  void setName(const std::string& name);

  int switchId() const;
  void setSwitchId(int id);

  CommonEventTriggerType trigger() const;
  void setTrigger(CommonEventTriggerType trigger);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;
  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_commands, [](const std::shared_ptr<IEventCommand>& command) { return command->isModified(); });
  }
  rpgmutils::signal<void(CommonEvent*, int)>& idModified();
  rpgmutils::signal<void(CommonEvent*, const std::vector<std::shared_ptr<IEventCommand>>&)>& commandsModified();
  rpgmutils::signal<void(CommonEvent*, const std::string&)>& nameModified();
  rpgmutils::signal<void(CommonEvent*, int)>& switchIdModified();
  rpgmutils::signal<void(CommonEvent*, CommonEventTriggerType)>& triggerModified();

  CommonEvent clone() const { return CommonEvent(*this, 1); }

private:
  CommonEvent(const CommonEvent& other, int) : IModifiable(other) {
    m_id = other.m_id;
    m_name = other.m_name;
    m_switchId = other.m_switchId;
    m_trigger = other.m_trigger;
    for (const auto& command : other.m_commands) {
      m_commands.emplace_back(command->clone());
    }
  };
  int m_id{};
  std::vector<std::shared_ptr<IEventCommand>> m_commands;
  std::string m_name{};
  int m_switchId{};
  CommonEventTriggerType m_trigger;

  std::optional<int> m_oldid;
  std::optional<std::vector<std::shared_ptr<IEventCommand>>> m_oldcommands;
  std::optional<std::string> m_oldname;
  std::optional<int> m_oldswitchId;
  std::optional<CommonEventTriggerType> m_oldtrigger;

  std::optional<rpgmutils::signal<void(CommonEvent*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(CommonEvent*, const std::vector<std::shared_ptr<IEventCommand>>&)>> m_commandsModified;
  std::optional<rpgmutils::signal<void(CommonEvent*, const std::string&)>> m_nameModified;
  std::optional<rpgmutils::signal<void(CommonEvent*, int)>> m_switchIdModified;
  std::optional<rpgmutils::signal<void(CommonEvent*, CommonEventTriggerType)>> m_triggerModified;
};

void to_json(nlohmann::ordered_json& to, const CommonEvent& event);
void from_json(const nlohmann::ordered_json& from, CommonEvent& event);