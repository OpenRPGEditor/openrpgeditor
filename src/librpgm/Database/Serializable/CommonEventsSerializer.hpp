#pragma once

#include "Database/Serializable/ISerializable.hpp"
#include "Database/CommonEvents.hpp"

class CommonEventsSerializer final : public ITypedSerializable<CommonEvents> {
public:
  explicit CommonEventsSerializer(const std::string_view filepath) : ITypedSerializable(filepath) {}
  CommonEventsSerializer(const CommonEvents& commonEvents, const std::string_view filepath)
  : ITypedSerializable(commonEvents, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};
