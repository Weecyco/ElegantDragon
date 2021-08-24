#pragma once

#include <GL/glew.h>
#include <vector>
#include "projectDefines.h"


struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:          return(sizeof(GLfloat));
        case GL_UNSIGNED_INT:   return(sizeof(GLuint));
        case GL_UNSIGNED_BYTE:  return(sizeof(GLubyte));

        }
        PROJ_ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_stride;

public:
    VertexBufferLayout() : m_stride(0) {};// initiallizes m_stride (same way you would initialize using constructor of a base class)
    ~VertexBufferLayout() {};

    //this explicitly define template-specializations without namespacing
    //normally we would have to move the templates outside the class and 
    //specify the class namespace on the function (try later, maybe keep 
    //for compatibility?)
    template<typename T>
    void push(unsigned int count)
    {
        static_assert(false);
    }

    template<>
    void push<float> (unsigned int count)
    {
        m_Elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
        m_stride += count * sizeof(GLfloat);
    }

    template<>
    void push<unsigned int>(unsigned int count)
    {
        m_Elements.push_back(VertexBufferElement{ GL_UNSIGNED_INT, count, GL_FALSE });
        m_stride += count * sizeof(GLuint);
    }

    template<>
    void push<unsigned char>(unsigned int count)
    {
        m_Elements.push_back(VertexBufferElement{ GL_UNSIGNED_BYTE, count, GL_TRUE });
        m_stride += count * sizeof(GLubyte);
    }
    inline const std::vector<VertexBufferElement>& getElements() const { return m_Elements; }
    inline unsigned int getStride() const { return m_stride; }
};