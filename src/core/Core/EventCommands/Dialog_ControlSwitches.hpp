#pragma once
#include "IDialogController.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Weapons.hpp"
struct Project;
struct Dialog_ControlSwitches : IDialogController {
  Dialog_ControlSwitches() = delete;
  explicit Dialog_ControlSwitches(const char* name) : IDialogController(name) {}
  void draw() override;
};