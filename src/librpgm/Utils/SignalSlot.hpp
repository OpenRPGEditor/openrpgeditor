#pragma once
#include "nano_signal_slot.hpp"

namespace rpgmutils {
using SignalPolicy = Nano::TS_Policy_Safe<>;
template <typename Signature>
using signal = Nano::Signal<Signature, SignalPolicy>;
using observer = Nano::Observer<SignalPolicy>;
} // namespace rpgmutils