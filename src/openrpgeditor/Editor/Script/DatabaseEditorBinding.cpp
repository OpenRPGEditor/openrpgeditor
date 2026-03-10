#include "Editor/Script/DatabaseEditorBinding.hpp"

#include "angelscript.h"
#include "Editor/DatabaseEditor.hpp"

static bool IsDatabaseEditorReady() { return DatabaseEditor::instance() && DatabaseEditor::instance()->isReady(); }

void RegisterDatabaseEditor(asIScriptEngine* engine) {
  int r = engine->RegisterGlobalFunction("bool IsDatabaseEditorReady()", asFUNCTION(IsDatabaseEditorReady), asCALL_CDECL);
  assert(r >= 0);
  // TODO: IEditorTab related API
}
