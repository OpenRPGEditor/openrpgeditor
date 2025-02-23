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

  std::vector<std::vector<std::string>> headerTexts;          // All categorized sections
  std::vector<std::vector<std::string>> referenceHeaderTexts; // Referenced for replacement
  std::vector<std::string> m_names;
  std::vector<std::string> m_texts;
  std::string headerText;
  std::string referenceText;

  bool m_isEdittingText{false};
  bool m_isEdittingNames{false};
  // bool m_isEdittingTexts{false};
  bool m_isEdittingHeader{false};

  bool isArrowBullet{false};
  bool isIdenting{false};
  bool isFontFormat{false};
  bool isBulletFormat{false};
  bool isPictureFormat{false};

  float m_formatScale{0.0};

  void Save() const;
  bool isHeader() const { return inHeader; }
  bool isBulletFormatting(std::string text) { return text.starts_with("-"); }
  bool isSeperatorFormatting(std::string text) { return text.starts_with("~"); }
  bool isArrowBulletFormatting(std::string text) { return text.starts_with(">"); }
  bool isIndentFormatting(std::string text) { return text.starts_with("\\t"); }
  bool isFormatting() const { return (isHeader() == false); }
  bool isFontFormatting(std::string text) const { return text.starts_with("\\f"); }
  bool isPictureFormatting(std::string text) const { return text.starts_with("\\p"); }
  void createHeaders();
  void setDoc(int index);
  std::string GetFormattingHelpText();
  bool ProcessFormatting(std::string& text);
  bool isNumericOnly(const std::string& text);
};
