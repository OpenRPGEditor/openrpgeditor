#pragma once
#include "CommonUI/IDialogController.hpp"

#include <nlohmann/json.hpp>

class JSONInspector final {
public:
  bool openFile(const std::filesystem::path& toInspect);
  bool saveFile(const std::filesystem::path& toSave = {}) const;
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
  bool m_confirmed{false};
  bool m_open{false};
  std::filesystem::path m_path{};
  nlohmann::ordered_json m_inspectedJson{};
  bool m_askForSave{false};
};
