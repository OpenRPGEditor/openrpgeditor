﻿#pragma once
#include "Globals.hpp"
#include "Utils/SignalSlot.hpp"

#define STRINGIZE(x) #x
#define MODIFIABLE_SET_OLD_VALUE(name)                                                                                                                                                                 \
  do {                                                                                                                                                                                                 \
    if (!m_old##name && !signalsDisabled()) {                                                                                                                                                          \
      m_old##name = m_##name;                                                                                                                                                                          \
    }                                                                                                                                                                                                  \
    m_hasChanges = true;                                                                                                                                                                               \
  } while (0)

#define MODIFIABLE_RESTORE_ORIGINAL_VALUE(name)                                                                                                                                                        \
  do {                                                                                                                                                                                                 \
    if (m_old##name) {                                                                                                                                                                                 \
      m_##name = *m_old##name;                                                                                                                                                                         \
      m_old##name.reset();                                                                                                                                                                             \
    }                                                                                                                                                                                                  \
  } while (0)

#define MODIFIABLE_ACCEPT_VALUE(name) m_old##name.reset()
#define MODIFIABLE_SERIALIZE_VALUE(name) {STRINGIZE(name), m_old##name ? *m_old##name : m_##name}
#define MODIFIABLE_SERIALIZE_VALUE_FLAT_ARRAY(json, name)                                                                                                                                              \
  do {                                                                                                                                                                                                 \
    json.push_back(m_old##name ? *m_old##name : m_##name);                                                                                                                                             \
  } while (0)
#define MODIFIABLE_SERIALIZE_VALUE_NONTRIVIAL_ARRAY(name)                                                                                                                                              \
  [&]() {                                                                                                                                                                                              \
    nlohmann::ordered_json name;                                                                                                                                                                       \
    const auto& vec = m_old##name ? *m_old##name : m_##name;                                                                                                                                           \
    for (const auto& v : vec) {                                                                                                                                                                        \
      name.push_back(v.serializeOldValues());                                                                                                                                                          \
    }                                                                                                                                                                                                  \
    return name;                                                                                                                                                                                       \
  }()

#define MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(name)                                                                                                                                                   \
  {STRINGIZE(name), [&]() {                                                                                                                                                                            \
     nlohmann::json name;                                                                                                                                                                              \
     const auto& vec = m_old##name ? *m_old##name : m_##name;                                                                                                                                          \
     for (const auto& v : vec) {                                                                                                                                                                       \
       name.push_back(v.serializeOldValues());                                                                                                                                                         \
     }                                                                                                                                                                                                 \
     return name;                                                                                                                                                                                      \
   }()}

#define MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY_OPTIONAL(name)                                                                                                                                          \
  {STRINGIZE(name), [&]() {                                                                                                                                                                            \
     nlohmann::json name;                                                                                                                                                                              \
     const auto& vec = m_old##name ? *m_old##name : m_##name;                                                                                                                                          \
     for (const auto& v : vec) {                                                                                                                                                                       \
       if (v) {                                                                                                                                                                                        \
         name.push_back(v->serializeOldValues());                                                                                                                                                      \
       } else {                                                                                                                                                                                        \
         name.push_back(nullptr);                                                                                                                                                                      \
       }                                                                                                                                                                                               \
     }                                                                                                                                                                                                 \
     return name;                                                                                                                                                                                      \
   }()}

#define MODIFIABLE_SERIALIZE_CHILD_VALUE(name) {STRINGIZE(name), (m_old##name ? *m_old##name : m_##name).serializeOldValues()}

class IModifiable {
public:
  IModifiable();
  IModifiable(const IModifiable& other);
  IModifiable& operator=(const IModifiable& other);
  IModifiable(IModifiable&& other) noexcept;
  IModifiable& operator=(IModifiable&& other) noexcept;
  virtual ~IModifiable();
  [[nodiscard]] virtual bool isModified() const { return m_modified; }
  void setModified(const bool _modified = true) {
    m_modified = _modified;
    if (m_modified && !signalsDisabled()) {
      emit_signal(modified(), this);
    }
  }

  virtual void restoreOriginal() { m_hasChanges = false; }
  virtual void acceptChanges() {
    m_modified = false;
    setHasChanges(true);
  }
  virtual nlohmann::ordered_json serializeOldValues() const { return {}; }
  bool hasChanges() const { return m_hasChanges | m_modified; };
  void setHasChanges(const bool hasChanges = true) { m_hasChanges = hasChanges; }

  bool signalsDisabled() const { return m_signalsDisabled; }
  void disableSignals() { m_signalsDisabled = true; }
  void enableSignals() { m_signalsDisabled = false; }

  virtual std::map<int, bool> isConditionalReference(int targetId, SearchType type) const { return {}; }

  virtual bool hasReference(int targetId, SearchType type) { return false; }
  virtual bool setReference(int targetId, int newId, SearchType type) { return false; }
  virtual bool hasStringReference(const std::string& text, SearchType type) { return false; }
  virtual bool setStringReference(const std::string& replaceStr, const std::string& text, SearchType type) { return false; }

  rpgmutils::signal<void(IModifiable*)>& modified();

protected:
  bool m_hasChanges{false};

private:
  std::optional<rpgmutils::signal<void(IModifiable*)>> m_modifiedSignal;
  bool m_modified{false};
  bool m_signalsDisabled{false};
};