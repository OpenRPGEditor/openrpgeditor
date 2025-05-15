#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/ChangeTileset.hpp"
#include "Database/Tilesets.hpp"

struct Dialog_ChangeTileset : IEventDialogController {
  Dialog_ChangeTileset() = delete;
  explicit Dialog_ChangeTileset(const std::string& name, const std::shared_ptr<ChangeTilesetCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeTilesetCommand());
    }
    m_tileset = m_command->tileset;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_tileset;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Tileset>> m_tilesetPicker;
  std::shared_ptr<ChangeTilesetCommand> m_command;
};