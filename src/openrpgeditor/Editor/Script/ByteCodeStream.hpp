#pragma once

#include <angelscript.h>
#include <athena/FileReader.hpp>
#include <athena/FileWriter.hpp>
#include <athena/MemoryReader.hpp>
#include <athena/MemoryWriter.hpp>

class ByteCodeWriter final : public athena::io::FileWriter, public asIBinaryStream {
public:
  explicit ByteCodeWriter(const std::string& filepath);
  ~ByteCodeWriter() override;
  int Read(void* ptr, asUINT size) override;
  int Write(const void* ptr, asUINT size) override;

private:
  athena::io::MemoryCopyWriter m_writer;
};

class ByteCodeReader final : public athena::io::FileReader, public asIBinaryStream {
public:
  explicit ByteCodeReader(const std::string& filepath);
  int Read(void* ptr, asUINT size) override;
  int Write(const void* ptr, asUINT size) override;

private:
  athena::io::MemoryReader m_reader;
};
