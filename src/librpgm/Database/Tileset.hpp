#pragma once

#include "Database/IModifiable.hpp"
#include "Database/TileHelper.hpp"

#include <array>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class Tileset final : public IModifiable {
public:
  friend class TilesetsSerializer;
  friend void to_json(nlohmann::ordered_json& j, const Tileset& tileset);
  friend void from_json(const nlohmann::ordered_json& j, Tileset& tileset);
  enum class Mode {
    World,
    Area,
  };

  Tileset() = default;
  Tileset(const Tileset& other);
  Tileset& operator=(const Tileset& other);
  Tileset(Tileset&& other) noexcept;
  Tileset& operator=(Tileset&& other) noexcept;

  [[nodiscard]] int id() const;
  void setId(int id);

  [[nodiscard]] const std::vector<std::optional<int>>& flags() const;

  int flag(int id);
  [[nodiscard]] int flag(int idx) const;

  void setFlags(const std::vector<std::optional<int>>& flags);
  void setFlag(int idx, int flag, bool enabled = true);

  void setTerrainTag(int idx, int tag);
  [[nodiscard]] Mode mode() const;
  void setMode(Mode mode);

  [[nodiscard]] const std::string& name() const;
  void setName(const std::string& name);

  [[nodiscard]] const std::string& note() const;
  void setNote(const std::string& note);

  [[nodiscard]] const std::array<std::string, 9>& tilesetNames() const;
  void setTilesetNames(const std::array<std::string, 9>& tilesetNames);
  [[nodiscard]] const std::string& tilesetName(int idx) const;
  void setTilesetName(int idx, const std::string& name);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  rpgmutils::signal<void(Tileset*, int)>& idModified();
  rpgmutils::signal<void(Tileset*, const std::vector<std::optional<int>>&)>& flagsModified();
  rpgmutils::signal<void(Tileset*, Mode)>& modeModified();
  rpgmutils::signal<void(Tileset*, const std::string&)>& nameModified();
  rpgmutils::signal<void(Tileset*, const std::string&)>& noteModified();
  rpgmutils::signal<void(Tileset*, const std::array<std::string, 9>&)>& tilesetNamesModified();

  [[nodiscard]] bool isValid() const { return m_isValid; }
  void setValid(const bool isValid) { m_isValid = isValid; }

private:
  int m_id;
  std::vector<std::optional<int>> m_flags;
  Mode m_mode;
  std::string m_name;
  std::string m_note;
  std::array<std::string, 9> m_tilesetNames;

  std::optional<int> m_oldid;
  std::optional<std::vector<std::optional<int>>> m_oldflags;
  std::optional<Mode> m_oldmode;
  std::optional<std::string> m_oldname;
  std::optional<std::string> m_oldnote;
  std::optional<std::array<std::string, 9>> m_oldtilesetNames;

  std::optional<rpgmutils::signal<void(Tileset*, int)>> m_idModified;
  std::optional<rpgmutils::signal<void(Tileset*, const std::vector<std::optional<int>>&)>> m_flagsModified;
  std::optional<rpgmutils::signal<void(Tileset*, Mode)>> m_modeModified;
  std::optional<rpgmutils::signal<void(Tileset*, const std::string&)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Tileset*, const std::string&)>> m_noteModified;
  std::optional<rpgmutils::signal<void(Tileset*, const std::array<std::string, 9>&)>> m_tilesetNamesModified;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   *
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::json& j, const Tileset& tileset);
void from_json(const nlohmann::json& j, Tileset& tileset);