#include "Map.h"

#include "AssetData.h"

#define BLOCK(token, name, color, solid) color
#define INVISIBLE 0,
#define COLOR(...) 1,
const bool *Block::visible_table = new bool[type_count]{BLOCK_LIST};
#undef INVISIBLE
#undef COLOR
#define INVISIBLE {},
#define C(r,g,b) vec3(r/255.f, g/255.f, b/255.f)
#define COLOR(...) {__VA_ARGS__},
const std::vector<vec3> *Block::color_table = new std::vector<vec3>[type_count]{BLOCK_LIST};
#undef INVISIBLE
#undef C
#undef COLOR
#undef BLOCK

#define BLOCK(token, name, color, solid) color
#define INVISIBLE 0,
#define COLOR(...) 1,
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
		ret = new ShaderProgram(SHADER_PATH("vertChunk", "fragChunk"));
    }
    return *ret;
}