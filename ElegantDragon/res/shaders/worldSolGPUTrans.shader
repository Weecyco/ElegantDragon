#tag Vertex
#version 330 core

layout(location = 0) in vec4 position;// we're using vec 4 bc gl_position needs to be a vec and we need to convert it later if we do vec 2. (doesn't matter if we have vecc 4)

uniform vec2 u_objLocation; //corrects for object location
uniform vec2 u_camLocation; //corrects for camera location
uniform float u_camScale; // corrects for camera zoom
uniform vec2 u_screenScale; //corrects for the screen width by zooming out on certain axises

void main()
{
    //x=(camLoc[0] + src[drawIdx]) * camScaling * xScale
    //y=(camLoc[1] + src[drawIdx + 1]) * camScaling * yScale;

    gl_Position = vec4((u_objLocation[0] - u_camLocation[0] + position[0]) * u_camScale * u_screenScale[0], (u_objLocation[1] - u_camLocation[1] + position[1]) * u_camScale * u_screenScale[1], 0.0, 1.0);
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