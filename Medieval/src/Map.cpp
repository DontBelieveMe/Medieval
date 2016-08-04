#include "Map.h"


#define BLOCK(token, name, color, solid) color
#define INVISIBLE 0,
#define COLOR(r,g,b) 1,
const bool *Block::visible_table = new bool[type_count]{BLOCK_LIST};
#undef INVISIBLE
#undef COLOR
#define INVISIBLE {0,0,0},
#define COLOR(r,g,b) {r/255.f, g/255.f, b/255.f},
const vec3 *Block::color_table = new vec3[type_count]{BLOCK_LIST};
#undef INVISIBLE
#undef COLOR
#undef BLOCK

#define BLOCK(token, name, color, solid) color
#define INVISIBLE 0,
#define COLOR(r,g,b) 1,
const bool *Block::solid_table = new bool[type_count]{BLOCK_LIST};
#undef INVISIBLE
#undef COLOR
#undef BLOCK

ShaderProgram &Chunk::Shader()
{
    static bool first = 1;
    static ShaderProgram *ret;
    if (first)
    {
        first = 0;
        ret = new ShaderProgram("res/shaders/vertChunk.shader", "res/shaders/fragChunk.shader");
    }
    return *ret;
}