#pragma once
#include <cstdint>

class VertexBuffer
{
public:
    VertexBuffer(const void* vertices, size_t size);
    VertexBuffer(size_t size);
    VertexBuffer(const VertexBuffer&) = delete;
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
    void setData(const void* data, size_t size) const;
private:
    uint32_t m_id;
};

class IndexBuffer
{
public:
    IndexBuffer(uint32_t* indices, uint32_t count);
    IndexBuffer(const IndexBuffer&) = delete;
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    uint32_t getCount() const { return m_count; }
private:
    uint32_t m_id;
    uint32_t m_count;
};
