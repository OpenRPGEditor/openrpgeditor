#pragma once
#include "CommonUI/IDialogController.hpp"
#include "Undo/UndoStack.hpp"

#include <nlohmann/json.hpp>
#include <utility>

class JSONUndoCommand final : public IUndoCommand {
public:
  explicit JSONUndoCommand(nlohmann::ordered_json& container, const std::string_view path, const nlohmann::ordered_json& value)
  : m_object(container)
  , m_path(path)
  , m_value(value) {}

  [[nodiscard]] constexpr std::string_view type() const override { return "JSON Undo Command"; }

  bool undo() override { return false; }

private:
  nlohmann::ordered_json& m_object;
  std::string m_path;
  nlohmann::ordered_json m_value;
};

class JSONInspector final {
public:
  bool openFile(const std::filesystem::path& toInspect);
  [[nodiscard]] bool saveFile(const std::filesystem::path& toSave = {}, int indent=4) const;
  void closeFile(bool promptSave);

  [[nodiscard]] bool isOpen() const { return m_open; }
  [[nodiscard]] bool isConfirmed() const { return m_confirmed; }
  void setOpen(const bool open = true) {
    m_open = open;
    m_confirmed = false;
  }

  void accept() {
    m_open = false;
    m_confirmed = true;
  }
  std::tuple<bool, bool> draw();

private:
  void DrawJsonPrimitive(const std::string& name, nlohmann::ordered_json& element);
  void DrawJsonContainer(const std::string& name, nlohmann::ordered_json& container);
  void DrawJsonElement(const std::string& name, nlohmann::ordered_json& element);
  bool m_confirmed{false};
  bool m_open{false};
  std::filesystem::path m_path{};
  nlohmann::ordered_json m_inspectedJson{};
  bool m_askForSave{false};
  std::string m_newKey;
  bool m_newKeyInvalid{true};
  nlohmann::ordered_json::value_t m_newType{};
  nlohmann::ordered_json* m_targetContainer{nullptr};

  UndoStack m_undoStack;
};
