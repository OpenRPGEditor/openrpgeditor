#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeParallax.hpp"
#include "Core/CommonUI/ImagePicker.hpp"

struct Project;
struct Dialog_ChangeParallax : IEventDialogController {
  Dialog_ChangeParallax() = delete;
  explicit Dialog_ChangeParallax(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new ChangeParallaxCommand());
    m_image = command->image;
    m_loopHorizontally = command->loopHorizontally;
    m_loopVertically = command->loopVertically;
    m_scrollX = command->scrollX;
    m_scrollY = command->scrollY;

    m_imagePicker.emplace(ImagePicker::PickerMode::Parallax, m_image, "");
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  std::string m_image;
  bool m_loopHorizontally;
  bool m_loopVertically;
  int m_scrollX;
  int m_scrollY;

  int m_image_selection{0};
  std::optional<ImagePicker> m_imagePicker;

  bool m_confirmed{false};
  std::shared_ptr<ChangeParallaxCommand> command;
  std::tuple<bool, bool> result;
};