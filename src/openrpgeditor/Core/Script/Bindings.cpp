#include "Core/Script/Bindings.hpp"

#include "Core/MainWindow.hpp"
#include "Core/Script/ScriptEngine.hpp"

#include "Core/Script/ImGui/ImGuiCoreOperations.hpp"
#include "ImGui/ImGuiMath.hpp"
void ImGuiBindings();

void RegisterBindings() {
  auto engine = ScriptEngine::instance()->engine();
  MainWindow::RegisterBindings();
  ImGuiMathBindings(engine);
  ImGuiCoreOperationBindings(engine);
}
