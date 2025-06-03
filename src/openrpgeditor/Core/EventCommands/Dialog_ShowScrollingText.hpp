#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Database/EventCommands/ScrollingText.hpp"

struct Dialog_ShowScrollingText : IEventDialogController {
  Dialog_ShowScrollingText() = delete;
  explicit Dialog_ShowScrollingText(const std::string& name, const std::shared_ptr<ShowScrollTextCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ShowScrollTextCommand());
    }
    m_speed = m_command->speed;
    m_noFast = m_command->noFast;

    for (const auto& str : m_command->text) {
      m_textLine += !m_textLine.empty() ? "\n" + str->text : str->text;
    }
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_speed;
  bool m_noFast;
  std::string m_textLine;

  bool m_confirmed{false};
  std::shared_ptr<ShowScrollTextCommand> m_command;
};