#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

//some people make this all static and basically objectless, 
//but if you want to add extra data or other contexts, it might be helpful to have it as an object.
class Renderer
{
public:
    void clear() const;
    void draw(const unsigned int& count) const;
    void draw(const VertexArray& VA, const IndexBuffer& IB, const Shader& Sh) const;
    void draw(const VertexArray& VA, const IndexBuffer& IB) const;
    void enableBlend();
    void enableDepth();
    void setBackground(float r, float g, float b, float a) {GL_SAFE(glClearColor(r, g, b, a));};
    void setBackground(float r, float g, float b) { GL_SAFE(glClearColor(r, g, b, 1.0)); };
    void setBackground() {GL_SAFE(glClearColor(0.0, 0.0, 0.0, 0.0));};
};