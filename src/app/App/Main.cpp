#define SDL_MAIN_HANDLED

#include <exception>

#include "Core/Application.hpp"
#include "Core/Debug/Instrumentor.hpp"
#include "Core/Log.hpp"

int main() {
  //try {

    {
      App::Application app{"OpenRPGMaker"};
      app.run();
    }
  // } catch (std::exception& e) {
  //   APP_ERROR("Main process terminated with: {}", e.what());
  // }

  return 0;
}
