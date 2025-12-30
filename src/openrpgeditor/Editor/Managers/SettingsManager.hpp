#pragma once
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class SettingsManager {
public:
  static SettingsManager& instance();

  [[nodiscard]] bool load(const std::filesystem::path& filename);
  [[nodiscard]] bool save(const std::filesystem::path& filename);

  [[nodiscard]] bool saveForce(const std::filesystem::path& filename) const;
  void merge(const nlohmann::ordered_json& overrideJson);
  bool loadMerged(const std::filesystem::path& defaultFile, const std::filesystem::path& userFile);

  template <typename T>
  T getValue(const std::string& path, const T& defaultValue = {}) const {
    const nlohmann::ordered_json* current = jsonAtPath(path);
    if (!current)
      return defaultValue;
    try {
      return current->get<T>();
    } catch (...) { return defaultValue; }
  }

  template <typename T>
  void setValue(const std::string& path, const T& newValue) {
    const std::vector<PathSegment> segments = parsePath(path);
    if (segments.empty())
      return;

    nlohmann::ordered_json* current = &m_settings;

    for (size_t i = 0; i < segments.size() - 1; ++i) {

      if (const auto& [key, index, isIndex] = segments[i]; isIndex) {
        // Ensure current node is an array
        if (!current->is_array())
          *current = nlohmann::ordered_json::array();

        // Extend array manually if index out of bounds
        while (current->size() <= index)
          current->push_back(nlohmann::ordered_json::object());

        current = &(*current)[index];
      } else {
        // Ensure current node is an object
        if (!current->is_object())
          *current = nlohmann::ordered_json::object();

        if (!current->contains(key))
          (*current)[key] = nlohmann::ordered_json::object();

        current = &(*current)[key];
      }
    }

    // Set final element
    const auto& [key, index, isIndex] = segments.back();
    nlohmann::ordered_json* target = nullptr;

    if (isIndex) {
      if (!current->is_array())
        *current = nlohmann::ordered_json::array();
      while (current->size() <= index)
        current->push_back(nullptr);
      target = &(*current)[index];
    } else {
      if (!current->is_object())
        *current = nlohmann::ordered_json::object();
      target = &(*current)[key];
    }

    auto newJson = nlohmann::ordered_json(newValue);
    if (!target->is_null() && *target == newJson)
      return; // no change

    *target = std::move(newJson);
    m_dirty = true;
  }

  template <typename T>
  void appendValue(const std::string& path, const T& newElement) {
    std::vector<PathSegment> segments = parsePath(path);
    if (segments.empty())
      return;

    nlohmann::ordered_json* current = &m_settings;

    // Traverse until final path node
    for (const auto& [key, index, isIndex] : segments) {
      if (isIndex) {
        if (!current->is_array())
          *current = nlohmann::ordered_json::array();
        while (current->size() <= index)
          current->push_back(nlohmann::ordered_json::object());
        current = &(*current)[index];
      } else {
        if (!current->is_object())
          *current = nlohmann::ordered_json::object();
        if (!current->contains(key))
          (*current)[key] = nlohmann::ordered_json::array(); // create as array if last
        current = &(*current)[key];
      }
    }

    // Ensure final node is array
    if (!current->is_array())
      *current = nlohmann::ordered_json::array();

    current->push_back(nlohmann::ordered_json(newElement));
    m_dirty = true;
  }

  template <typename T>
  T getArrayElement(const std::string& path, const size_t index, const T& defaultValue = {}) const {
    const nlohmann::ordered_json* arrNode = jsonAtPath(path);
    if (!arrNode || !arrNode->is_array() || index >= arrNode->size())
      return defaultValue;

    try {
      return (*arrNode)[index].get<T>();
    } catch (...) { return defaultValue; }
  }

  [[nodiscard]] size_t getArraySize(const std::string& path) const;

  [[nodiscard]] bool isDirty() const { return m_dirty; }

  [[nodiscard]] std::string dump(const int indent = 4) const { return m_settings.dump(indent); }
  
private:
  nlohmann::ordered_json m_settings = nlohmann::ordered_json::object();
  bool m_dirty = false;

  SettingsManager() = default;

  struct PathSegment {
    std::string key;
    size_t index = 0;
    bool isIndex = false;
  };

  static std::vector<PathSegment> parsePath(const std::string& path);

  [[nodiscard]] const nlohmann::ordered_json* jsonAtPath(const std::string& path) const;

  static bool readJson(const std::filesystem::path& file, nlohmann::ordered_json& out);

  static void mergeRecursive(nlohmann::ordered_json& base, const nlohmann::ordered_json& overrideJson);
};
