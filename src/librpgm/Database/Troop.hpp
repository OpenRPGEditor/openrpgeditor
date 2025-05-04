#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

#include <nlohmann/json.hpp>

class Troop final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& j, const Troop& t);
  friend void from_json(const nlohmann::ordered_json& j, Troop& t);
  friend class TroopsSerializer;

public:
  class Member final : public IModifiable {
    friend void to_json(nlohmann::ordered_json& j, const Member& m);
    friend void from_json(const nlohmann::ordered_json& j, Member& m);

  public:
    Member() = default;
    Member(const Member& other);
    Member& operator=(const Member& other);
    Member(Member&& other) noexcept;
    Member& operator=(Member&& other) noexcept;

    int enemyId() const;
    void setEnemyId(int value);

    int x() const;
    void setX(int value);

    int y() const;
    void setY(int value);

    bool hidden() const;
    void setHidden(bool value);

    void restoreOriginal() override;
    void acceptChanges() override;
    nlohmann::ordered_json serializeOldValues() const override;

    rpgmutils::signal<void(Member*, int)>& enemyIdModified();
    rpgmutils::signal<void(Member*, int)>& xModified();
    rpgmutils::signal<void(Member*, int)>& yModified();
    rpgmutils::signal<void(Member*, bool)>& hiddenModified();

    bool operator==(const Member& other) const { return m_enemyId == other.m_enemyId && m_x == other.m_x && m_y == other.m_y && m_hidden == other.m_hidden; }

  private:
    int m_enemyId = 0;
    int m_x = 0;
    int m_y = 0;
    bool m_hidden = false;

    std::optional<int> m_oldenemyId;
    std::optional<int> m_oldx;
    std::optional<int> m_oldy;
    std::optional<bool> m_oldhidden;

    std::optional<rpgmutils::signal<void(Member*, int)>> m_enemyIdModified;
    std::optional<rpgmutils::signal<void(Member*, int)>> m_xModified;
    std::optional<rpgmutils::signal<void(Member*, int)>> m_yModified;
    std::optional<rpgmutils::signal<void(Member*, bool)>> m_hiddenModified;
  };

  class Conditions final : public IModifiable {
    friend void to_json(nlohmann::ordered_json& j, const Conditions& c);
    friend void from_json(const nlohmann::ordered_json& j, Conditions& c);

  public:
    Conditions() = default;
    Conditions(const Conditions& other);
    Conditions& operator=(const Conditions& other);
    Conditions(Conditions&& other) noexcept;
    Conditions& operator=(Conditions&& other) noexcept;

    int actorHp() const;
    void setActorHp(int value);

    int actorId() const;
    void setActorId(int value);

    bool actorValid() const;
    void setActorValid(bool value);

    int enemyIndex() const;
    void setEnemyIndex(int value);

    int enemyHp() const;
    void setEnemyHp(int value);

    bool enemyValid() const;
    void setEnemyValid(bool value);

    int switchId() const;
    void setSwitchId(int value);

    bool switchValid() const;
    void setSwitchValid(bool value);

    int turnA() const;
    void setTurnA(int value);

    int turnB() const;
    void setTurnB(int value);

    bool turnEnding() const;
    void setTurnEnding(bool value);

    bool turnValid() const;
    void setTurnValid(bool value);

    void restoreOriginal() override;
    void acceptChanges() override;
    nlohmann::ordered_json serializeOldValues() const override;

    rpgmutils::signal<void(Conditions*, int)>& actorHpModified();
    rpgmutils::signal<void(Conditions*, int)>& actorIdModified();
    rpgmutils::signal<void(Conditions*, bool)>& actorValidModified();
    rpgmutils::signal<void(Conditions*, int)>& enemyIndexModified();
    rpgmutils::signal<void(Conditions*, int)>& enemyHpModified();
    rpgmutils::signal<void(Conditions*, bool)>& enemyValidModified();
    rpgmutils::signal<void(Conditions*, int)>& switchIdModified();
    rpgmutils::signal<void(Conditions*, bool)>& switchValidModified();
    rpgmutils::signal<void(Conditions*, int)>& turnAModified();
    rpgmutils::signal<void(Conditions*, int)>& turnBModified();
    rpgmutils::signal<void(Conditions*, bool)>& turnEndingModified();
    rpgmutils::signal<void(Conditions*, bool)>& turnValidModified();

    bool operator==(const Conditions& other) const {
      return m_actorHp == other.m_actorHp && m_actorId == other.m_actorId && m_enemyIndex == other.m_enemyIndex && m_enemyHp == other.m_enemyHp && m_enemyValid == other.m_enemyValid &&
             m_switchId == other.m_switchId && m_switchValid == other.m_switchValid && m_turnA == other.m_turnA && m_turnB == other.m_turnB && m_turnEnding == other.m_turnEnding;
    }

  private:
    int m_actorHp{50};
    int m_actorId{1};
    bool m_actorValid{false};
    int m_enemyIndex{0};
    int m_enemyHp{50};
    bool m_enemyValid{false};
    int m_switchId{1};
    bool m_switchValid{false};
    int m_turnA{0};
    int m_turnB{0};
    bool m_turnEnding{false};
    bool m_turnValid{false};

    std::optional<int> m_oldactorHp;
    std::optional<int> m_oldactorId;
    std::optional<bool> m_oldactorValid;
    std::optional<int> m_oldenemyIndex;
    std::optional<int> m_oldenemyHp;
    std::optional<bool> m_oldenemyValid;
    std::optional<int> m_oldswitchId;
    std::optional<bool> m_oldswitchValid;
    std::optional<int> m_oldturnA;
    std::optional<int> m_oldturnB;
    std::optional<bool> m_oldturnEnding;
    std::optional<bool> m_oldturnValid;

    std::optional<rpgmutils::signal<void(Conditions*, int)>> m_actorHpModified;
    std::optional<rpgmutils::signal<void(Conditions*, int)>> m_actorIdModified;
    std::optional<rpgmutils::signal<void(Conditions*, bool)>> m_actorValidModified;
    std::optional<rpgmutils::signal<void(Conditions*, int)>> m_enemyIndexModified;
    std::optional<rpgmutils::signal<void(Conditions*, int)>> m_enemyHpModified;
    std::optional<rpgmutils::signal<void(Conditions*, bool)>> m_enemyValidModified;
    std::optional<rpgmutils::signal<void(Conditions*, int)>> m_switchIdModified;
    std::optional<rpgmutils::signal<void(Conditions*, bool)>> m_switchValidModified;
    std::optional<rpgmutils::signal<void(Conditions*, int)>> m_turnAModified;
    std::optional<rpgmutils::signal<void(Conditions*, int)>> m_turnBModified;
    std::optional<rpgmutils::signal<void(Conditions*, bool)>> m_turnEndingModified;
    std::optional<rpgmutils::signal<void(Conditions*, bool)>> m_turnValidModified;
  };
  class Page final : public IModifiable {
    friend void to_json(nlohmann::ordered_json& j, const Page& p);
    friend void from_json(const nlohmann::ordered_json& j, Page& p);

  public:
    Page() = default;
    Page(const Page& other);
    Page& operator=(const Page& other);
    Page(Page&& other) noexcept;
    Page& operator=(Page&& other) noexcept;

    Conditions& conditions();
    const Conditions& conditions() const;
    void setConditions(const Conditions& value);

    std::vector<std::shared_ptr<IEventCommand>>& list();
    const std::vector<std::shared_ptr<IEventCommand>>& list() const;
    void setList(const std::vector<std::shared_ptr<IEventCommand>>& value);

    int span() const;
    void setSpan(int value);

    void restoreOriginal() override;
    void acceptChanges() override;
    nlohmann::ordered_json serializeOldValues() const override;

    bool isModified() const override {
      bool modified = IModifiable::isModified();
      modified |= m_conditions.isModified();
      modified |= std::ranges::any_of(m_list, [](const std::shared_ptr<IEventCommand>& command) { return command->isModified(); });
      return modified;
    }
    rpgmutils::signal<void(Page*, const Conditions&)>& conditionsModified();
    rpgmutils::signal<void(Page*, const std::vector<std::shared_ptr<IEventCommand>>&)>& listModified();
    rpgmutils::signal<void(Page*, int)>& spanModified();

    bool operator==(const Page& other) const { return m_conditions == m_conditions && m_list == other.m_list && m_span == other.m_span; }

  private:
    Conditions m_conditions;
    std::vector<std::shared_ptr<IEventCommand>> m_list;
    int m_span;

    std::optional<Conditions> m_oldconditions;
    std::optional<std::vector<std::shared_ptr<IEventCommand>>> m_oldlist;
    std::optional<int> m_oldspan;

    std::optional<rpgmutils::signal<void(Page*, const Conditions&)>> m_conditionsModified;
    std::optional<rpgmutils::signal<void(Page*, const std::vector<std::shared_ptr<IEventCommand>>&)>> m_listModified;
    std::optional<rpgmutils::signal<void(Page*, int)>> m_spanModified;
  };

  Troop() = default;
  Troop(const Troop& other);
  Troop& operator=(const Troop& other);
  Troop(Troop&& other) noexcept;
  Troop& operator=(Troop&& other) noexcept;

  Member* member(int index);
  const Member* member(int index) const;

  int id() const;
  void setId(int id);

  std::vector<Member>& members();
  const std::vector<Member>& members() const;
  void setMembers(const std::vector<Member>& value);

  const std::string& name() const;
  void setName(const std::string& name);

  std::vector<Page>& pages();
  const std::vector<Page>& pages() const;
  void setPages(const std::vector<Page>& value);
  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  bool isModified() const override {
    bool modified = IModifiable::isModified();
    modified |= std::ranges::any_of(m_members, [](const auto& member) { return member.isModified(); });
    modified |= std::ranges::any_of(m_pages, [](const auto& page) { return page.isModified(); });
    return modified;
  }

  rpgmutils::signal<void(Troop*, int)>& idModified();
  rpgmutils::signal<void(Troop*, const std::vector<Member>&)>& membersModified();
  rpgmutils::signal<void(Troop*, const std::string&)>& nameModified();
  rpgmutils::signal<void(Troop*, const std::vector<Page>&)>& pagesModified();

  bool isValid() const { return m_isValid; }
  void setValid(bool valid) { m_isValid = valid; }

private:
  int m_id;
  std::vector<Member> m_members;
  std::string m_name;
  std::vector<Page> m_pages;

  std::optional<int> m_oldid;
  std::optional<std::vector<Member>> m_oldmembers;
  std::optional<std::string> m_oldname;
  std::optional<std::vector<Page>> m_oldpages;

  std::optional<rpgmutils::signal<void(Troop*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(Troop*, const std::vector<Member>&)>> m_membersModified;
  std::optional<rpgmutils::signal<void(Troop*, const std::string&)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Troop*, const std::vector<Page>&)>> m_pagesModified;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::ordered_json& j, const Troop::Conditions& c);
void from_json(const nlohmann::ordered_json& j, Troop::Conditions& c);
void to_json(nlohmann::ordered_json& j, const Troop::Page& p);
void from_json(nlohmann::ordered_json& j, Troop::Page& p);
void to_json(nlohmann::ordered_json& j, const Troop::Member& m);
void from_json(const nlohmann::ordered_json& j, Troop::Member& m);
void to_json(nlohmann::ordered_json& j, const Troop& t);
void from_json(const nlohmann::ordered_json& j, Troop& t);