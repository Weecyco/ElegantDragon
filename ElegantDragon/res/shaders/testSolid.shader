#tag Vertex
#version 330 core

layout(location = 0) in vec4 position;// we're using vec 4 bc gl_position needs to be a vec and we need to convert it later if we do vec 2. (doesn't matter if we have vecc 4)

void main()
{
   gl_Position = position;
};

#tag Fragment
#version 330 core

layout(location = 0) out vec4 color; //outputs colour data to location 0

uniform vec4 u_Color; //uniforms are usually labelled "u_<name>"

void main()
{
    color = u_Color;
};