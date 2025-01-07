#define SDL_MAIN_HANDLED

#include <exception>

#include <iostream>

#if defined(_WIN32) && defined(DEBUG)
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#endif

#include "Core/Application.hpp"
#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"
#include <orei18n.hpp>

#include "Database/Globals.hpp"

#if defined(_WIN32) && defined(ZLIB_DEBUG)
// This is needed due to freetype's gzip not defining these
#include <stdlib.h>
#ifndef verbose
#define verbose 0
#endif
extern "C" int z_verbose = verbose;

extern "C" void z_error(char* m) {
  fprintf(stderr, "%s\n", m);
  exit(1);
}

#endif

// template <typename E>
// void translateEnum(bool withValue = false) {
//   std::cout << "enum class " << magic_enum::enum_type_name<E>() << " {" << std::endl;
//   for (const auto& v : magic_enum::enum_values<E>()) {
//     const auto decoName = DecodeEnumName(magic_enum::enum_name(v));
//     const auto undecoName = UndectorateEnumName(magic_enum::enum_name(v));
//     std::cout << std::format("  trENUM(\"{}\", ", decoName);
//     if (decoName != undecoName) {
//       std::cout << std::format("trENUM(\"{}\", ", undecoName);
//     }
//     std::cout << magic_enum::enum_name(v) << ")";
//     if (decoName != undecoName) {
//       std::cout << ")" << std::endl;
//     }
//     if (withValue) {
//       std::cout << " = " << static_cast<int>(v);
//     }
//     std::cout << "," << std::endl;
//   }
//   std::cout << "};" << std::endl;
// }

int main() {
  srand(time(NULL));
#if defined(_WIN32) && defined(DEBUG)
  AllocConsole();

  HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
  int hCrt = _open_osfhandle((size_t)handle_out, _O_TEXT);
  FILE* hf_out = _fdopen(hCrt, "w");
  setvbuf(hf_out, NULL, _IONBF, 1);
  *stdout = *hf_out;

  HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
  hCrt = _open_osfhandle((size_t)handle_in, _O_TEXT);
  FILE* hf_in = _fdopen(hCrt, "r");
  setvbuf(hf_in, NULL, _IONBF, 128);
  *stdin = *hf_in;

  HANDLE handle_err = GetStdHandle(STD_ERROR_HANDLE);
  hCrt = _open_osfhandle((size_t)handle_err, _O_TEXT);
  FILE* hf_err = _fdopen(hCrt, "r");
  setvbuf(hf_err, NULL, _IONBF, 128);
  *stderr = *hf_err;
#endif

#ifndef DEBUG
  try {
#endif
    // APP_PROFILE_BEGIN_SESSION_WITH_FILE("App", "profile.json");
    setlocale(LC_ALL, "");
    {
      App::Application app{"OpenRPGEditor"};
      app.run();
    }

    // APP_PROFILE_END_SESSION();
#ifndef DEBUG
  } catch (std::exception& e) { APP_ERROR("Main process terminated with: {}", e.what()); }
#endif

  return 0;
}
