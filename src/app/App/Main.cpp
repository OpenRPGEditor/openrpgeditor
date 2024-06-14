#define SDL_MAIN_HANDLED

#include <exception>

#include <iostream>
#include "Core/Application.hpp"
#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"

static std::string buff;
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
  return fwrite(contents, size, nmemb, (FILE*)userp);
}

int main() {
  // try {
  // APP_PROFILE_BEGIN_SESSION_WITH_FILE("App", "profile.json");

  {
    App::Application app{"OpenRPGMaker"};
    app.run();
  }

  // APP_PROFILE_END_SESSION();
  //  } catch (std::exception& e) {
  //    APP_ERROR("Main process terminated with: {}", e.what());
  //  }

  return 0;
}
