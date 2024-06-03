#pragma once


#include "Database/Trait.hpp"

#include <string>
#include <array>
#include <vector>
#include <string_view>

#include <nlohmann/json.hpp>

class Actor {
public:

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Actor, id, battlerName, characterIndex, characterName, classId, equips, faceIndex,
                                 faceName, traits, maxLevel, name, nickname, note, profile);
  int id = 0;
  std::string battlerName;
  int characterIndex = 0;
  std::string characterName;
  int classId = 0;
  std::vector<int> equips;
  int faceIndex = 0;
  std::string faceName;
  std::vector<Trait> traits;
  int maxLevel = 99;
  std::string name;
  std::string nickname;
  std::string note;
  std::string profile;

  void draw();

  void setSelected(bool selected) { m_selected = selected; }
  bool selected() const { return m_selected; }

private:
  bool m_selected = false;
};

class Actors {
public:
  Actors();

  static Actors load(std::string_view filename);
  bool serialize(std::string_view filename);

  void draw();
  // private:
  std::vector<Actor> m_actors;
  std::optional<Actor> m_selectedActor;
  bool m_isOpen;
};
