
#include "Database/Tileset.hpp"
#include "Database/Globals.hpp"

Tileset::Tileset(const Tileset& other)
: IModifiable(other)
, m_id(other.m_id)
, m_flags(other.m_flags)
, m_mode(other.m_mode)
, m_name(other.m_name)
, m_note(other.m_note)
, m_tilesetNames(other.m_tilesetNames)
, m_oldid(other.m_oldid)
, m_oldflags(other.m_oldflags)
, m_oldmode(other.m_oldmode)
, m_oldname(other.m_oldname)
, m_oldnote(other.m_oldnote)
, m_oldtilesetNames(other.m_oldtilesetNames)
, m_isValid(other.m_isValid) {}

Tileset& Tileset::operator=(const Tileset& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_flags = other.m_flags;
  m_mode = other.m_mode;
  m_name = other.m_name;
  m_note = other.m_note;
  m_tilesetNames = other.m_tilesetNames;
  m_oldid = other.m_oldid;
  m_oldflags = other.m_oldflags;
  m_oldmode = other.m_oldmode;
  m_oldname = other.m_oldname;
  m_oldnote = other.m_oldnote;
  m_oldtilesetNames = other.m_oldtilesetNames;
  m_isValid = other.m_isValid;
  return *this;
}

Tileset::Tileset(Tileset&& other) noexcept
: IModifiable(other)
, m_id(other.m_id)
, m_flags(std::move(other.m_flags))
, m_mode(other.m_mode)
, m_name(std::move(other.m_name))
, m_note(std::move(other.m_note))
, m_tilesetNames(std::move(other.m_tilesetNames))
, m_oldid(other.m_oldid)
, m_oldflags(std::move(other.m_oldflags))
, m_oldmode(other.m_oldmode)
, m_oldname(std::move(other.m_oldname))
, m_oldnote(std::move(other.m_oldnote))
, m_oldtilesetNames(std::move(other.m_oldtilesetNames))
, m_isValid(other.m_isValid) {}

Tileset& Tileset::operator=(Tileset&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_flags = std::move(other.m_flags);
  m_mode = other.m_mode;
  m_name = std::move(other.m_name);
  m_note = std::move(other.m_note);
  m_tilesetNames = std::move(other.m_tilesetNames);
  m_oldid = other.m_oldid;
  m_oldflags = std::move(other.m_oldflags);
  m_oldmode = other.m_oldmode;
  m_oldname = std::move(other.m_oldname);
  m_oldnote = std::move(other.m_oldnote);
  m_oldtilesetNames = std::move(other.m_oldtilesetNames);
  m_isValid = other.m_isValid;
  return *this;
}

[[nodiscard]] int Tileset::id() const { return m_id; }
void Tileset::setId(const int id) {
  if (m_id == id) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    idModified().fire(this, id);
  }
  setModified();
}

[[nodiscard]] const std::vector<std::optional<int>>& Tileset::flags() const { return m_flags; }

int Tileset::flag(const int id) {
  if (id > TileHelper::TILE_ID_MAX) {
    return 0;
  }
  if (id >= m_flags.size() && m_flags.size() <= TileHelper::TILE_ID_MAX) {
    int i = id - (m_flags.size() - 1);
    while (i--) {
      m_flags.emplace_back();
    }
    m_flags.back() = 0;
  }

  if (id >= m_flags.size()) {
    return 0;
  }
  if (!m_flags[id]) {
    m_flags[id] = 0;
  }
  return *m_flags[id];
}

[[nodiscard]] int Tileset::flag(const int idx) const {
  if (idx > TileHelper::TILE_ID_MAX) {
    return 0;
  }

  if (idx >= 0 && idx < m_flags.size()) {
    return m_flags[idx] ? *m_flags[idx] : 0;
  }
  return 0;
}

void Tileset::setFlags(const std::vector<std::optional<int>>& flags) {
  if (m_flags == flags) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(flags);
  m_flags = flags;
  if (!signalsDisabled()) {
    flagsModified().fire(this, flags);
  }
  setModified();
}
void Tileset::setFlag(const int idx, const int flag, const bool enabled) {
  MODIFIABLE_SET_OLD_VALUE(flags);
  if (idx >= m_flags.size() && m_flags.size() <= TileHelper::TILE_ID_MAX) {
    int i = idx - (m_flags.size() - 1);
    while (i--) {
      m_flags.emplace_back();
    }
  } else if (idx > TileHelper::TILE_ID_MAX) {
    return;
  }
  if (!m_flags[idx]) {
    m_flags[idx] = 0;
  }

  if (enabled) {
    *m_flags[idx] |= flag;
  } else {
    *m_flags[idx] &= ~flag;
  }
  if (!signalsDisabled()) {
    flagsModified().fire(this, m_flags);
  }
  setModified();
}

void Tileset::setTerrainTag(const int idx, const int tag) {
  MODIFIABLE_SET_OLD_VALUE(flags);
  if (idx >= m_flags.size() && m_flags.size() <= TileHelper::TILE_ID_MAX) {
    int i = idx - (m_flags.size() - 1);
    while (i--) {
      m_flags.emplace_back();
    }
  } else if (idx > TileHelper::TILE_ID_MAX) {
    return;
  }
  if (!m_flags[idx]) {
    m_flags[idx] = 0;
  }

  *m_flags[idx] = ((tag & 0xFFFFF) << 12) | (*m_flags[idx] & 0xFFF);
  if (!signalsDisabled()) {
    flagsModified().fire(this, m_flags);
  }
  setModified();
}

[[nodiscard]] Tileset::Mode Tileset::mode() const { return m_mode; }
void Tileset::setMode(const Mode mode) {
  if (m_mode == mode) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(mode);
  m_mode = mode;
  if (!signalsDisabled()) {
    modeModified().fire(this, mode);
  }
  setModified();
}

[[nodiscard]] const std::string& Tileset::name() const { return m_name; }
void Tileset::setName(const std::string& name) {
  if (m_name == name) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(name);
  m_name = name;
  if (!signalsDisabled()) {
    nameModified().fire(this, name);
  }
  setModified();
}

[[nodiscard]] const std::string& Tileset::note() const { return m_note; }
void Tileset::setNote(const std::string& note) {
  if (m_note == note) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(note);
  m_note = note;
  if (!signalsDisabled()) {
    noteModified().fire(this, note);
  }
  setModified();
}

[[nodiscard]] const std::array<std::string, 9>& Tileset::tilesetNames() const { return m_tilesetNames; }
void Tileset::setTilesetNames(const std::array<std::string, 9>& tilesetNames) {
  if (m_tilesetNames == tilesetNames) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(tilesetNames);
  m_tilesetNames = tilesetNames;
  if (!signalsDisabled()) {
    tilesetNamesModified().fire(this, tilesetNames);
  }
  setModified();
}
[[nodiscard]] const std::string& Tileset::tilesetName(const int idx) const {
  assert(idx >= 0 && idx < m_tilesetNames.size());
  return m_tilesetNames[idx];
}
void Tileset::setTilesetName(const int idx, const std::string& name) {
  assert(idx >= 0 && idx < m_tilesetNames.size());
  if (m_tilesetNames[idx] == name) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(tilesetNames);
  m_tilesetNames[idx] = name;
  if (!signalsDisabled()) {
    tilesetNamesModified().fire(this, m_tilesetNames);
  }
  setModified();
}

void Tileset::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(flags);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(mode);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(note);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(tilesetNames);
}
void Tileset::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(flags);
  MODIFIABLE_ACCEPT_VALUE(mode);
  MODIFIABLE_ACCEPT_VALUE(name);
  MODIFIABLE_ACCEPT_VALUE(note);
  MODIFIABLE_ACCEPT_VALUE(tilesetNames);
}
nlohmann::ordered_json Tileset::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(id),   MODIFIABLE_SERIALIZE_VALUE(flags), MODIFIABLE_SERIALIZE_VALUE(mode),
      MODIFIABLE_SERIALIZE_VALUE(name), MODIFIABLE_SERIALIZE_VALUE(note),  MODIFIABLE_SERIALIZE_VALUE(tilesetNames),
  };
}

rpgmutils::signal<void(Tileset*, int)>& Tileset::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}
rpgmutils::signal<void(Tileset*, const std::vector<std::optional<int>>&)>& Tileset::flagsModified() {
  if (!m_flagsModified) {
    m_flagsModified.emplace();
  }
  return *m_flagsModified;
}
rpgmutils::signal<void(Tileset*, Tileset::Mode)>& Tileset::modeModified() {
  if (!m_modeModified) {
    m_modeModified.emplace();
  }
  return *m_modeModified;
}
rpgmutils::signal<void(Tileset*, const std::string&)>& Tileset::nameModified() {
  if (!m_nameModified) {
    m_nameModified.emplace();
  }
  return *m_nameModified;
}
rpgmutils::signal<void(Tileset*, const std::string&)>& Tileset::noteModified() {
  if (!m_noteModified) {
    m_noteModified.emplace();
  }
  return *m_noteModified;
}
rpgmutils::signal<void(Tileset*, const std::array<std::string, 9>&)>& Tileset::tilesetNamesModified() {
  if (!m_tilesetNamesModified) {
    m_tilesetNamesModified.emplace();
  }
  return *m_tilesetNamesModified;
}

void to_json(nlohmann::ordered_json& j, const Tileset& tileset) {
  j = {
      {"id", tileset.m_id}, {"flags", tileset.m_flags}, {"mode", tileset.m_mode}, {"name", tileset.m_name}, {"note", tileset.m_note}, {"tilesetNames", tileset.m_tilesetNames},
  };
}

void from_json(const nlohmann::ordered_json& j, Tileset& tileset) {
  tileset.m_id = j.value("id", tileset.m_id);
  tileset.m_flags = j.value("flags", tileset.m_flags);
  tileset.m_mode = j.value("mode", tileset.m_mode);
  tileset.m_name = j.value("name", tileset.m_name);
  tileset.m_note = j.value("note", tileset.m_note);
  tileset.m_tilesetNames = j.value("tilesetNames", tileset.m_tilesetNames);
}
