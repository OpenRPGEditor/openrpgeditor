#pragma once

#include <filesystem>
#include <string_view>

namespace App {

class Resources {
public:
  [[nodiscard]] static std::filesystem::path resource_path(const std::filesystem::path& file_path);
  [[nodiscard]] static std::filesystem::path font_path(std::string_view font_file);
  [[nodiscard]] static std::filesystem::path image_path(std::string_view image_file);
};

} // namespace App
