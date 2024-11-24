#include "SystemSerializer.hpp"

void SystemSerializer::serialize(std::ofstream& os) const {
  // try {
  nlohmann::ordered_json data = m_data;
  if (m_data.encryptionKey && (m_data.hasEncryptedAudio || m_data.hasEncryptedImages)) {
    data["hasEncryptedImages"] = m_data.hasEncryptedImages ? *m_data.hasEncryptedImages : false;
    data["hasEncryptedAudio"] = m_data.hasEncryptedAudio ? *m_data.hasEncryptedAudio : false;
    data["encryptionKey"] = *m_data.encryptionKey;
  }
  os << data.dump(4);
  // } catch (...) {}
}

void SystemSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
    m_data = data.get<System>();
    data["hasEncryptedImages"].get_to(m_data.hasEncryptedImages);
    data["hasEncryptedAudio"].get_to(m_data.hasEncryptedAudio);
    data["encryptionKey"].get_to(m_data.encryptionKey);
  } catch (...) {}
}
