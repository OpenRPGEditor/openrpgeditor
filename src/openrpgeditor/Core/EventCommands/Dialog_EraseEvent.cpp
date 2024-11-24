#include "Core/EventCommands/Dialog_EraseEvent.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_EraseEvent::draw() { return std::make_tuple(!m_open, m_confirmed); }
