#pragma once
#include "Core/CommonUI/IDialogController.hpp"
#include "Core/Graphics/FaceSheet.hpp"
#include "Directory.hpp"

#include <optional>

class FacePicker final : public IDialogController {
public:
  explicit FacePicker(std::string_view sheetName = {}, int faceIndex = 0);

  std::tuple<bool, bool> draw() override;

  [[nodiscard]] std::string selectedSheet() const {
    return m_selectedSheet >= 0 ? m_faceDirectory.isParentDirectory() ? m_faceSheets[m_selectedSheet] : m_faceDirectory.getPathPrefix() + '/' + m_faceSheets[m_selectedSheet] : "";
  }

  [[nodiscard]] int setFaceIndex() const { return m_faceIndex; }

  void setFaceInfo(const std::string_view faceName, const int faceIndex) {
    m_faceName = faceName;
    m_faceIndex = faceIndex;
    setSelectedSheet();
  }

private:
  void setSelectedSheet();
  std::optional<FaceSheet> m_faceSheet;
  std::string m_faceName;
  std::vector<std::string> m_faceSheets;
  std::vector<std::string> m_folderDir;
  Directory m_faceDirectory;
  int m_selectedFolder{-1};
  int m_selectedSheet{-1};
  int m_faceIndex{0};
};
