#pragma once
#include "Database/ColorFormatter.hpp"
#include "Database/Globals.hpp"
#include "nlohmann/json.hpp"

struct Database;
struct IEventCommand {
  static constexpr std::string_view diamond = "\u25c6";
  static constexpr std::string_view colon = "\uff1a";

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
  virtual void adjustIndent(int offset) {
    if (indent) {
      indent.value() += offset;
    } else {
      indent.emplace(0);
      indent.value() += offset;
    }
  }

  [[nodiscard]] virtual constexpr bool collapsable() const { return false; }
  [[nodiscard]] virtual bool isCollapsed() const { return false; }
  virtual void setCollapsed(bool collapsed) {}
  [[nodiscard]] virtual bool isPartner(EventCode code, const std::optional<int>& codeIndent) { return false; }
  [[nodiscard]] virtual constexpr bool hasPartner() const { return false; }
  [[nodiscard]] virtual constexpr int partnerCount() const { return 0; }
  [[nodiscard]] virtual bool reverseSelection() const { return false; }
  virtual void setReverseSelection(bool reverseSelection) {}

  std::optional<int> indent{};
  mutable bool m_isDirty{false};
};