#pragma once

#include "Database/Trait.hpp"
#include <nlohmann/json.hpp>
#include <string>

class Class final : public IModifiable {
  friend class ClassesSerializer;
  friend void to_json(nlohmann::ordered_json& to, const Class& cls);
  friend void from_json(const nlohmann::ordered_json& from, Class& cls);

public:
  class Learning final : public IModifiable {
    friend void to_json(nlohmann::ordered_json& to, const Learning& learning);
    friend void from_json(const nlohmann::ordered_json& from, Learning& learning);

  public:
    Learning() = default;
    Learning(const Learning& other);
    Learning& operator=(const Learning& other);
    Learning(Learning&& other) noexcept;
    Learning& operator=(Learning&& other) noexcept;

    int level() const;
    void setLevel(int level);

    const std::string& note() const;
    void setNote(const std::string& note);

    int skillId() const;
    void setSkillId(int skillId);

    void restoreOriginal() override;
    void acceptChanges() override;
    nlohmann::ordered_json serializeOldValues() const override;

    rpgmutils::signal<void(Learning*, int)>& levelModified();
    rpgmutils::signal<void(Learning*, const std::string&)>& noteModified();
    rpgmutils::signal<void(Learning*, int)>& skillIdModified();

  private:
    int m_level{};
    std::string m_note;
    int m_skillId{};

    std::optional<int> m_oldlevel;
    std::optional<std::string> m_oldnote;
    std::optional<int> m_oldskillId;
    std::optional<rpgmutils::signal<void(Learning*, int)>> m_levelModified;
    std::optional<rpgmutils::signal<void(Learning*, const std::string&)>> m_noteModified;
    std::optional<rpgmutils::signal<void(Learning*, int)>> m_skillIdModified;
  };

  Class() = default;
  Class(const Class& other);
  Class& operator=(const Class& other);
  Class(Class&& other) noexcept;
  Class& operator=(Class&& other) noexcept;

  [[nodiscard]] int id() const;
  void setId(const int id);

  [[nodiscard]] const std::array<int, 4>& expParams() const;
  void setExpParams(const std::array<int, 4>& expParams);
  void setExpParam(const int index, const int value);

  /* TODO: Remove */
  [[nodiscard]] std::vector<Trait>& traits();
  [[nodiscard]] const std::vector<Trait>& traits() const;
  void setTraits(const std::vector<Trait>& traits);

  [[nodiscard]] const std::vector<Learning>& learnings() const;
  void setLearnings(const std::vector<Learning>& learnings);

  [[nodiscard]] const std::string& name() const;
  void setName(const std::string& name);

  [[nodiscard]] const std::string& note() const;
  void setNote(const std::string& note);

  [[nodiscard]] const std::array<std::array<int, 99>, 8>& params() const;
  void setParams(const std::array<std::array<int, 99>, 8>& params);

  [[nodiscard]] bool isValid() const;
  void setValid(bool isValid);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Class*, int)>& idModified();
  rpgmutils::signal<void(Class*, const std::array<int, 4>&)>& expParamsModified();
  rpgmutils::signal<void(Class*, int, int)>& expParamModified();
  rpgmutils::signal<void(Class*, const std::vector<Trait>&)>& traitsModified();
  rpgmutils::signal<void(Class*, const std::vector<Learning>&)>& learningsModified();
  rpgmutils::signal<void(Class*, const std::string&)>& nameModified();
  rpgmutils::signal<void(Class*, const std::string&)>& noteModified();
  rpgmutils::signal<void(Class*, const std::array<std::array<int, 99>, 8>&)>& paramsModified();

private:
  int m_id{};
  std::array<int, 4> m_expParams{}; // Base, Extra, Acceleration A, Acceleration B
  std::vector<Trait> m_traits;
  std::vector<Learning> m_learnings;
  std::string m_name;
  std::string m_note;
  std::array<std::array<int, 99>, 8> m_params;

  std::optional<int> m_oldid;
  std::optional<std::array<int, 4>> m_oldexpParams; // Base, Extra, Acceleration A, Acceleration B
  std::optional<std::vector<Trait>> m_oldtraits;
  std::optional<std::vector<Learning>> m_oldlearnings;
  std::optional<std::string> m_oldname;
  std::optional<std::string> m_oldnote;
  std::optional<std::array<std::array<int, 99>, 8>> m_oldparams;

  std::optional<rpgmutils::signal<void(Class*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(Class*, const std::array<int, 4>&)>> m_expParamsModified;
  std::optional<rpgmutils::signal<void(Class*, int, int)>> m_expParamModified;
  std::optional<rpgmutils::signal<void(Class*, const std::vector<Trait>&)>> m_traitsModified;
  std::optional<rpgmutils::signal<void(Class*, const std::vector<Learning>&)>> m_learningsModified;
  std::optional<rpgmutils::signal<void(Class*, const std::string&)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Class*, const std::string&)>> m_noteModified;
  std::optional<rpgmutils::signal<void(Class*, const std::array<std::array<int, 99>, 8>&)>> m_paramsModified;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::ordered_json& to, const Class::Learning& learning);
void from_json(const nlohmann::ordered_json& from, Class::Learning& learning);
void to_json(nlohmann::ordered_json& to, const Class& cls);
void from_json(const nlohmann::ordered_json& from, Class& cls);