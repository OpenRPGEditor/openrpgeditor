#pragma once

#include <string>

class IUndoOwner;
struct IUndoCommand {
  friend class UndoStack;
  virtual ~IUndoCommand() = default;
  virtual constexpr std::string_view type() const = 0;
  virtual bool undo() = 0;
  virtual std::string description() = 0;
  void setIsRedo(const bool isRedoOp) { m_isRedo = isRedoOp; }

protected:
  IUndoOwner* m_owner;
  bool m_isRedo = false;
};
