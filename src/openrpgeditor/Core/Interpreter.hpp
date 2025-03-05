#pragma once

#include "Core/Graphics/Image.hpp"
#include "Database/EventCommands/IEventCommand.hpp"

#include <string>
#include <vector>

class Interpreter {
public:
  explicit Interpreter(std::vector<std::shared_ptr<IEventCommand>> commands);
  bool update();
  Image& getImage() { return m_image.value(); }
  [[nodiscard]] bool image() const { return m_image.has_value(); }
  std::vector<int> getKeyFrames() const;
  std::vector<int> getSwitches();
  std::vector<int> getVariables();

  bool hasVariable(int index);
  bool hasSwitch(int index);
  void setVariable(int index, int value);
  void setSwitch(int index, bool value);
  void setIndex(int index, int keyFrame);
  std::string findStringMatch(std::string text);
  bool isStringMatch(std::string text) const;
  int getKeyFrameIndex() const { return m_keyFrame; }
  void setFastForward(bool cond) { m_isFastForward = cond; }

private:
  int m_index{0};
  int m_keyFrame{0};
  int m_wait{0};

  bool m_isFastForward{false};
  bool m_isSkip{false}; // For branch condition parsing
  std::optional<Image> m_image;
  std::vector<std::shared_ptr<IEventCommand>> m_commands;

  std::map<int, int> m_variableMap;
  std::map<int, bool> m_switchMap;
};
