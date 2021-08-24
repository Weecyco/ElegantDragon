#tag Vertex
#version 330 core

layout(location = 0) in vec4 position;// we're using vec 4 bc gl_position needs to be a vec and we need to convert it later if we do vec 2. (doesn't matter if we have vecc 4)

uniform mat4 MVP; //Model View Projection matrix

void main()
{
    vec4 temp = position;
    temp[3] = 1;
    gl_Position = MVP * temp;
    //gl_Position = position;
};

#tag Fragment
#version 330 core

layout(location = 0) out vec4 color; //outputs colour data to location 0

uniform vec4 u_Color; //uniforms are usually labelled "u_<name>"

//the texture slot what is the sampler2D type?
//sampler literally just means the index of the slot (image texture unit) in this case

void main()
{
    //returns colour for texture slot 0 and coordinates texCoords
    color = u_Color;
};