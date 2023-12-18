#pragma once
#include "gl/glew.h"
#include "graphicUtilities.h"
#include <string>

class Texture
{
private:
    unsigned int renderID;
    std::string filePath;
    unsigned char* localBuffer;
    int width, height, BPP; //bits per pixel
    unsigned int slot;

public:
    Texture(const std::string& path, unsigned int slot = 0);
    ~Texture();

    void bind() const; //defaults to 0 (when was this notation added to cpp???)
    void unbind();

    inline int getWidth() const { return width; };
    inline int getHeight() const { return height; };
    inline unsigned int getSlot() const { return slot; };

};