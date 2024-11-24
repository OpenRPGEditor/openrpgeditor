#include "Core/EventCommands/Dialog_BreakLoop.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_BreakLoop::draw() { return std::make_tuple(!m_open, m_confirmed); }
