#pragma once

#include <format>
#include <libintl.h>

/**
 * Wrapper around std::vformat to retrieve a translated string
 * @param fmt String to translate
 */
template <typename... Args>
static inline std::string trFormat(std::string_view fmt, Args&&... args) {
  return std::vformat(gettext(fmt.data()), std::make_format_args(args...));
}

static inline std::string tr(std::string_view str) { return gettext(str.data()); }

#define trNOOP(S) gettext(S)

#define trENUM(S, X) X
#define trENUMAlt(S, X) X