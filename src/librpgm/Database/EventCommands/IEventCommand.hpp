#pragma once
#include "Database/ColorFormatter.hpp"
#include "Database/Globals.hpp"
#include "Database/IModifiable.hpp"
#include "nlohmann/json.hpp"

struct Database;
class IEventCommand : public IModifiable {
  friend class CommandParser;

public:
  static constexpr std::string_view diamond = "\u25c6";
  static constexpr std::string_view colon = "\uff1a";

  IEventCommand() = default;
  explicit IEventCommand(const std::optional<int>& _indent, [[maybe_unused]] const nlohmann::ordered_json& parameters);
  ~IEventCommand() override = default;
  [[nodiscard]] virtual EventCode code() const = 0;
  virtual void serializeParameters(nlohmann::ordered_json& out) const {}
  virtual void serialize(nlohmann::ordered_json& out, bool doIndent = true, bool doParameters = true, bool oldValues = false) const;
  [[nodiscard]] virtual std::string stringRep(const Database& db) const;
  virtual std::string symbol(EventCode code) const;
  virtual std::string indentText(std::optional<int> indent) const;
  virtual void adjustIndent(const int offset) {
    if (m_indent) {
      m_indent.value() += offset;
    } else {
      m_indent.emplace(0);
      m_indent.value() += offset;
    }
    setModified();
  }

  [[nodiscard]] virtual constexpr bool collapsable() const { return false; }
  [[nodiscard]] virtual bool isCollapsed() const { return false; }
  [[nodiscard]] virtual bool isCollapseEnd() const { return true; }
  virtual void setCollapsed(bool collapsed) {}
  [[nodiscard]] virtual bool isPartner(EventCode code, const std::optional<int>& codeIndent) { return false; }
  [[nodiscard]] virtual constexpr bool hasPartner() const { return false; }
  [[nodiscard]] virtual constexpr int partnerCount() const { return 0; }
  [[nodiscard]] virtual bool reverseSelection() const { return false; }
  [[nodiscard]] virtual bool isParent() const { return false; }
  virtual void setReverseSelection(bool reverseSelection) {}

  std::optional<int> indent() const { return m_indent; }
  void setIndent(int indent) {
    m_indent = indent;
    setModified();
  }

  virtual std::shared_ptr<IEventCommand> clone() const = 0;

private:
  std::optional<int> m_indent{};
};