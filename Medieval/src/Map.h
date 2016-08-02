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
    Block (*data)[width][width];

  private:
    bool dirty_mesh = 0;
    GLuint vbo, vao;
    int triangles = 0;

    void Render(const glm::mat4 &view, vec3 pos) const
    {
        "Ffs, why this does not work?";
        /*
        Shader().Use();
        Shader().UploadMatrix4f("u_proj", perspective_matrix);
        Shader().UploadMatrix4f("u_view", view);
        Shader().UploadMatrix4f("u_model", glm::translate({}, pos));
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, triangles*3);*/
    }

  public:
    Chunk()
    {
        data = new Block[depth][width][width];

        glGenVertexArrays(1, &vao);
        if (!vao)
            Error("Can't create VAO.");

        glGenBuffers(1, &vbo);
        if (!vbo)
            Error("Can't create VBO.");

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof (float) * 9, (void *) (sizeof (float) * 0));
        glVertexAttribPointer(1, 3, GL_FLOAT, 0, sizeof (float) * 9, (void *) (sizeof (float) * 3));
        glVertexAttribPointer(2, 3, GL_FLOAT, 0, sizeof (float) * 9, (void *) (sizeof (float) * 6));
    }

    Chunk(const Chunk &) = delete;
    Chunk &operator=(const Chunk &) = delete;

    Chunk(Chunk &&o)
    {
        vao = o.vao;
        vbo = o.vbo;
        data = o.data;
        dirty_mesh = o.dirty_mesh;
        triangles = o.triangles;
        o.vao = 0;
        o.vbo = 0;
        o.data = 0;
    }
    Chunk &operator=(Chunk &&o)
    {
        this->~Chunk();
        new(this) Chunk((Chunk &&)o);
        return *this;
    }

    ~Chunk()
    {
        if (vao)
            glDeleteVertexArrays(1, &vao);
        if (vbo)
            glDeleteBuffers(1, &vbo);
        if (data)
            delete [] data;
    }

    void Set(ivec3 pos, Block block)
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

    void DebugGenerate()
    {
        for (int y = 0; y < depth; y++)
        {
            for (int x = 0; x < width; x++)
            {
                for (int z = 0; z < width; z++)
                {
                    Set({x,y,z}, Block{Block::Type(int(Random() % depth) > y*2)});
                }
            }
        }
    }

    void DebugRender(const glm::mat4 &view) const
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

    void AddChunk(ivec2 pos)
    {
        if (chunks.find(pos) != chunks.end())
            return; // Already exists.

        using T = decltype(chunks)::value_type;
        chunks.insert((T &&) T{pos, (Chunk &&) Chunk{}});
    }

    void RemoveChunk(ivec2 pos)
    {
        auto it = chunks.find(pos);
        if (it == chunks.end())
            return; // No such chunk.
        chunks.erase(it);
    }

    void UpdateChunkMesh(ivec2 pos)
    {
        auto it = chunks.find(pos);
        if (it == chunks.end())
            return; // No such chunk.
        std::vector<vec3> vec(3 * 3 * Chunk::width * Chunk::width * Chunk::depth);
        it->second.triangles = 0;
        for (int y = 0; y < Chunk::depth; y++)
        {
            for (int x = 0; x < Chunk::width; x++)
            {
                for (int z = 0; z < Chunk::width; z++)
                {
                    vec[it->second.triangles*9+0] = {x,y,z};
                    vec[it->second.triangles*9+1] = {1,0,0};
                    vec[it->second.triangles*9+2] = {1,0,0};
                    vec[it->second.triangles*9+3] = {x+1,y,z};
                    vec[it->second.triangles*9+4] = {1,0,0};
                    vec[it->second.triangles*9+5] = {0,1,0};
                    vec[it->second.triangles*9+6] = {x,y+1,z};
                    vec[it->second.triangles*9+7] = {1,0,0};
                    vec[it->second.triangles*9+8] = {0,0,1};

                    it->second.triangles++;
                    return;
                }
            }
        }
        glBindVertexArray(0); // Don't remove this.
        glBindBuffer(GL_ARRAY_BUFFER, it->second.vbo);
        glBufferData(GL_ARRAY_BUFFER, it->second.triangles * 3 * 3 * 3 * sizeof (float), &vec[0], GL_STATIC_DRAW);
    }

    void SetBlock(ivec3 pos, Block block)
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
        it->second.Set(block_pos, block);
    }

    const Block GetBlock(ivec3 pos) const
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

    void Render(const glm::mat4 &view, vec3 pos)
    {
        for (const auto &it : chunks)
        {
            it.second.Render(view, pos + vec3(it.first.x * Chunk::width, 0, it.first.y * Chunk::width));
        }
    }
};