#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEnemyMPCommand final : IEventCommand {
  ChangeEnemyMPCommand() = default;
  explicit ChangeEnemyMPCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeEnemyMPCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Enemy_MP; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int enemy;
  QuantityChangeOp enemyOp;
  QuantityChangeSource quantitySource;
  int quantity;
};
