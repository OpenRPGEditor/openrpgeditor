#pragma once

#include "Editor/IUndoCommand.hpp"

#include <memory>
#include <stack>

struct UndoStack {

  void push(const std::weak_ptr<IUndoCommand>& undo) { m_undoStack.push(undo.lock()); }

  std::shared_ptr<IUndoCommand> pop() {
    auto ret = m_undoStack.top();
    m_undoStack.pop();
    return ret;
  }

  bool hasCommands() const { return !m_undoStack.empty(); }

  void clear() {
    while (!m_undoStack.empty()) {
      m_undoStack.pop();
    }
  }

  std::shared_ptr<IUndoCommand> top() { return m_undoStack.top(); }

private:
  std::stack<std::shared_ptr<IUndoCommand>> m_undoStack;
};
