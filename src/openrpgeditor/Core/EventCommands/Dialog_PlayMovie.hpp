#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Log.hpp"
#include "Core/Settings.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/PlayMovie.hpp"

#include <iostream>

namespace fs = std::filesystem;
struct Dialog_PlayMovie : IEventDialogController {
  Dialog_PlayMovie() = delete;
  explicit Dialog_PlayMovie(const std::string& name,
                            const std::shared_ptr<PlayMovieCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new PlayMovieCommand());
    }
    m_movie = command->name;
    try {
      auto files = getFileNames(Database::instance()->basePath + "movies/");
      for (const auto& file : files) {
        m_movies.push_back(file);
      }
    } catch (const std::filesystem::filesystem_error& e) {
      std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    m_movie = "";
  }
  std::tuple<bool, bool> draw() override;
  [[nodiscard]] std::shared_ptr<IEventCommand> getCommand() override { return command; }

private:
  bool m_confirmed{false};

  int m_selected = 0;
  std::string m_movie;

  std::shared_ptr<PlayMovieCommand> command;
  std::tuple<bool, bool> result;
  std::vector<std::string> m_movies;
  std::vector<std::string> getFileNames(const std::string& directoryPath) {
    std::vector<std::string> fileNames;

    for (const auto& entry : fs::directory_iterator(directoryPath)) {
      std::string filename = entry.path().filename().string();
      size_t lastDotPos = filename.find_last_of(".");
      if (lastDotPos != std::string::npos) {
        std::string str = filename.substr(0, lastDotPos);
        fileNames.push_back(str);
      } else {
        fileNames.push_back(filename);
      }
    }
    return fileNames;
  }
};
