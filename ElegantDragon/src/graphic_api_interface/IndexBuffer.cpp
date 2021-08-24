#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
    GL_SAFE(glGenBuffers(1, &renderID));
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int inCount, const unsigned short& drawType /*= GL_STATIC_DRAW*/)
{
    PROJ_ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    count = inCount;
    GL_SAFE(glGenBuffers(1, &renderID));
    GL_SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID));
    GL_SAFE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, drawType));
}

IndexBuffer::~IndexBuffer()
{
    GL_SAFE(glDeleteBuffers(1, &renderID));
}

void IndexBuffer::setData(const void* data, const unsigned int& count, const unsigned short& drawType /*= GL_STATIC_DRAW*/)
{
    PROJ_ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GL_SAFE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, drawType));
}

void IndexBuffer::setDataB(const void* data, const unsigned int& count, const unsigned short& drawType /*= GL_STATIC_DRAW*/)
{
    PROJ_ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GL_SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID));
    GL_SAFE(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, drawType));
}

void IndexBuffer::editData(const void* data, const unsigned int& offset, const unsigned int& count)
{
    PROJ_ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GL_SAFE(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(unsigned int), count * sizeof(unsigned int), data));
}

void IndexBuffer::editDataB(const void* data, const unsigned int& offset, const unsigned int& count)
{
    PROJ_ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GL_SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID));
    GL_SAFE(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(unsigned int), count * sizeof(unsigned int), data));
}

void IndexBuffer::bind() const
{
    GL_SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID));
}

void IndexBuffer::unbind()
{
    GL_SAFE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
