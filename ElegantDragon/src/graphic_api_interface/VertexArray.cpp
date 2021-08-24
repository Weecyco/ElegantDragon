#include "GL/glew.h"
#include "VertexArray.h"


VertexArray::VertexArray()
{
    GL_SAFE(glGenVertexArrays(1, &m_renderVAID));
}

VertexArray::VertexArray(const VertexBuffer& VB, const VertexBufferLayout& Layout)
{
    GL_SAFE(glGenVertexArrays(1, &m_renderVAID));
    addBuffer(VB, Layout);
}

VertexArray::~VertexArray()
{
    GL_SAFE(glDeleteVertexArrays(1, &m_renderVAID));
}

void VertexArray::addBuffer(const VertexBuffer& VB, const VertexBufferLayout& Layout)
{
    bind();
    VB.bind();
    const auto& elements = Layout.getElements();
    unsigned int offset = 0;
    for (unsigned int elemID = 0; elemID < elements.size(); elemID++)
    {
        VertexBufferElement element = elements[elemID];
        GL_SAFE(glEnableVertexAttribArray(elemID));
        GL_SAFE(glVertexAttribPointer(elemID, element.count, element.type, 
            element.normalized, Layout.getStride(), (const void*)offset));
        
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
    
    lastBuffer = &VB;
    lastLayout = &Layout;
}

void VertexArray::bind() const
{
    GL_SAFE(glBindVertexArray(m_renderVAID));
}

void VertexArray::unbind()
{
    GL_SAFE(glBindVertexArray(0));
}
