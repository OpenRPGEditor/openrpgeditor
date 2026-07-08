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
  explicit IEventCommand(const std::optional<int>& _indent);

  IEventCommand(const IEventCommand& other) = default;
  IEventCommand& operator=(const IEventCommand& other) = default;
  IEventCommand(IEventCommand&& other) noexcept
  : IModifiable(std::move(other))
  , m_indent(other.m_indent) {}

  IEventCommand& operator=(IEventCommand&& other) noexcept {
    IModifiable::operator=(std::move(other));
    m_indent = other.m_indent;
    return *this;
  }

  ~IEventCommand() override = default;
  [[nodiscard]] virtual EventCode code() const = 0;
  virtual void serializeParameters(nlohmann::ordered_json& out) const {}
  virtual void serialize(nlohmann::ordered_json& out, bool doIndent = true, bool doParameters = true, bool oldValues = false) const;
  [[nodiscard]] virtual std::string stringRep(const Database& db, bool colored = true) const;
  /**
   * Returns any plaintext values that are stored in a given command parameters, if the command does not contain strings.
   *
   * If a command has text spread across multiple sub-commands, those are considered one string and will be concatenated together.
   *
   * Commands with multiple string properties will have their strings ordered by appearance (e.g: {battlebackImage1, battlebackImage2})
   * @return If the command has any plain text, the plaintext value(s), otherwise an empty vector
   */
  [[nodiscard]] virtual std::vector<std::string> stringValues() const { return {}; }
  [[nodiscard]] virtual bool hasStringValues() const { return false; }
  [[nodiscard]] virtual std::vector<std::string> stringValueNames() const { return {}; }
  [[nodiscard]] static std::string symbol(EventCode code);
  [[nodiscard]] static std::string indentText(const std::optional<int>& indent);
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
  [[nodiscard]] virtual bool isTerminatingPartner(EventCode code, const std::optional<int>& codeIndent) const { return false; }
  virtual void setReverseSelection(bool reverseSelection) {}

  [[nodiscard]] std::optional<int> indent() const { return m_indent; }
  virtual void setIndent(int indent) {
    m_indent = indent;
    setModified();
  }

  [[nodiscard]] virtual std::shared_ptr<IEventCommand> clone() const = 0;

private:
  std::optional<int> m_indent{};
};