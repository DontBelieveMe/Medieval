#pragma once

#include <physics/Physics.h>

struct Block
{
    enum class Type : uint8_t
    {
        air,
        stone,
    };

    Type type;
};

class Chunk
{
  public:
    static constexpr int width = 16, depth = 64;
    Block data[depth][width][width] = {};

    Block &At(ivec3 pos) {return data[pos.y][pos.x][pos.z];}
    const Block &At(ivec3 pos) const {return data[pos.y][pos.x][pos.z];}

    void debugGenerate()
    {
        for (int y = 0; y < depth; y++)
        {
            for (int x = 0; x < width; x++)
            {
                for (int z = 0; z < width; z++)
                {
                    At({x,y,z}) = Block{Block::Type(int(Random() % depth) > y*2)};
                }
            }
        }
    }

    void render(const glm::mat4 &view) const
    {
        for (int y = 0; y < depth; y++)
        {
            for (int x = 0; x < width; x++)
            {
                for (int z = 0; z < width; z++)
                {
                    if (bool(At({x,y,z}).type))
                        AABB(vec3(x,y,z) * float(units_per_voxel), {1,1,1}).debugDraw(view, Colors::light_grey);
                }
            }
        }
    }
};