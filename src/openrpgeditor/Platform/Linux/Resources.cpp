#include "Core/Resources.hpp"

#include <SDL2/SDL.h>

#include <filesystem>
#include <string_view>

#include "Core/Debug/Instrumentor.hpp"

namespace App {

std::filesystem::path Resources::resource_path(const std::filesystem::path& file_path) {
  const char* base_path = SDL_GetBasePath();
  std::filesystem::path font_path{base_path};
  SDL_free((void*)base_path);
  font_path /= "../share";
  font_path /= "fonts" / file_path;
  return font_path;
}

std::filesystem::path Resources::font_path(const std::string_view& font_file) { return resource_path(font_file); }

} // namespace App
