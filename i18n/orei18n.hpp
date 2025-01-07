#pragma once

#include <format>
#include <moloader/moloader.hpp>

/**
 * Wrapper around std::vformat to retrieve a translated string
 * @param fmt String to translate
 * @param args
 * @return Translated string
 */
template <typename... Args>
static inline std::string trFormat(const std::string_view fmt, Args&&... args) {
  return std::vformat(moloader::getstring(fmt.data()), std::make_format_args(args...));
}
static inline std::string tr(std::string_view str) { return moloader::getstring(str.data()); }
#define trNOOP(S) moloader::gettext(S)
#define trENUM(S, X) X
#define trENUMAlt(S, X) X