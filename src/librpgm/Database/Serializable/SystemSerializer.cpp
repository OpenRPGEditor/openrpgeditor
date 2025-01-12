#include "SystemSerializer.hpp"

void SystemSerializer::serialize(std::ofstream& os) const {
  // try {
  nlohmann::ordered_json data = m_data;
  if (m_data.m_encryptionKey && (m_data.m_hasEncryptedAudio || m_data.m_hasEncryptedImages)) {
    data["hasEncryptedImages"] = m_data.m_hasEncryptedImages ? *m_data.m_hasEncryptedImages : false;
    data["hasEncryptedAudio"] = m_data.m_hasEncryptedAudio ? *m_data.m_hasEncryptedAudio : false;
    data["encryptionKey"] = *m_data.m_encryptionKey;
  }
  os << data.dump(4);
  // } catch (...) {}
}

void SystemSerializer::deserialize(std::ifstream& is) {
  try {
    nlohmann::ordered_json data = nlohmann::ordered_json::parse(is);
    m_data = data.get<System>();
    data["hasEncryptedImages"].get_to(m_data.m_hasEncryptedImages);
    data["hasEncryptedAudio"].get_to(m_data.m_hasEncryptedAudio);
    data["encryptionKey"].get_to(m_data.m_encryptionKey);
  } catch (...) {}
}
