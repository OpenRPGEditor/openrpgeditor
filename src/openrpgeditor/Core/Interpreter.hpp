#pragma once

#include "Core/Graphics/Image.hpp"
#include "Database/EventCommands/IEventCommand.hpp"

#include <string>
#include <vector>

class Interpreter {
public:
  explicit Interpreter(const std::vector<std::shared_ptr<IEventCommand>>& commands);
  bool update();
  [[nodiscard]] Image& getImage() { return m_image.value(); }
  [[nodiscard]] bool image() const { return m_image.has_value(); }
  [[nodiscard]] std::vector<int> getKeyFrames() const;
  [[nodiscard]] std::vector<int> getSwitches();
  [[nodiscard]] std::vector<int> getVariables();

  [[nodiscard]] bool hasVariable(const int index) const { return m_variableMap.contains(index); }
  [[nodiscard]] bool hasSwitch(const int index) const { return m_switchMap.contains(index); }
  
  void setVariable(int index, int value);
  void setSwitch(int index, bool value);
  void setIndex(int index, int keyFrame);
  static std::string findStringMatch(const std::string& text);
  static bool isStringMatch(const std::string& text);
  [[nodiscard]] int getKeyFrameIndex() const { return m_keyFrame; }
  void setFastForward(const bool cond) { m_isFastForward = cond; }

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
