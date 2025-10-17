#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Database.hpp"
#include "Database/Docs.hpp"

struct DBDocTab : IDBEditorTab {
  DBDocTab() = delete;
  explicit DBDocTab(DatabaseEditor* parent);
  void draw() override;
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  std::string getName(int index) override { return ""; }
  int getCount() override { return 0; }

  [[nodiscard]] std::string tabName() const override { return tr("Documentation"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBDocTab"sv; };
  bool isExperimental() const override { return true; }
  
  bool isReady() const override { return !!Database::instance()->docs; }
  void initialize() override {
    if (!isReady()) {
      return;
    }
    m_docs = &Database::instance()->docs.value();
    if (m_docs->docs.size() == 0) {
      Doc doc;

      doc.setNames({"Example Name 1", "Example Name 2", "Example Name 3"});
      doc.setTexts({" Example Text 1", "Example Text 2", "Example Test 3"});

      m_docs->docs.emplace_back(doc);

      Save();
    }
    m_names = m_docs->docs.front().names();
    m_texts = m_docs->docs.front().texts();

    createHeaders();
  }

  bool isInitialized() const override { return m_docs; }

private:
  int m_categoryStart;
  int m_categoryEnd;
  std::vector<int> m_headers;
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
