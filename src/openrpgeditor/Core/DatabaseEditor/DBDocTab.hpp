#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Docs.hpp"

struct DBDocTab : IDBEditorTab {
  explicit DBDocTab(Docs& docs, DatabaseEditor* parent) : IDBEditorTab(parent), m_docs(&docs) {}
  void draw() override;

private:
  Docs* m_docs = nullptr;
  int m_id{0};

  bool m_isEdittingText{false}; // Used to edit text by double clicking

  void SaveToFile() const;
  void SaveChanges();
};
