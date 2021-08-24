#include "Renderer.h"

void Renderer::clear() const
{
    //TODO: check if depth is enabled so we aren't clearing it for nothing!
    // current solution shouldn't cost much extra tho (I think)
    //clear color and depth data before new frame 
    GL_SAFE(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const unsigned int& count) const
{
    GL_SAFE(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw(const VertexArray& VA, const IndexBuffer& IB, const Shader& Sh) const
{
    VA.bind();
    IB.bind();
    Sh.bind();
    //may need to change the render type and index type (in this case GL_TRIANGLES and GL_UNSIGNED_INT) 
    //if we want to let it vary.
    GL_SAFE(glDrawElements(GL_TRIANGLES, IB.getCount(), GL_UNSIGNED_INT, nullptr)); //using nullptr will use whatever is bound in "GL_ELEMENT_ARRAY_BUFFER"
    //no need to unbind unless doing some special debug stuff (maybe add if we want to differenciate debug/release builds)
}

void Renderer::draw(const VertexArray& VA, const IndexBuffer& IB) const
{
    VA.bind();
    IB.bind();
    GL_SAFE(glDrawElements(GL_TRIANGLES, IB.getCount(), GL_UNSIGNED_INT, nullptr)); //using nullptr will use whatever is bound in "GL_ELEMENT_ARRAY_BUFFER"
}

void Renderer::enableBlend()
{
    GL_SAFE(glEnable(GL_BLEND));
    GL_SAFE(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //defaults to src = GL_ONE and dest = GL_ZERO
    //GL_SAFE(glBlendEquation(GL_FUNC_ADD)) //defaults to mode = GL_FUNC_ADD
    
    //(colour includes alpha too?!)
    //Current Blend: final colour = src_alpha*src_colour + (1-src_alpha)*dest_colour
    //Default Blend: final colour = 1*src_colour + 0*dest_colour
}

void Renderer::enableDepth()
{
    //Enable depth test
    glEnable(GL_DEPTH_TEST);
    //Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
}
