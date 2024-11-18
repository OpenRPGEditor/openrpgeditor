#include "Database/Serializable/MapSerializer.hpp"

#include <iostream>

void MapSerializer::serialize(std::ofstream& os) const {
  try {
    if (os.is_open()) {
      nlohmann::json data = m_data;
      os << data.dump(4);
    }
  } catch (const std::exception& e) {
    std::cerr << "Error while writing to file \"" << filepath() << "\" " << e.what() << std::endl;
  }
}

void MapSerializer::deserialize(std::ifstream& is) {
  try {
    if (is.is_open()) {
      m_data = nlohmann::json::parse(is).get<Map>();
      m_data.m_isValid = true;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error while reading from file \"" << filepath() << "\"" << e.what() << std::endl;
  }
}
