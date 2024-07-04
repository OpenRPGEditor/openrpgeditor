#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyMPCommand : IEventCommand {
  ~ChangeEnemyMPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_MP; }
  int enemy;
  QuantityChangeOp enemyOp;
  QuantityChangeSource quantitySource;
  int quantity;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
