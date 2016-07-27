#include "Font.h"

Font::Font(ShaderProgram* shader, const std::string & fontImgPath)
 : shaderRef(shader)
{
    renderer = new Renderer2D(shader, fontImgPath, glm::ivec2(16, 16));
}

void Font::drawString(const std::string & msg, int xOff, int yOff, float scale)
{
    if (!bound) 
    {
        std::cerr << "Font isn't bound" << endl;
        exit(1);
    }

    float x = 0;
    for (unsigned int i = 0; i < msg.size(); i++)
    {
        unsigned char c = (unsigned char) msg.at(i);
        int tx = c % 16;
        int ty = c / 16;
        renderer->drawTile(tx, ty, 1, 1, ((int)((12.0f * scale) * x)) + xOff, (int)(21.0f * scale) + yOff, (int)(12.0f * scale), (int)(21.0f * scale));
        x += 1.0;
        if (c == ' ') x -= 0.5f;
    }

}

Font::~Font()
{
    delete renderer;
}
