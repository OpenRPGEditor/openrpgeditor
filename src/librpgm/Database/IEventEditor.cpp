#include "Database/IEventEditor.hpp"
#include "Database/Event.hpp"

int IEventEditor::id() const { return m_event->id(); }