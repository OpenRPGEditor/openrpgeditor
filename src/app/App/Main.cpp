#define SDL_MAIN_HANDLED

#include <exception>

#include <iostream>
#include "Core/Application.hpp"
#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"

#if defined(_WIN32) && defined(ZLIB_DEBUG)
// This is needed due to freetype's gzip not defining these
#include <stdlib.h>
#include "zlib.h"
#  ifndef verbose
#    define verbose 0
#  endif
extern "C" int z_verbose = verbose;

extern "C" void z_error(char *m) {
  fprintf(stderr, "%s\n", m);
  exit(1);
}

#endif

int main() {
  // try {
  // APP_PROFILE_BEGIN_SESSION_WITH_FILE("App", "profile.json");

  {
    App::Application app{"OpenRPGEditor"};
    app.run();
  }

  // APP_PROFILE_END_SESSION();
  //  } catch (std::exception& e) {
  //    APP_ERROR("Main process terminated with: {}", e.what());
  //  }

  return 0;
}
