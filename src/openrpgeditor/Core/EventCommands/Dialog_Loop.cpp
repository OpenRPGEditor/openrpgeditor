#include "Core/EventCommands/Dialog_Loop.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_Loop::draw() { return std::make_tuple(!m_open, m_confirmed); }
