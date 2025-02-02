#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Docs.hpp"

struct DBDocTab : IDBEditorTab {
  DBDocTab() = delete;
  explicit DBDocTab(Docs& docs, DatabaseEditor* parent);
  void draw() override;

private:
  Docs* m_docs = nullptr;
  int m_id{0};
  bool inHeader{false};
  int m_selectedHeader{0};
  int m_selectedCategory{0};

  std::vector<std::vector<std::string>> headerTexts; // All categorized sections
  std::vector<std::string> m_names;
  std::vector<std::string> m_texts;

  bool m_isEdittingText{false}; // Used to edit text by double clicking
  bool m_isEdittingNames{false};
  bool m_isEdittingTexts{false};

  void Save() const;
  bool isHeader() const { return inHeader; }
  bool isBulletFormatting(std::string text) { return text.starts_with("- "); }
  bool isSeperatorFormatting(std::string text) { return text.starts_with("~"); }
  bool isArrowBulletFormatting(std::string text) { return text.starts_with(">"); }
  bool isIndentFormatting(std::string text) { return text.starts_with("\\t"); }
  bool isFormatting() const { return (isHeader() == false); }
  void createHeaders();
  void setDoc(int index);
  std::string GetFormattingHelpText();
};
