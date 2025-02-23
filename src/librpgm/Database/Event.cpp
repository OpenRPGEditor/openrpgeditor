#include "Database/Event.hpp"
#include "Database/IEventEditor.hpp"

Event::Event(const Event& other)
: IModifiable(other)
, m_id(other.m_id)
, m_name(other.m_name)
, m_note(other.m_note)
, m_pages(other.m_pages)
, m_x(other.m_x)
, m_y(other.m_y)
, m_oldid(other.m_oldid)
, m_oldname(other.m_oldname)
, m_oldnote(other.m_oldnote)
, m_oldpages(other.m_oldpages)
, m_oldx(other.m_oldx)
, m_oldy(other.m_oldy)
, m_editor(other.m_editor)
, m_renderer(other.m_renderer) {
  if (m_editor) {
    m_editor->setEventPtr(this);
  }
  if (m_renderer) {
    m_renderer->setEventPtr(this);
  }
}

Event& Event::operator=(const Event& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_name = other.m_name;
  m_note = other.m_note;
  m_pages = other.m_pages;
  m_x = other.m_x;
  m_y = other.m_y;
  m_oldid = other.m_oldid;
  m_oldname = other.m_oldname;
  m_oldnote = other.m_oldnote;
  m_oldpages = other.m_oldpages;
  m_oldx = other.m_oldx;
  m_oldy = other.m_oldy;
  m_editor = other.m_editor;
  m_renderer = other.m_renderer;
  if (m_editor) {
    m_editor->setEventPtr(this);
  }
  if (m_renderer) {
    m_renderer->setEventPtr(this);
  }
  return *this;
}

Event::Event(Event&& other) noexcept
: IModifiable(std::move(other))
, m_id(other.m_id)
, m_name(std::move(other.m_name))
, m_note(std::move(other.m_note))
, m_pages(std::move(other.m_pages))
, m_x(other.m_x)
, m_y(other.m_y)
, m_oldid(other.m_oldid)
, m_oldname(std::move(other.m_oldname))
, m_oldnote(std::move(other.m_oldnote))
, m_oldpages(std::move(other.m_oldpages))
, m_oldx(other.m_oldx)
, m_oldy(other.m_oldy)
, m_editor(std::move(other.m_editor))
, m_renderer(std::move(other.m_renderer)) {
  if (m_editor) {
    m_editor->setEventPtr(this);
  }
  if (m_renderer) {
    m_renderer->setEventPtr(this);
  }
}

Event& Event::operator=(Event&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_name = std::move(other.m_name);
  m_note = std::move(other.m_note);
  m_pages = std::move(other.m_pages);
  m_x = other.m_x;
  m_y = other.m_y;
  m_oldid = other.m_oldid;
  m_oldname = std::move(other.m_oldname);
  m_oldnote = std::move(other.m_oldnote);
  m_oldpages = std::move(other.m_oldpages);
  m_oldx = other.m_oldx;
  m_oldy = other.m_oldy;
  m_editor = std::move(other.m_editor);
  m_renderer = std::move(other.m_renderer);
  if (m_editor) {
    m_editor->setEventPtr(this);
  }
  if (m_renderer) {
    m_renderer->setEventPtr(this);
  }
  return *this;
}

int Event::id() const { return m_id; }
void Event::setId(const int id) {
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    idModified().fire(this, id);
  }
  setHasChanges();
}

const std::string& Event::name() const { return m_name; }
void Event::setName(const std::string& name) {
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    nameModified().fire(this, name);
  }
  setHasChanges();
}

const std::string& Event::note() const { return m_note; }
void Event::setNote(const std::string& note) {
  MODIFIABLE_SET_OLD_VALUE(note);
  m_note = note;
  if (!signalsDisabled()) {
    noteModified().fire(this, note);
  }
  setHasChanges();
}

std::vector<EventPage>& Event::pages() { return m_pages; }
const std::vector<EventPage>& Event::pages() const { return m_pages; }
void Event::setPages(const std::vector<EventPage>& pages) {
  MODIFIABLE_SET_OLD_VALUE(pages);
  m_pages = pages;
  if (!signalsDisabled()) {
    pagesModified().fire(this, pages);
  }
  setHasChanges();
}

void Event::addPage(const EventPage& page) {
  MODIFIABLE_SET_OLD_VALUE(pages);
  m_pages.emplace_back(page);
}

void Event::insertPage(const EventPage& page, int index) {
  MODIFIABLE_SET_OLD_VALUE(pages);
  m_pages.insert(m_pages.begin() + index, page);
  if (!signalsDisabled()) {
    pageInserted().fire(this, &m_pages[index], index);
  }
  setHasChanges();
}

void Event::removePage(const int index) {
  if (index < 0 || index >= m_pages.size()) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(pages);
  m_pages.erase(m_pages.begin() + index);
  if (!signalsDisabled()) {
    pageRemoved().fire(this);
  }
  setHasChanges();
}

int Event::x() const { return m_x; }
void Event::setX(const int x) {
  MODIFIABLE_SET_OLD_VALUE(x);
  m_x = x;
  if (!signalsDisabled()) {
    xModified().fire(this, m_x);
  }
  setHasChanges();
}

int Event::y() const { return m_y; }
void Event::setY(const int y) {
  MODIFIABLE_SET_OLD_VALUE(y);
  m_y = y;
  if (!signalsDisabled()) {
    yModified().fire(this, m_y);
  }
  setHasChanges();
}

void Event::swapPages(int a, int b) {
  if (a < 0 || a >= m_pages.size()) {
    return;
  }
  if (b < 0 || b >= m_pages.size()) {
    return;
  }

  MODIFIABLE_SET_OLD_VALUE(pages);
  std::swap(m_pages.at(a), m_pages.at(b));
  if (!signalsDisabled()) {
    pagesModified().fire(this, m_pages);
  }
  setHasChanges();
}
std::map<int, std::shared_ptr<const IModifiable>> Event::getConditionReferences(int targetId, SearchType type) const {
  std::map<int, std::shared_ptr<const IModifiable>> events; // Change shared_ptr to const IModifiable
  int pageIndex{0};
  if (type == SearchType::Variable) {
    for (auto& pages : m_pages) {
      if (pages.conditions().variableValid()) {
        if (pages.conditions().variableId() == targetId) {
          events.emplace(pageIndex, this);
        }
      }
      pageIndex++;
    }
  } else if (type == SearchType::Switch) {
    for (auto& pages : m_pages) {
      if (pages.conditions().switch1Valid() || pages.conditions().switch2Valid()) {
        if (pages.conditions().switch1Id() == targetId || pages.conditions().switch2Id() == targetId) {
          events.emplace(pageIndex, this);
        }
      }
      pageIndex++;
    }
  }
  return events;
}
std::map<int, std::shared_ptr<const IModifiable>> Event::getListReferences(int targetId, SearchType type) const {
  std::map<int, std::shared_ptr<const IModifiable>> cmds; // Change shared_ptr to const IModifiable
  for (auto& pages : m_pages) {
    for (const auto& command : pages.list()) {
      // Go through command list and find matches
      if (command->hasReference(targetId, type)) {
        cmds.emplace(0, command);
      }
    }
  }
  return cmds;
}

std::map<int, std::shared_ptr<const IModifiable>> Event::getListReferences(std::string text, SearchType type) const {
  std::map<int, std::shared_ptr<const IModifiable>> cmds; // Change shared_ptr to const IModifiable
  for (auto& pages : m_pages) {
    for (const auto& command : pages.list()) {
      // Go through command list and find matches
      if (command->hasStringReference(text, type)) {
        cmds.emplace(0, command);
      }
    }
  }
  return cmds;
}

void Event::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(pages);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(x);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(y);
}

void Event::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(pages);
  MODIFIABLE_ACCEPT_VALUE(x);
  MODIFIABLE_ACCEPT_VALUE(y);
}

nlohmann::ordered_json Event::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(id), MODIFIABLE_SERIALIZE_VALUE(name), MODIFIABLE_SERIALIZE_VALUE(note), MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(pages),
      MODIFIABLE_SERIALIZE_VALUE(x),  MODIFIABLE_SERIALIZE_VALUE(y),
  };
}

rpgmutils::signal<void(Event*, int)>& Event::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}
rpgmutils::signal<void(Event*, const std::string&)>& Event::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}
rpgmutils::signal<void(Event*, const std::string&)>& Event::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}
rpgmutils::signal<void(Event*, const std::vector<EventPage>&)>& Event::pagesModified() {
  if (!m_pagesModified) {
    m_pagesModified.emplace();
  }
  return *m_pagesModified;
}
rpgmutils::signal<void(Event*, const EventPage*, int)>& Event::pageAdded() {
  if (!m_pageAdded) {
    m_pageAdded.emplace();
  }
  return *m_pageAdded;
}
rpgmutils::signal<void(Event*, const EventPage*, int)>& Event::pageInserted() {
  if (!m_pageInserted) {
    m_pageInserted.emplace();
  }
  return *m_pageInserted;
}
rpgmutils::signal<void(Event*)>& Event::pageRemoved() {
  if (!m_pageRemoved) {
    m_pageRemoved.emplace();
  }
  return *m_pageRemoved;
}
rpgmutils::signal<void(Event*, int)>& Event::xModified() {
  if (!m_xModified) {
    m_xModified.emplace();
  }
  return *m_xModified;
}
rpgmutils::signal<void(Event*, int)>& Event::yModified() {
  if (!m_yModified) {
    m_yModified.emplace();
  }
  return *m_yModified;
}

void to_json(nlohmann::ordered_json& to, const Event& event) {
  to["id"] = event.m_id;
  to["name"] = event.m_name;
  to["note"] = event.m_note;
  for (const auto& page : event.m_pages) {
    to["pages"].push_back(page.clone());
  }
  to["x"] = event.m_x;
  to["y"] = event.m_y;
}

void from_json(const nlohmann::ordered_json& from, Event& event) {
  event.m_id = from.value("id", event.m_id);
  event.m_name = from.value("name", event.m_name);
  event.m_note = from.value("note", event.m_note);
  event.m_pages = from.value("pages", event.m_pages);
  event.m_x = from.value("x", event.m_x);
  event.m_y = from.value("y", event.m_y);
}