#pragma once
#include "Database/Items.hpp"
#include "Database/Serializable/ISerializable.hpp"

class ItemsSerializer final : public ITypedSerializable<Items> {
public:
  explicit ItemsSerializer(const std::string_view filepath) : ITypedSerializable(filepath) {}
  ItemsSerializer(const Items& items, const std::string_view filepath) : ITypedSerializable(items, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};
