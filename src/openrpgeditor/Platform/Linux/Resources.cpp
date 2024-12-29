#include "Core/Resources.hpp"

#include <SDL2/SDL.h>

#include <filesystem>
#include <string_view>

#include "Core/Debug/Instrumentor.hpp"

namespace App {

std::filesystem::path Resources::resource_path(const std::filesystem::path& file_path) {
  const char* base_path = SDL_GetBasePath();
  std::filesystem::path path{base_path};
  SDL_free((void*)base_path);
  path /= "../share" / file_path;
  return path;
}

std::filesystem::path Resources::font_path(const std::string_view& font_file) { return resource_path("fonts") / font_file; }
std::filesystem::path Resources::image_path(const std::string_view& image_file) { return resource_path("images") / image_file; }

} // namespace App
