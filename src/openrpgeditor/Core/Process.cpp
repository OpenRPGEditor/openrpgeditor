#include "Core/Process.hpp"

Process::Process(const std::filesystem::path& filePath, const std::vector<std::string>& arguments, const bool redirectOutput)
: m_filePath(filePath.generic_string())
, m_arguments(arguments)
, m_redirectOutput(redirectOutput) {}

Process::~Process() {
  if (m_handle) {
    SDL_KillProcess(m_handle, true);
  }
  SDL_DestroyProcess(m_handle);
  SDL_DestroyProperties(m_optionsHandle);
}

bool Process::exec() {
  if (!m_handle) {
    m_argumentArray.clear();
    m_argumentArray.resize(m_arguments.size() + 2);
    m_argumentArray[0] = m_filePath.c_str();
    for (int i = 1; i < m_arguments.size(); i++) {
      m_argumentArray[i] = m_arguments[i].c_str();
    }

    m_argumentArray.back() = nullptr;

    if (m_optionsHandle == 0) {
      m_optionsHandle = SDL_CreateProperties();
    }
    // TODO: add output redirection
    SDL_SetPointerProperty(m_optionsHandle, SDL_PROP_PROCESS_CREATE_ARGS_POINTER, m_argumentArray.data());
    m_handle = SDL_CreateProcessWithProperties(m_optionsHandle);
  }
  return SDL_WaitProcess(m_handle, false, &m_status);
}
