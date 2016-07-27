#pragma once
#include "../includes.h"
#include "../rendering/Renderer2D.h"

#define NO_OF_CHARACTERS 256

class Font
{
private:
    Renderer2D*   renderer;
    ShaderProgram* shaderRef;
    bool bound = false;

public:

    Font(ShaderProgram* shader, const std::string & fontImgPath);

    inline void bind() { renderer->bind(); bound = true; }

    void drawString(const std::string & msg, int xOff, int yOff, float scale);

    inline void halt() { renderer->halt(); bound = false; }

    ~Font();

};

