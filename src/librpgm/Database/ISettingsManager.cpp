#include "Database/ISettingsManager.hpp"
#include "Database/Log.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

// TODO: Clean this up and add proper logging.
bool ISettingsManager::load(const std::filesystem::path& filename) {
  std::ifstream f(filename);
  if (!f.is_open()) {
    return false;
  }
  try {
    f >> m_settings;
    m_dirty = false;
    return true;
  } catch (const nlohmann::ordered_json::exception& e) {
    m_settings = nlohmann::ordered_json::object();
    m_dirty = false;
    RPGM_WARN("Unable to load settings \"{}\"", e.what());
    return false;
  }
}

bool ISettingsManager::save(const std::filesystem::path& filename) {
  if (!m_dirty) {
    return true; // no change
  }
  std::ofstream f(filename);
  if (!f.is_open()) {
    return false;
  }
  f << m_settings.dump(4);
  m_dirty = false;
  return true;
}

bool ISettingsManager::saveForce(const std::filesystem::path& filename) const {
  std::ofstream f(filename);
  if (!f.is_open())
    return false;
  f << m_settings.dump(4);
  return true;
}

void ISettingsManager::merge(const nlohmann::ordered_json& overrideJson) {
  if (!overrideJson.is_object())
    return;
  mergeRecursive(m_settings, overrideJson);
  m_dirty = true;
}

bool ISettingsManager::loadMerged(const std::filesystem::path& defaultFile, const std::filesystem::path& userFile) {
  nlohmann::ordered_json defaults;
  nlohmann::ordered_json user;
  if (!readJson(defaultFile, defaults)) {
    return false;
  }
  if (!readJson(userFile, user)) {
    return false;
  }
  m_settings = defaults;
  mergeRecursive(m_settings, user);
  m_dirty = false;

  return true;
}

std::vector<ISettingsManager::PathSegment> ISettingsManager::parsePath(const std::string& path) {
  std::vector<PathSegment> segments;
  std::string token;

  auto flushToken = [&]() {
    if (!token.empty()) {
      if (std::ranges::all_of(token, ::isdigit))
        segments.push_back({{}, std::stoul(token), true});
      else
        segments.push_back({token, 0, false});
      token.clear();
    }
  };

  for (const char c : path) {
    if (c == '/')
      flushToken();
    else
      token += c;
  }
  flushToken();
  return segments;
}

const nlohmann::ordered_json* ISettingsManager::jsonAtPath(const std::string& path) const {
  const auto segments = parsePath(path);
  const nlohmann::ordered_json* current = &m_settings;
  for (const auto& [key, index, isIndex] : segments) {
    if (isIndex) {
      if (!current->is_array() || index >= current->size())
        return nullptr;
      current = &(*current)[index];
    } else {
      if (!current->is_object() || !current->contains(key))
        return nullptr;
      current = &(*current)[key];
    }
  }
  return current;
}

bool ISettingsManager::readJson(const std::filesystem::path& file, nlohmann::ordered_json& out) {
  std::ifstream f(file);
  if (!f.is_open()) {
    out = nlohmann::ordered_json::object();
    return false;
  }
  try {
    f >> out;
    return true;
  } catch (const nlohmann::ordered_json::exception& e) {
    out = nlohmann::ordered_json::object();
    RPGM_WARN("Unable to read settings \"{}\"", e.what());
    return false;
  }
}

void ISettingsManager::mergeRecursive(nlohmann::ordered_json& base, const nlohmann::ordered_json& overrideJson) {
  if (!base.is_object() || !overrideJson.is_object()) {
    base = overrideJson;
    return;
  }
  for (auto it = overrideJson.begin(); it != overrideJson.end(); ++it) {
    if (const std::string& key = it.key(); base.contains(key)) {
      mergeRecursive(base[key], it.value());
    } else {
      base[key] = it.value();
    }
  }
}

size_t ISettingsManager::getArraySize(const std::string& path) const {
  const nlohmann::ordered_json* arrNode = jsonAtPath(path);
  if (!arrNode || !arrNode->is_array())
    return 0;
  return arrNode->size();
}

TransientSettingsManager& TransientSettingsManager::instance() {
  static TransientSettingsManager instance;
  return instance;
}
