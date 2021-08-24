#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
    GL_SAFE(glGenBuffers(1, &renderID));
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size, const unsigned short& drawType/* = GL_STATIC_DRAW*/)
{
    GL_SAFE(glGenBuffers(1, &renderID));
    GL_SAFE(glBindBuffer(GL_ARRAY_BUFFER, renderID));
    GL_SAFE(glBufferData(GL_ARRAY_BUFFER, size, data, drawType));
}

VertexBuffer::~VertexBuffer()
{
    GL_SAFE(glDeleteBuffers(1, &renderID));
}

void VertexBuffer::setData(const void* data, unsigned int size, const unsigned short& drawType/* = GL_STATIC_DRAW*/)
{
    GL_SAFE(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::setDataB(const void* data, unsigned int size, const unsigned short& drawType/* = GL_STATIC_DRAW*/)
{
    GL_SAFE(glBindBuffer(GL_ARRAY_BUFFER, renderID));
    GL_SAFE(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::bind() const
{
    GL_SAFE(glBindBuffer(GL_ARRAY_BUFFER, renderID));
}

void VertexBuffer::unbind()
{
    GL_SAFE(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
