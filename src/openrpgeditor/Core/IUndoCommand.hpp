#pragma once

#include <string>

struct IUndoCommand {
  virtual int type() const = 0;
  virtual bool undo() = 0;
  virtual std::string description() = 0;
  void setIsRedo(bool isRedoOp) { m_isRedo = isRedoOp; }

protected:
  bool m_isRedo = false;
};
