#pragma once

#include "Database/Messages.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class Terms final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& j, const Terms& terms);
  friend void from_json(const nlohmann::ordered_json& j, Terms& terms);

public:
  Terms() = default;
  Terms(const Terms& other);
  Terms& operator=(const Terms& other);
  Terms(Terms&& other) noexcept;
  Terms& operator=(Terms&& other) noexcept;

  std::vector<std::optional<std::string>>& basic();
  const std::vector<std::optional<std::string>>& basic() const;
  void setBasic(std::vector<std::optional<std::string>>& value);

  std::vector<std::optional<std::string>>& commands();
  const std::vector<std::optional<std::string>>& commands() const;
  void setCommands(std::vector<std::optional<std::string>>& value);

  Messages& messages();
  const Messages& messages() const;
  void setMessages(const Messages& value);

  std::vector<std::optional<std::string>>& params();
  const std::vector<std::optional<std::string>>& params() const;
  void setParams(std::vector<std::optional<std::string>>& value);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  bool isModified() const override { return IModifiable::isModified() | m_messages.isModified(); }

  rpgmutils::signal<void(Terms*, const std::vector<std::optional<std::string>>&)>& basicModified();
  rpgmutils::signal<void(Terms*, const std::vector<std::optional<std::string>>&)>& commandsModified();
  rpgmutils::signal<void(Terms*, const Messages&)>& messagesModified();
  rpgmutils::signal<void(Terms*, const std::vector<std::optional<std::string>>&)>& paramsModified();

  bool operator==(const Terms& rhs) const { return m_basic == rhs.m_basic && m_commands == rhs.m_commands && m_params == rhs.m_params && m_messages == rhs.m_messages; }

private:
  std::vector<std::optional<std::string>> m_basic;
  std::vector<std::optional<std::string>> m_commands;
  Messages m_messages;
  std::vector<std::optional<std::string>> m_params;

  std::optional<std::vector<std::optional<std::string>>> m_oldbasic;
  std::optional<std::vector<std::optional<std::string>>> m_oldcommands;
  std::optional<Messages> m_oldmessages;
  std::optional<std::vector<std::optional<std::string>>> m_oldparams;

  std::optional<rpgmutils::signal<void(Terms*, const std::vector<std::optional<std::string>>&)>> m_basicModified;
  std::optional<rpgmutils::signal<void(Terms*, const std::vector<std::optional<std::string>>&)>> m_commandsModified;
  std::optional<rpgmutils::signal<void(Terms*, const Messages&)>> m_messagesModified;
  std::optional<rpgmutils::signal<void(Terms*, const std::vector<std::optional<std::string>>&)>> m_paramsModified;
};

void to_json(nlohmann::ordered_json& j, const Terms& terms);
void from_json(const nlohmann::ordered_json& j, Terms& terms);