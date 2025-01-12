#pragma once
#include "Database/IModifiable.hpp"
#include "nlohmann/json.hpp"

class Battler final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& out, const Battler& battler);
  friend void from_json(const nlohmann::ordered_json& from, Battler& battler);

public:
  Battler() = default;
  Battler(const Battler& other);
  Battler& operator=(const Battler& other);
  Battler(Battler&& other) noexcept;
  Battler& operator=(Battler&& other) noexcept;

  int actorId() const;
  void setActorId(int actorId);

  const std::vector<int>& equips() const;
  void setEquips(const std::vector<int>& equips);
  void setEquip(int index, int equip);

  int level() const;
  void setLevel(int level);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Battler*, int)>& actorIdModified();
  rpgmutils::signal<void(Battler*, const std::vector<int>&)>& equipsModified();
  rpgmutils::signal<void(Battler*, int, int)>& equipModified();
  rpgmutils::signal<void(Battler*, int)>& levelModified();

  bool operator==(const Battler& other) const { return m_actorId == other.m_actorId && m_equips == other.m_equips && m_level == other.m_level; }

private:
  int m_actorId{};
  std::vector<int> m_equips;
  int m_level{};

  std::optional<int> m_oldactorId;
  std::optional<std::vector<int>> m_oldequips;
  std::optional<int> m_oldlevel;

  std::optional<rpgmutils::signal<void(Battler*, int)>> m_actorIdModified;
  std::optional<rpgmutils::signal<void(Battler*, const std::vector<int>&)>> m_equipsModified;
  std::optional<rpgmutils::signal<void(Battler*, int, int)>> m_equipModified;
  std::optional<rpgmutils::signal<void(Battler*, int)>> m_levelModified;
};

void to_json(nlohmann::ordered_json& out, const Battler& battler);
void from_json(const nlohmann::ordered_json& from, Battler& battler);