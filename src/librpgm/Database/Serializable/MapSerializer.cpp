#include "Database/Serializable/MapSerializer.hpp"

#include <iostream>

void MapSerializer::serialize(std::ofstream& os) const {
  try {
    if (os.is_open()) {
      nlohmann::json data = m_map;
      os << data.dump(4);
    }
  } catch(...) {
    std::cerr << "Error while writing to file" << std::endl;
  }
}

void MapSerializer::deserialize(std::ifstream& is) {
  try {
    if (is.is_open()) {
      m_map = nlohmann::json::parse(is).get<Map>();
      m_map.m_isValid = true;
    }
  } catch(...) {
    std::cerr << "Error while reading from file" << std::endl;
  }
}
