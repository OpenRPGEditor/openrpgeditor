#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ShowChoiceCommand final : IEventCommand {
  ShowChoiceCommand() = default;
  explicit ShowChoiceCommand(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters);
  ~ShowChoiceCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Choices; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ShowChoiceCommand>(*this); }

  [[nodiscard]] constexpr bool collapsable() const override { return true; }
  [[nodiscard]] bool isCollapsed() const override { return m_collapsed; }
  void setCollapsed(const bool collapsed) override { m_collapsed = collapsed; }
  bool isPartner(const EventCode code, const std::optional<int>& codeIndent) override {
    if (!codeIndent) {
      return false;
    }

    return (code == EventCode::End_del_ShowChoices) && *codeIndent == indent();
  }

  [[nodiscard]] constexpr int partnerCount() const override { return 1; }

  [[nodiscard]] constexpr bool hasPartner() const override { return true; }

  bool m_collapsed{false};

  TextBackground background = TextBackground::Window;
  ChoiceWindowPosition positionType = ChoiceWindowPosition::Right;
  std::vector<std::string> choices{"Yes", "No"};
  int cancelType{1};  // < 0 == disallow/branch
  int defaultType{0}; // -1 is none

  bool hasStringReference(const std::string& str, SearchType type) override {
    if (type == SearchType::Text) {
      for (auto& cmd : choices) {
        if (cmd.contains(str)) {
          return true;
        }
      }
    }
    return false;
  }
  bool setStringReference(const std::string& replaceStr, const std::string& str, SearchType type) override {
    if (type == SearchType::Text) {
      for (auto& cmd : choices) {
        if (cmd.contains(str)) {
          size_t pos = str.find(replaceStr);
          if (pos != std::string::npos) {
            cmd.replace(pos, replaceStr.size(), str);
          }
        }
      }
    }
    return true;
  }
};

struct WhenSelectedCommand final : IEventCommand {
  WhenSelectedCommand() = default;
  explicit WhenSelectedCommand(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters);
  ~WhenSelectedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Selected; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<WhenSelectedCommand>(*this); }

  [[nodiscard]] constexpr bool collapsable() const override { return true; }
  [[nodiscard]] bool isCollapsed() const override { return m_collapsed; }
  [[nodiscard]] bool isCollapseEnd() const override { return false; }
  void setCollapsed(const bool collapsed) override { m_collapsed = collapsed; }
  bool isPartner(const EventCode code, const std::optional<int>& codeIndent) override {
    if (!codeIndent) {
      return false;
    }

    return (code == EventCode::When_Selected || code == EventCode::When_Cancel || code == EventCode::End_del_ShowChoices) && *codeIndent == indent();
  }
  [[nodiscard]] constexpr bool hasPartner() const override { return true; }
  [[nodiscard]] constexpr int partnerCount() const override { return 3; }
  [[nodiscard]] bool reverseSelection() const override { return m_reverseSelection; }
  void setReverseSelection(const bool reverseSelection) override { m_reverseSelection = reverseSelection; }
  bool m_reverseSelection = false;
  bool m_collapsed{false};

  int param1;
  std::string choice;

  bool hasStringReference(const std::string& str, SearchType type) override {
    if (type == SearchType::Text) {
      return true;
    }
    return false;
  }
  bool setStringReference(const std::string& replaceStr, const std::string& str, SearchType type) override {
    if (type == SearchType::Text) {
      choice = str;
    }
    return true;
  }
};

struct WhenCancelCommand final : IEventCommand {
  WhenCancelCommand() = default;
  explicit WhenCancelCommand(const std::optional<int>& _indent, const nlohmann::json& parameters) : IEventCommand(_indent, parameters) {}
  ~WhenCancelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Cancel; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<WhenCancelCommand>(*this); }

  [[nodiscard]] constexpr bool collapsable() const override { return true; }
  [[nodiscard]] bool isCollapsed() const override { return m_collapsed; }
  [[nodiscard]] bool isCollapseEnd() const override { return false; }
  void setCollapsed(const bool collapsed) override { m_collapsed = collapsed; }
  bool isPartner(const EventCode code, const std::optional<int>& codeIndent) override {
    if (!codeIndent) {
      return false;
    }

    return (code == EventCode::End_del_ShowChoices) && *codeIndent == indent();
  }
  [[nodiscard]] constexpr bool hasPartner() const override { return true; }
  [[nodiscard]] constexpr int partnerCount() const override { return 1; }
  [[nodiscard]] bool reverseSelection() const override { return m_reverseSelection; }
  void setReverseSelection(const bool reverseSelection) override { m_reverseSelection = reverseSelection; }
  bool m_reverseSelection = false;
  bool m_collapsed{false};
};

struct ShowChoicesEndCommand final : IEventCommand {
  ShowChoicesEndCommand() = default;
  explicit ShowChoicesEndCommand(const std::optional<int>& _indent, const nlohmann::json& parameters) : IEventCommand(_indent, parameters) {}
  ~ShowChoicesEndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End_del_ShowChoices; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ShowChoicesEndCommand>(*this); }

  bool hasStringReference(const std::string& str, SearchType type) override {
    if (type == SearchType::Text) {
      return true;
    }
    return false;
  }
};