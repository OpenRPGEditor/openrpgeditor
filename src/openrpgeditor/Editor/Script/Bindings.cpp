#include "Editor/Script/Bindings.hpp"

#include "Editor/MainWindow.hpp"
#include "Editor/Script/ScriptEngine.hpp"

#include "Editor/Script/ImGui/ImGuiCoreOperations.hpp"
#include "ImGui/ImGuiMath.hpp"
void ImGuiBindings();

void RegisterBindings() {
  auto engine = ScriptEngine::instance()->engine();
  MainWindow::RegisterBindings();
  ImGuiMathBindings(engine);
  ImGuiCoreOperationBindings(engine);
}
