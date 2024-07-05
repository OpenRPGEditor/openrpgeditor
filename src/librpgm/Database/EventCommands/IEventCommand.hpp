#pragma once
#include "Database/Globals.hpp"
#include "Database/ColorFormatter.hpp"
#include "nlohmann/json.hpp"

struct Database;
struct IEventCommand {
  std::optional<int> indent{};
  mutable bool m_isDirty{false};

  static constexpr std::string_view diamond = "\u25c6";
  static constexpr std::string_view colon = "\uff1a";

  IEventCommand() = default;
  explicit IEventCommand(const std::optional<int>& _indent, [[maybe_unused]] nlohmann::json& parameters)
  : indent(_indent) {}

  virtual ~IEventCommand() = default;
  [[nodiscard]] virtual EventCode code() const = 0;
  [[nodiscard]] virtual bool isDirty() const { return m_isDirty; }
  virtual void serializeParameters(nlohmann::json& out) {}

  void serialize(nlohmann::json& out, const bool doIndent = true, const bool doParameters = true) {
    out["code"] = code();
    if (doIndent) {
      out["indent"] = indent;
    }
    if (doParameters) {
      out["parameters"] = nlohmann::json::array();
      serializeParameters(out["parameters"]);
    }
  }

  [[nodiscard]] virtual std::string stringRep(const Database& db) const {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + DecodeEnumName(code()) +
           ColorFormatter::popColor();
  }
  virtual std::string symbol(EventCode code) const {
    return static_cast<int>(code) < 400 ? diamond.data() : colon.data();
  }
  virtual std::string indentText(std::optional<int> indent) const {
    std::string text = "";
    for (int i = 0; i < indent; ++i) {
      text += "  ";
    }
    return text;
  }
};