# ElegantDragon
A simple graphics engine.
<br>Currently developing as a personal computer graphics learning and exploration platform. 
### Goal: 
To develop Elegant Dragon into a personalized a graphics library for future projects.

### To Do:
 - Change the data structure behind the matrix transformation and move frequent/low variability operations to the GPU.
 - Crop screen space depending on camera settings
 - Create proper shaders to accomany camera changes and implement various shading techniques presets ie. blin-phong
 - Design the user input interface or add an existing one as part of the package
 - Change dependencies on git into submodules (if applicable)
 - Add Vulkan support

### Dependencies / Other people's code:
 - Code under *ElegantDragon/ElegantDragon/src/graphic_api_interface/* - Heavily based on Cherno's OpenGL tutorials: https://www.youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2 
 - GLFW under *ElegantDragon/ElegantDragon/depend/GLFW/*
 - GLEW under *ElegantDragon/ElegantDragon/depend/glew-2.1.0/*
 - ImGUI under *ElegantDragon/ElegantDragon/vendor/imgui/*
 - STBImage under *ElegantDragon/ElegantDragon/vendor/stb_image/*
