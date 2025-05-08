#pragma once
#include "nano_signal_slot.hpp"

namespace rpgmutils {
using SignalPolicy = Nano::ST_Policy_Safe;
template <typename Signature>
using signal = Nano::Signal<Signature, SignalPolicy>;
using observer = Nano::Observer<SignalPolicy>;
} // namespace rpgmutils

template <typename Signal, typename... Args>
static inline void emit_signal(Signal& signal, Args... args) {
  signal.fire(std::forward<Args>(args)...);
}