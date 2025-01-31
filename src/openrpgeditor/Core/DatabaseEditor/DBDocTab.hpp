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

  bool m_isEdittingText{false}; // Used to edit text by double clicking

  void Save() const;
  bool isHeader() const { return inHeader; }
  bool isBulletFormatting(std::string text) { return text.contains("-"); }
  bool isFormatting() const { return (isHeader() == false); }
};
