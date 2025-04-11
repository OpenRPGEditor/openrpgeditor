#include "Database/Serializable/MapSerializer.hpp"

#include <iostream>

void MapSerializer::serialize(std::ofstream& os) const {
  try {
    if (os.is_open()) {
      const nlohmann::ordered_json data = m_data;
      os << data.dump(4);
    }
  } catch (const std::exception& e) { std::cerr << "Error while writing to file \"" << filepath() << "\" " << e.what() << std::endl; }
}

void MapSerializer::deserialize(std::ifstream& is) {
  try {
    if (is.is_open()) {
      const auto data = nlohmann::ordered_json::parse(is);
      data.get_to(m_data);
      m_data.m_isValid = true;
    }
  } catch (const std::exception& e) { std::cerr << "Error while reading from file \"" << filepath() << "\"" << e.what() << std::endl; }
}
