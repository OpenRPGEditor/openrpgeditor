#include "Core/Resources.hpp"

#include <SDL3/SDL.h>

#include <filesystem>
#include <string>
#include <string_view>

#include "Core/Debug/Instrumentor.hpp"

namespace App {

std::filesystem::path Resources::resource_path(const std::filesystem::path& file_path) {
  const char* base_path = SDL_GetBasePath();
  std::filesystem::path font_path{base_path};
  font_path /= file_path;
  return font_path;
}

std::filesystem::path Resources::font_path(const std::string_view& font_file) { return resource_path("fonts") / font_file; }
std::filesystem::path Resources::image_path(const std::string_view& image_file) { return resource_path("images") / image_file; }

} // namespace App
