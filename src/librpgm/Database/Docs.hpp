#pragma once

#include "Database/Doc.hpp"

class Docs final : public IModifiable {

public:
  friend void to_json(nlohmann::ordered_json& j, const Docs& templ);
  friend void from_json(const nlohmann::ordered_json& j, Docs& templ);
  std::vector<Doc> docs;

  static Docs load(std::string_view path);
  bool serialize(std::string_view path);

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(docs, [](const Doc& t) { return t.isModified(); });
  }
};
void to_json(nlohmann::ordered_json& j, const Docs& templ);
void from_json(const nlohmann::ordered_json& j, Docs& templ);
