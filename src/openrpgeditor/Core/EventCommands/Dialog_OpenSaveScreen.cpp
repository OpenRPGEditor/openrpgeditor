#include "Core/EventCommands/Dialog_OpenSaveScreen.hpp"
#include <tuple>

std::tuple<bool, bool> Dialog_OpenSaveScreen::draw() { return std::make_tuple(!m_open, m_confirmed); }
