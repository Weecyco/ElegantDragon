#include "Textures.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path, unsigned int slotIn/* = 0 */) :
    renderID(0), filePath(path), localBuffer(nullptr), width(0), height(0), BPP(0)
{

    stbi_set_flip_vertically_on_load(1);
    localBuffer = stbi_load(path.c_str(), &width, &height, &BPP, 4); //Channels = 4 = RGBA colour
    PROJ_ASSERT_W_MSG(localBuffer, "Failed to load via stbi: " + path);

    //just creates the textures
    GL_SAFE(glGenTextures(1, &renderID));
    //GL_SAFE(glActiveTexture(GL_TEXTURE0 + slot)); //set slot
    GL_SAFE(glBindTexture(GL_TEXTURE_2D, renderID));

    //Minification / Magnification configs (we just scale linearly (GL_LINEAR))
    GL_SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); //GL_NEAREST Will snap to nearest pixel
    GL_SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    //Wrap / Clamp configs
    GL_SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); //Clamp == don't extend the area options: TO_EDGE, TO_BORDER
    GL_SAFE(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    //IMPORTANT: The 4 parameters above must be set for things to work!!! (may get a black screeen)

    //Loads texture to the bound texture (See documentation bc it's really complicated...)
    GL_SAFE(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer)); //"GL_RGBA8 may be replaced by GL_RGBA but that notation is old and going away in newer versions
    GL_SAFE(glBindTexture(GL_TEXTURE_2D, 0));
    
    if (localBuffer)
        stbi_image_free(localBuffer); //basically just free since it's dynamically alocated

    slot = slotIn;
    bind();
}

Texture::~Texture()
{
    GL_SAFE(glDeleteTextures(1, &renderID)); // deletes on GPU side
}

void Texture::bind() const
{
    //no check since OGL will probably catch it?
    GL_SAFE(glActiveTexture(GL_TEXTURE0 + slot)); //up to 32 but may be less depending on the system 
    //(all consecutive in definition so we can add)
    //Binds to whatever the last texture slot assigned was
    GL_SAFE(glBindTexture(GL_TEXTURE_2D, renderID));
}

void Texture::unbind()
{
    GL_SAFE(glBindTexture(GL_TEXTURE_2D, 0));
}
