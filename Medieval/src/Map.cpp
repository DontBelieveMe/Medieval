#include "Map.h"

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