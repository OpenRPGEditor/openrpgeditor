#include "Core/Script/ByteCodeStream.hpp"

#include <sha1.h>

#include <athena/Compression.hpp>

ByteCodeWriter::ByteCodeWriter(const std::string& filepath) : FileWriter(filepath) {}
ByteCodeWriter::~ByteCodeWriter() {
  writeUint32Big('ASC\0');
  writeUint32Little(m_writer.length());
  auto data = m_writer.data();

  // std::unique_ptr<atUint8[]> compData(new atUint8[m_writer.length() * 2]);
  // auto compLen = m_writer.length() * 2;
  // compLen = athena::io::Compression::compressZlib(data, m_writer.length(), compData.get(), compLen);
  // if (compLen > 0 && compLen < m_writer.length()) {
  //   const auto realLen = ROUND_UP_32(compLen);
  //   writeUint32Little(realLen);
  //   std::unique_ptr<atUint8[]> realData(new atUint8[realLen]);
  //   athena::utility::fillRandom(realData.get(), realLen);
  //   memcpy(realData.get(), compData.get(), compLen);
  //   std::unique_ptr<atUint8[]> sha1(getSha1(realData.get(), realLen));
  //   FileWriter::writeUBytes(sha1.get(), 20);
  //   FileWriter::writeUBytes(realData.get(), realLen);
  //   return;
  // }

  writeUint32Little(0);
  const auto realLen = ROUND_UP_32(m_writer.length());
  std::unique_ptr<atUint8[]> realData(new atUint8[realLen]);
  athena::utility::fillRandom(realData.get(), realLen);
  memcpy(realData.get(), data, m_writer.length());
  std::unique_ptr<atUint8[]> sha1(getSha1(realData.get(), realLen));
  FileWriter::writeUBytes(sha1.get(), 20);
  FileWriter::writeUBytes(realData.get(), realLen);
}

int ByteCodeWriter::Write(const void* ptr, const asUINT size) {
  if (!size)
    return asERROR;

  m_writer.writeUBytes(static_cast<const atUint8*>(ptr), size);
  return asSUCCESS;
}

int ByteCodeWriter::Read(void* ptr, asUINT size) { return asNOT_SUPPORTED; }

ByteCodeReader::ByteCodeReader(const std::string& filepath) : FileReader(filepath), m_reader(new atUint8[1], 1, true) {
  auto magic = readUint32Big();
  if (magic != 'ASC\0') {
    setError();
    return;
  }

  auto uncompLen = readUint32Little();
  auto compLen = readUint32Little();
  auto sha1Hash = readUBytes(20);
  atUint32 realLen = 0;
  if (compLen != 0) {
    realLen = ROUND_UP_32(compLen);
  } else {
    realLen = ROUND_UP_32(uncompLen);
  }

  atUint8* data = new atUint8[realLen];
  FileReader::readUBytesToBuf(data, realLen);
  std::unique_ptr<atUint8[]> calculatedHash(getSha1(data, realLen));

  if (memcmp(sha1Hash.get(), calculatedHash.get(), 20) != 0) {
    setError();
    return;
  }

  if (compLen != 0) {
    const auto byteCode(new atUint8[uncompLen]);
    if (auto outSize = athena::io::Compression::decompressZlib(data, compLen, byteCode, uncompLen); outSize != uncompLen) {
      setError();
      return;
    }

    m_reader.setData(byteCode, uncompLen, true);
    delete[] data;
  } else {
    m_reader.setData(data, uncompLen, true);
  }
}

int ByteCodeReader::Write(const void* ptr, asUINT size) { return asNOT_SUPPORTED; }

int ByteCodeReader::Read(void* ptr, const asUINT size) {
  if (!size || m_hasError) {
    return asERROR;
  }

  m_reader.readUBytesToBuf(ptr, size);
  return asSUCCESS;
}
