#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/PlayMovie.hpp"
#include "Editor/CommonUI/Directory.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"
#include "Editor/Log.hpp"
#include "Editor/Settings.hpp"

#include <iostream>

namespace fs = std::filesystem;
struct Dialog_PlayMovie : IEventDialogController {
  Dialog_PlayMovie() = delete;
  explicit Dialog_PlayMovie(const std::string& name, const std::shared_ptr<PlayMovieCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (!m_command) {
      m_command = std::make_shared<PlayMovieCommand>();
    }
    m_movieDir.emplace("movies", ".mp4", m_command->name); // TODO: Add multiple filter format, include .webm
    m_movies = m_movieDir.value().getDirectoryContents();
    m_folders = m_movieDir.value().getDirectories();
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return m_command; }

  bool playMovie(const char* path) {

    // TODO

    // if (!buffer.loadFromFile(Database::instance()->basePath + path + m_movieDir.value().extFilter)) {
    //   // error loading file
    //   return false;
    // }
    return false;
  }

private:
  bool m_confirmed{false};

  int m_selected = 0;
  std::string m_movie;

  std::shared_ptr<PlayMovieCommand> m_command;
  std::tuple<bool, bool> result;
  std::vector<std::string> m_movies;

  int m_selectedFolder{-1};
  std::optional<Directory> m_movieDir;
  std::vector<std::string> m_folders;
};
