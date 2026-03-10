#include "Editor/Script/Bindings.hpp"

#include "Editor/MainWindow.hpp"
#include "Editor/Script/ScriptEngine.hpp"

#include "Editor/Script/ImGui/ImGuiCoreOperations.hpp"
#include "Editor/Script/DatabaseEditorBinding.hpp"
#include "ImGui/ImGuiMath.hpp"
void ImGuiBindings();

void RegisterBindings() {
  const auto engine = ScriptEngine::instance().engine();
  MainWindow::RegisterBindings();
  ImGuiMathBindings(engine);
  ImGuiCoreOperationBindings(engine);
  RegisterDatabaseEditor(engine);
}
