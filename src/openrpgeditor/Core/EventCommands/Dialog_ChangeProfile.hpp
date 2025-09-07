#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/EventCommands/ChangeProfile.hpp"

struct Dialog_ChangeProfile : IEventDialogController {
  Dialog_ChangeProfile() = delete;
  explicit Dialog_ChangeProfile(const std::string& name, const std::shared_ptr<ChangeProfileCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeProfileCommand());
    }
    m_actor = m_command->actor;
    m_profile = m_command->profile;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  friend int multilineTextCallback(ImGuiInputTextCallbackData* data);

  void drawPickers();
  static int multilineTextCallback(ImGuiInputTextCallbackData* data);

  int m_actor;
  std::string m_profile;

  bool m_confirmed{false};
  std::optional<ActorPicker> m_actorPicker;
  std::shared_ptr<ChangeProfileCommand> m_command;
};