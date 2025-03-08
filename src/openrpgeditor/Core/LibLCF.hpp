#pragma once
#include "Database/Event.hpp"
#include "Database/EventCommands/MovementRoute/Jump.hpp"
#include "Database/Map.hpp"
#include "LCF_Importer/LCF_Importer.hpp"
#include "imgui.h"

struct MainWindow;
class LibLCF {
public:
  explicit LibLCF(MainWindow* parent) : m_parent(parent) {}
  void draw();
  [[nodiscard]] bool isOpen() const { return m_isOpen; }
  void open() { m_isOpen = true; }

  std::shared_ptr<IEventCommand> Comment(std::string codeName, std::string text, int indent);
  void attachToCommand(std::string text);

  void convertEvent(Event* event, const lcf::rpg::Event& ev);
  void convertPage(EventPage* page, const lcf::rpg::EventPage& evPage);
  void processJumpParameters(int32_t code, std::shared_ptr<IEventCommand>& list);
  std::shared_ptr<IEventCommand> createCommand(int32_t code, int32_t indent, lcf::DBArray<int32_t> data, const std::string& string);
  void convertCommands(std::vector<std::shared_ptr<IEventCommand>>* r_cmds, const std::vector<lcf::rpg::EventCommand>& s_cmds);
  std::shared_ptr<IEventCommand> convertMovementCommand(lcf::rpg::MoveCommand moveCmd, int32_t indent);
  void convertPageConditions();

  MoveType getMoveType(int32_t move_type);
  MovementFrequency getMoveFrequency(int32_t move_freq);
  Direction getDirection(int32_t direction);
  lcf::rpg::MoveCommand DecodeMove(lcf::DBArray<int32_t>::const_iterator& it);
  int DecodeInt(lcf::DBArray<int32_t>::const_iterator& it);
  const std::string DecodeString(lcf::DBArray<int32_t>::const_iterator& it);

private:
  MainWindow* m_parent;

  void log(const std::string& text) { m_lcfLogger.push_back(text); }

  // LCF
  LCF_Importer lcf;
  std::unique_ptr<lcf::rpg::Map> map;

  bool m_isOpen{false};
  bool m_navigate{false};
  bool m_attachToCommand{false};
  bool m_attachDummy{false};
  bool m_movementProcessing{false};
  bool m_jumpProcessing{false};

  // For Message Options
  bool m_windowTransparent{false};
  int m_windowPosition{2}; // 0 = up; 1 = middle; 2 = down (default)

  int selectedMapIndex = -1;
  int selectedEvent = -1;
  int selectedPage = -1;

  std::vector<std::string> m_lcfLogger;
  std::vector<std::string> m_commandLogger;
};
