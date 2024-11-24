#include "Core/EventCommands/Dialog_ReturnToTitle.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_ReturnToTitle::draw() { return std::make_tuple(!m_open, m_confirmed); }
