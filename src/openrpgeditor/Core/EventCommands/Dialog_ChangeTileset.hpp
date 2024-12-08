#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/ChangeTileset.hpp"
#include "Database/Tilesets.hpp"

struct Dialog_ChangeTileset : IEventDialogController {
  Dialog_ChangeTileset() = delete;
  explicit Dialog_ChangeTileset(const std::string& name, const std::shared_ptr<ChangeTilesetCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeTilesetCommand());
    }
    m_tileset = command->tileset;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_tileset;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Tileset>> tileset_picker;
  std::shared_ptr<ChangeTilesetCommand> command;
  std::tuple<bool, bool> result;
};