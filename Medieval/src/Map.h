#pragma once

#include <unordered_map>

#include <physics/Physics.h>
#include <Utils.h>
#include <rendering/Shader.h>

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
    friend class Map;

  public:
    static constexpr int width = 16, depth = 64;
    Block data[depth][width][width] = {};
  private:
    bool dirty_mesh = 0;
  public:

    void set(ivec3 pos, Block block)
    {
        if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= width || pos.z >= width || pos.y >= depth)
            return; // Out of range.
        dirty_mesh = 1;
        data[pos.y][pos.x][pos.z] = block;
    }
    const Block get(ivec3 pos) const
    {
        if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= width || pos.z >= width || pos.y >= depth)
            return Block{}; // Out of range.
        return data[pos.y][pos.x][pos.z];
    }

    static ShaderProgram &Shader();

    void debugGenerate()
    {
        for (int y = 0; y < depth; y++)
        {
            for (int x = 0; x < width; x++)
            {
                for (int z = 0; z < width; z++)
                {
                    set({x,y,z}, Block{Block::Type(int(Random() % depth) > y*2)});
                }
            }
        }
    }

    void debugRender(const glm::mat4 &view) const
    {
        for (int y = 0; y < depth; y++)
        {
            for (int x = 0; x < width; x++)
            {
                for (int z = 0; z < width; z++)
                {
                    if (get({x,y,z}).type != Block::Type::air)
                        AABB(vec3(x,y,z) * float(units_per_voxel), {1,1,1}).debugDraw(view, Colors::light_grey);
                }
            }
        }
    }
};

class Map
{
  public:
    std::unordered_map<ivec2, Chunk> chunks;

    void addChunk(ivec2 pos)
    {
        if (chunks.find(pos) != chunks.end())
            return; // Already exists.
        chunks.insert({pos, Chunk{}});
    }

    void removeChunk(ivec2 pos)
    {
        auto it = chunks.find(pos);
        if (it == chunks.end())
            return; // No such chunk.
        chunks.erase(it);
    }

    void setBlock(ivec3 pos, Block block)
    {
        if (pos.y < 0 || pos.y >= Chunk::depth)
            return;
        ivec2 chunk_pos(proper_div(pos.x, Chunk::width),
                        proper_div(pos.z, Chunk::width));
        auto it = chunks.find(chunk_pos);
        if (it == chunks.end())
            return;
        ivec3 block_pos(proper_mod(pos.x, Chunk::width),
                        pos.y, // Note the lack of proper_mod().
                        proper_mod(pos.z, Chunk::width));
        it->second.set(block_pos, block);
    }
    const Block getBlock(ivec3 pos) const
    {
        if (pos.y < 0 || pos.y >= Chunk::depth)
            return Block{};
        ivec2 chunk_pos(proper_div(pos.x, Chunk::width),
                        proper_div(pos.z, Chunk::width));
        auto it = chunks.find(chunk_pos);
        if (it == chunks.end())
            return Block{};
        ivec3 block_pos(proper_mod(pos.x, Chunk::width),
                        pos.y, // Note the lack of proper_mod().
                        proper_mod(pos.z, Chunk::width));
        return it->second.get(block_pos);
    }
};