#pragma once

#include "Database/EventCommands/IEventCommand.hpp"
#include "Editor/Interpreter.hpp"
#include "IDialogController.hpp"

#include <vector>

class Preview : public IDialogController {
public:
  Preview(int commonId);
  std::tuple<bool, bool> draw() override;

private:
  std::optional<Interpreter> m_interpreter;

  bool m_isPaused{false};
  bool m_isFastForward{false};
  int m_selectedKeyFrame{1};
  int m_inputInt{0};

  std::vector<int> m_keyFrames;
  std::map<int, int> m_variables;
  std::map<int, bool> m_switches;
};
