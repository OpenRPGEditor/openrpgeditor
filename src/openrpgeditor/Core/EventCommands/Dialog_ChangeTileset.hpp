#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/Tilesets.hpp"
#include "Database/EventCommands/ChangeTileset.hpp"

struct Project;
struct Dialog_ChangeTileset : IEventDialogController {
  Dialog_ChangeTileset() = delete;
  explicit Dialog_ChangeTileset(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ChangeTilesetCommand());
    m_tileset = command->tileset;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_tileset;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Tileset>> tileset_picker;
  std::shared_ptr<ChangeTilesetCommand> command;
  std::tuple<bool, bool> result;
};