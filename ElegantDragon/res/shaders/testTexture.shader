#tag Vertex
#version 330 core

layout(location = 0) in vec4 position;// we're using vec 4 bc gl_position needs to be a vec and we need to convert it later if we do vec 2. (doesn't matter if we have vecc 4)
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord; //v is for varying (things that you pass from shader to shader that varies based on Vertex locations)

void main()
{
    gl_Position = position;
    v_texCoord = texCoord;
};

#tag Fragment
#version 330 core

in vec2 v_texCoord;
layout(location = 0) out vec4 color; //outputs color data to location 0

//uniform vec4 u_Color; //uniforms are usually labelled "u_<name>"
uniform sampler2D u_Texture; 
//the texture slot what is the sampler2D type?
//sampler literally just means the index of the slot (image texture unit) in this case

void main()
{
    //returns colour for texture slot 0 and coordinates texCoords
    vec4 texColor = texture(u_Texture, v_texCoord);
    color = texColor;
};