#pragma once

#include "Database/EventPage.hpp"
#include "Database/Globals.hpp"
#include "Database/IEventEditor.hpp"
#include "SearchResult.hpp"
#include <string>
#include <vector>

class Event;
class IEventRenderer {
public:
  IEventRenderer() = delete;
  virtual ~IEventRenderer() = default;

  virtual void draw(float mapScale, bool isHovered, bool selected, bool halfAlpha) = 0;
  virtual void setPage(int page) = 0;
  virtual EventPage* page() const = 0;
  virtual int pageIndex() const = 0;
  virtual double x() const = 0;
  virtual double y() const = 0;
  virtual int screenZ() const = 0;
  virtual uint32_t spriteId() const = 0;
  virtual bool isThrough() const = 0;

  /* Client code must implement this to create their own event renderer */
  static IEventRenderer* create(Event* ev);

protected:
  friend class Event;
  explicit IEventRenderer(Event* event) : m_event(event) {}
  virtual void eventPointerInvalidated() = 0;
  void setEventPtr(Event* event) {
    m_event = event;
    eventPointerInvalidated();
  }
  Event* m_event{nullptr};
};

class Event final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& to, const Event& event);
  friend void from_json(const nlohmann::ordered_json& from, Event& event);

public:
  Event() = default;
  Event(const Event& other);
  Event& operator=(const Event& other);
  Event(Event&& other) noexcept;
  Event& operator=(Event&& other) noexcept;
  int id() const;
  void setId(int id);

  const std::string& name() const;
  void setName(const std::string& name);

  const std::string& note() const;
  void setNote(const std::string& note);

  int pageCount() const { return m_pages.size(); }

  std::vector<EventPage>& pages();
  const std::vector<EventPage>& pages() const;
  void setPages(const std::vector<EventPage>& pages);
  EventPage* page(const int page) {
    if (page < 0 || page >= pages().size()) {
      return nullptr;
    }
    return &m_pages[page];
  }

  const EventPage* page(const int page) const {
    if (page < 0 || page >= pages().size()) {
      return nullptr;
    }
    return &m_pages[page];
  }

  void addPage(const EventPage& page);
  void insertPage(const EventPage& page, int index);
  void removePage(int index);

  int x() const;
  void setX(int x);

  int y() const;
  void setY(int y);

  IEventRenderer* renderer() const {
    if (!m_renderer) {
      m_renderer.reset(IEventRenderer::create(const_cast<Event*>(this)));
    }
    return m_renderer.get();
  }
  IEventEditor* editor() const {
    if (!m_editor) {
      m_editor.reset(IEventEditor::create(const_cast<Event*>(this)));
    }
    return m_editor.get();
  }

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_pages, [](const auto& page) { return page.isModified(); });
  }

  rpgmutils::signal<void(Event*, int)>& idModified();
  rpgmutils::signal<void(Event*, const std::string&)>& nameModified();
  rpgmutils::signal<void(Event*, const std::string&)>& noteModified();
  rpgmutils::signal<void(Event*, const std::vector<EventPage>&)>& pagesModified();
  rpgmutils::signal<void(Event*, const EventPage*, int)>& pageAdded();
  rpgmutils::signal<void(Event*, const EventPage*, int)>& pageInserted();
  rpgmutils::signal<void(Event*)>& pageRemoved();
  rpgmutils::signal<void(Event*, int)>& xModified();
  rpgmutils::signal<void(Event*, int)>& yModified();

  void swapPages(int a, int b);

  Event clone() const { return Event(*this, 1); }

  bool operator==(const Event& other) const { return m_id == other.m_id && m_name == other.m_name && m_note == other.m_note && m_pages == other.m_pages && m_x == other.m_x && m_y == other.m_y; }
  std::vector<std::shared_ptr<const IModifiable>> getConditionReferences(int targetId, SearchType type) const override;
  std::vector<std::shared_ptr<const IModifiable>> getListReferences(int targetId, SearchType type) const override;
  std::vector<std::shared_ptr<const IModifiable>> getListReferences(std::string text, SearchType type) const override;

private:
  Event(const Event& other, int) : IModifiable(other), m_id(other.m_id), m_name(other.m_name), m_note(other.m_note), m_x(other.m_x), m_y(other.m_y) {
    /* Clone our pages */
    for (const auto& page : other.m_pages) {
      m_pages.push_back(page.clone());
    }
  }
  int m_id{};
  std::string m_name{};
  std::string m_note{};
  std::vector<EventPage> m_pages{};
  int m_x{};
  int m_y{};

  std::optional<int> m_oldid;
  std::optional<std::string> m_oldname;
  std::optional<std::string> m_oldnote;
  std::optional<std::vector<EventPage>> m_oldpages;
  std::optional<int> m_oldx;
  std::optional<int> m_oldy;

  std::optional<rpgmutils::signal<void(Event*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(Event*, const std::string&)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Event*, const std::string&)>> m_noteModified;
  std::optional<rpgmutils::signal<void(Event*, const std::vector<EventPage>&)>> m_pagesModified;
  std::optional<rpgmutils::signal<void(Event*, const EventPage*, int)>> m_pageAdded;
  std::optional<rpgmutils::signal<void(Event*, const EventPage*, int)>> m_pageInserted;
  std::optional<rpgmutils::signal<void(Event*)>> m_pageRemoved;
  std::optional<rpgmutils::signal<void(Event*, int)>> m_xModified;
  std::optional<rpgmutils::signal<void(Event*, int)>> m_yModified;
  mutable std::shared_ptr<IEventEditor> m_editor;
  mutable std::shared_ptr<IEventRenderer> m_renderer;
};

void to_json(nlohmann::ordered_json& to, const Event& event);
void from_json(const nlohmann::ordered_json& from, Event& event);