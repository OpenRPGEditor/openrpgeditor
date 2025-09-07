#pragma once

#include "IDialogController.hpp"

#include <optional>
#include <string>
#include <vector>

#include <string_view>

#include <Utils/Typename.hpp>

/*!
 * @struct ObjectPicker
 * @details Provides a pickable list broken up into groups of 100
 * @tparam T Object type to list
 */
template <typename T>
class ObjectPicker {
  static constexpr std::string_view Name = type_name<T>();

public:
  std::tuple<bool, bool> draw();

  [[nodiscard]] int selection() const { return m_selection; }

  static int getId(const T& value) { return value.id(); }
  static const std::string& getName(const T& value) { return value.name(); }

  [[nodiscard]] bool isOpen() const { return m_open; }
  [[nodiscard]] bool isConfirmed() const { return m_confirmed; }
  void setOpen(const bool open) { m_open = open; }
  void accept() {
    m_open = false;
    m_confirmed = false;
  }

  void setNoSelectionMeansAdd(const bool noSelectionMeansAdd) { m_noSelectionMeansAdd = noSelectionMeansAdd; }
  void setUseDummyEntry(const std::string_view type) {
    m_useDummyEntry = true;
    m_entireText = type;
  }
  bool drawPicker(int& selectedData) {
    auto [closed, confirmed] = draw();
    if (closed) {
      if (confirmed) {
        selectedData = selection();
      }
    }
    return closed;
  }

protected:
  ObjectPicker(const std::string_view title, std::vector<T>& list, const int initialSelection)
  : m_title(title)
  , m_list(&list)
  , m_selection(initialSelection) {
    if (m_selection > 0) {
      m_navigateOnOpen = true;
    }
  }

private:
  std::string m_title;
  std::vector<T>* m_list;
  bool m_confirmed{false};
  bool m_navigateOnOpen{false};
  int m_selection{0};
  bool m_open{false};
  bool m_noSelectionMeansAdd{false};
  std::string m_filter;
  std::string m_entireText;
  bool m_useDummyEntry{false};
};

#include "Core/CommonUI/ObjectPicker.inl"

#include "Database/Actor.hpp"
#include "Database/Animation.hpp"
#include "Database/Armor.hpp"
#include "Database/Class.hpp"
#include "Database/CommonEvent.hpp"
#include "Database/Enemy.hpp"
#include "Database/Item.hpp"
#include "Database/MapInfo.hpp"
#include "Database/Skill.hpp"
#include "Database/State.hpp"
#include "Database/Template.hpp"
#include "Database/Tileset.hpp"
#include "Database/Troop.hpp"
#include "Database/Weapon.hpp"

class ActorPicker final : public ObjectPicker<Actor> {
public:
  ActorPicker(std::vector<Actor>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Actor Selection"), list, initialSelection) {}
  ActorPicker(const std::string_view title, std::vector<Actor>& list, const int initialSelection)
  : ObjectPicker(title, list, initialSelection) {}
};

class ClassPicker final : public ObjectPicker<Class> {
public:
  ClassPicker(std::vector<Class>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Class Selection"), list, initialSelection) {}
};

class SkillPicker final : public ObjectPicker<Skill> {
public:
  SkillPicker(std::vector<Skill>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Skill Selection"), list, initialSelection) {}
};

class ItemPicker final : public ObjectPicker<Item> {
public:
  ItemPicker(std::vector<Item>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Item Selection"), list, initialSelection) {}
};

class WeaponPicker final : public ObjectPicker<Weapon> {
public:
  WeaponPicker(std::vector<Weapon>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Weapon Selection"), list, initialSelection) {}
};

class ArmorPicker final : public ObjectPicker<Armor> {
public:
  ArmorPicker(std::vector<Armor>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Armor Selection"), list, initialSelection) {}
};

class EnemyPicker final : public ObjectPicker<Enemy> {
public:
  EnemyPicker(std::vector<Enemy>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Enemy Selection"), list, initialSelection) {}
};

class TroopPicker final : public ObjectPicker<Troop> {
public:
  TroopPicker(std::vector<Troop>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Troop Selection"), list, initialSelection) {}
};

class StatePicker final : public ObjectPicker<State> {
public:
  StatePicker(std::vector<State>& list, const int initialSelection)
  : ObjectPicker(trNOOP("State Selection"), list, initialSelection) {}
};

class AnimationPicker final : public ObjectPicker<Animation> {
public:
  AnimationPicker(std::vector<Animation>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Animation Selection"), list, initialSelection) {}
};

class TilesetPicker final : public ObjectPicker<Tileset> {
public:
  TilesetPicker(std::vector<Tileset>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Tileset Selection"), list, initialSelection) {}
};

class CommonEventPicker final : public ObjectPicker<std::optional<CommonEvent>> {
public:
  CommonEventPicker(std::vector<std::optional<CommonEvent>>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Common Event Selection"), list, initialSelection) {}
};

class MapInfoPicker final : public ObjectPicker<std::optional<MapInfo>> {
public:
  MapInfoPicker(std::vector<std::optional<MapInfo>>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Map Selection"), list, initialSelection) {}
};

class TemplatePicker final : public ObjectPicker<Template> {
public:
  TemplatePicker(std::vector<Template>& list, const int initialSelection)
  : ObjectPicker(trNOOP("Template Selection"), list, initialSelection) {}
};