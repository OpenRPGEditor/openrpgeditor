#pragma once

#include <filesystem>
#include <SDL3/SDL_process.h>
#include <string>
#include <vector>

class Process {
public:
  Process(const std::filesystem::path& filePath, const std::vector<std::string>& arguments, bool redirectOutput = true);
  ~Process();
  bool exec();

  [[nodiscard]] int status() const { return m_status; }

private:
  SDL_PropertiesID m_optionsHandle{};
  SDL_Process* m_handle{nullptr};
  int m_status{0};
  bool m_redirectOutput{false};
  std::string m_filePath;
  std::vector<std::string> m_arguments;
  std::vector<const char*> m_argumentArray;
};
