#pragma once
#include "Database/Globals.hpp"
#include "Database/ColorFormatter.hpp"
#include "nlohmann/json.hpp"

struct Database;
struct IEventCommand {
#pragma warning(push)
#pragma warning(disable : 4566)
  static constexpr std::string_view diamond = "\u25c6";
  static constexpr std::string_view colon = "\uff1a";
#pragma warning(pop)

  IEventCommand() = default;
  explicit IEventCommand(const std::optional<int>& _indent, [[maybe_unused]] const nlohmann::ordered_json& parameters);
  virtual ~IEventCommand() = default;
  [[nodiscard]] virtual EventCode code() const = 0;
  [[nodiscard]] virtual bool isDirty() const { return m_isDirty; }
  virtual void serializeParameters(nlohmann::ordered_json& out) const {}
  virtual void serialize(nlohmann::ordered_json& out, bool doIndent = true, bool doParameters = true) const;
  [[nodiscard]] virtual std::string stringRep(const Database& db) const;
  virtual std::string symbol(EventCode code) const;
  virtual std::string indentText(std::optional<int> indent) const;

  std::optional<int> indent{};
  mutable bool m_isDirty{false};
};