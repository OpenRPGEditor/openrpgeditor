#include "System.hpp"

#include <fstream>

System System::load(std::string_view filepath) {
  std::ifstream file(filepath.data());
  nlohmann::json data = nlohmann::json::parse(file);
  System ret = data.get<System>();
  data["hasEncryptedImages"].get_to(ret.hasEncryptedImages);
  data["hasEncryptedAudio"].get_to(ret.hasEncryptedAudio);
  data["encryptionKey"].get_to(ret.encryptionKey);
  return ret;
}

bool System::serialize(std::string_view filepath) {
  std::ofstream file(filepath.data());
  nlohmann::json data = *this;
  if (hasEncryptedImages || hasEncryptedAudio || encryptionKey) {
    data["hasEncryptedImages"] = *hasEncryptedImages;
    data["hasEncryptedAudio"] = *hasEncryptedAudio;
    data["encryptionKey"] = *encryptionKey;
  }
  file << data;
  return true;
}
