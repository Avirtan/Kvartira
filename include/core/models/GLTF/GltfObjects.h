#pragma once

#include <cstdint>
#include <iostream>
#include <rapidjson/document.h>
#include <string>

namespace GltfObject {
struct Buffer {
  public:
    Buffer() : m_length(0), m_buffer(nullptr) {}

    ~Buffer() {
        std::cout << "del" << std::endl;
        if (m_buffer != nullptr) {
            delete m_buffer;
        }
    }

    void SetLength(uint len) { m_length = len; }

    void SetBuffer(std::string &buffer) {
        char *c = new char[buffer.length() + 1];
        m_length = buffer.length();
        std::copy(buffer.c_str(), buffer.c_str() + buffer.length(), c);
        m_buffer = (int8_t *)(c);
    }

    template <typename Type> Type *Values(int32_t offset) {
        auto offsetBuffer = m_buffer + offset;
        auto values = (Type *)offsetBuffer;
        return values;
    }

    void Test() { std::cout << "test " << m_buffer << std::endl; }

  private:
    std::uint64_t m_length;
    std::int8_t *m_buffer;
};

struct BufferView {
  public:
    BufferView() {}
    ~BufferView() {}

  private:
    std::uint64_t m_BufferIndex;
    std::uint64_t m_Offset;
    std::uint64_t m_ByteLength;
    std::uint64_t m_ByteStride;
    std::uint64_t m_Target;
    std::string m_Name;
};
} // namespace GltfObject