#pragma once

#include <initializer_list>
#include <unordered_map>

#include <physics/Physics.h>
#include <Utils.h>
#include <rendering/Shader.h>

#define BLOCK_LIST \
    BLOCK(air   , "Air"   , INVISIBLE          , PASSABLE ) \
    BLOCK(stone , "Stone" , COLOR(132,139,114) , SOLID    ) \

class Block
{
    static const bool *visible_table;
    static const vec3 *color_table;
    static const bool *solid_table;

  public:
    #define BLOCK(token, name, color, solid) token,
    enum class Type : uint8_t {BLOCK_LIST};
    #undef BLOCK

    #define BLOCK(token, name, color, solid) 0,
    static constexpr int type_count = std::initializer_list<int>({BLOCK_LIST}).size();
    #undef BLOCK

    Type type;

    bool Visible() const {return visible_table[(int)type];}

    const vec3 &Color() const {return color_table[(int)type];}

    bool Solid() const {return solid_table[(int)type];}
};

class Chunk
{
    friend class Map;

  public:
    static constexpr int width = 16, depth = 64;

  private:
    //       y      x      z
    Block (*data)[width][width];
    bool dirty_mesh = 0;
    GLuint vbo, vao;
    int triangles = 0;

    void Render(const glm::mat4 &view, vec3 pos) const
    {
        Shader().Use();
        Shader().UploadMatrix4f("u_proj", perspective_matrix);
        Shader().UploadMatrix4f("u_view", view);
        Shader().UploadMatrix4f("u_model", glm::translate({}, pos));
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, triangles*3);
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
        new (this) Chunk((Chunk &&)o);
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
                    Set({x,y,z}, Block{Block::Type(int(Random() % depth + 20) > y + 10)});
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

  private:
    void UpdateChunkMesh(ivec2 chunk_pos)
    {
        auto it = chunks.find(chunk_pos);
        if (it == chunks.end())
            return; // No such chunk.

        struct Vertex
        {
            vec3 pos, normal, color;
        };

        static constexpr int vertices_per_buffer = 4096 / sizeof (Vertex);

        static constexpr float shadow_factor = 0.8f;

        class VertexArray
        {
            Vertex *arr;
          public:
            operator       Vertex *()       {return arr;}
            operator const Vertex *() const {return arr;}
            VertexArray()
            {
                arr = new Vertex[vertices_per_buffer];
            }
            VertexArray(VertexArray &&o)
            {
                arr = o.arr;
                o.arr = 0;
            }
            VertexArray &operator=(VertexArray &&o)
            {
                this->~VertexArray();
                new (this) VertexArray((VertexArray &&) o);
                return *this;
            }
            ~VertexArray()
            {
                if (arr)
                    delete [] arr;
            }
        };

        int vertices = 0;

        std::vector<VertexArray> buffer;

        auto PushVertex = [&](const Vertex &vertex)
        {
            if (vertices % vertices_per_buffer == 0)
                buffer.push_back({});
            buffer[vertices/vertices_per_buffer][vertices%vertices_per_buffer] = vertex;

            vertices++;
        };

        auto PushQuad = [&](const Vertex &a, const Vertex &b, const Vertex &c, const Vertex &d)
        {
            PushVertex(a);
            PushVertex(b);
            PushVertex(d);
            PushVertex(b);
            PushVertex(c);
            PushVertex(d);
        };
        enum Dir {x,y,z,_x,_y,_z};

        static const ivec3 dir11[]{{1,0,0},
                                   {0,1,0},
                                   {0,0,1},
                                   {-1,0,0},
                                   {0,-1,0},
                                   {0,0,-1}};

        static const ivec3 dir01[]{{1,0,0},
                                   {0,1,0},
                                   {0,0,1},
                                   {0,0,0},
                                   {0,0,0},
                                   {0,0,0}};

        ivec3 chunk_offset(chunk_pos.x * Chunk::width, 0, chunk_pos.y * Chunk::width);

        auto GenSide = [&](Block this_block, ivec3 pos, Dir up, Dir a, Dir b, Dir c, Dir d)
        {
            if (GetBlock(chunk_offset + pos + dir11[up]).type == Block::Type::air)
            {
                //
                //         a
                //
                //      3  0  0
                //  d   3  -  1   b
                //      2  2  1
                //
                //         c
                //

                vec3 color = this_block.Color();

                Vertex corners[4]{{pos+dir01[up]+dir01[a]+dir01[b], dir11[up], color},
                                  {pos+dir01[up]+dir01[b]+dir01[c], dir11[up], color},
                                  {pos+dir01[up]+dir01[c]+dir01[d], dir11[up], color},
                                  {pos+dir01[up]+dir01[d]+dir01[a], dir11[up], color}};

                Vertex edge_centers[4]{{vec3(pos+dir01[up]+dir01[d]+dir01[a]) - 0.5f*vec3(dir11[d]), dir11[up], color},
                                       {vec3(pos+dir01[up]+dir01[a]+dir01[b]) - 0.5f*vec3(dir11[a]), dir11[up], color},
                                       {vec3(pos+dir01[up]+dir01[b]+dir01[c]) - 0.5f*vec3(dir11[b]), dir11[up], color},
                                       {vec3(pos+dir01[up]+dir01[c]+dir01[d]) - 0.5f*vec3(dir11[c]), dir11[up], color}};

                Vertex center{vec3(pos+dir01[up]+dir01[a]+dir01[b]) - 0.5f*vec3(dir11[a]) - 0.5f*vec3(dir11[b]), dir11[up], this_block.Color()};

                /*
                Block near_blocks[4]{GetBlock(chunk_offset + pos + dir11[a]),
                                     GetBlock(chunk_offset + pos + dir11[b]),
                                     GetBlock(chunk_offset + pos + dir11[c]),
                                     GetBlock(chunk_offset + pos + dir11[d])};

                Block near_corners[4]{GetBlock(chunk_offset + pos + dir11[a]+dir11[b]),
                                      GetBlock(chunk_offset + pos + dir11[b]+dir11[c]),
                                      GetBlock(chunk_offset + pos + dir11[c]+dir11[d]),
                                      GetBlock(chunk_offset + pos + dir11[d]+dir11[a])};
                */

                Block near_blocks_up[4]{GetBlock(chunk_offset + pos + dir11[a] + dir11[up]),
                                        GetBlock(chunk_offset + pos + dir11[b] + dir11[up]),
                                        GetBlock(chunk_offset + pos + dir11[c] + dir11[up]),
                                        GetBlock(chunk_offset + pos + dir11[d] + dir11[up])};

                Block near_corners_up[4]{GetBlock(chunk_offset + pos + dir11[a]+dir11[b] + dir11[up]),
                                         GetBlock(chunk_offset + pos + dir11[b]+dir11[c] + dir11[up]),
                                         GetBlock(chunk_offset + pos + dir11[c]+dir11[d] + dir11[up]),
                                         GetBlock(chunk_offset + pos + dir11[d]+dir11[a] + dir11[up])};

                bool status[4]{}, edge_status[4]{};

                for (int dir = 0; dir < 4; dir++)
                {
                    if (near_blocks_up[dir].Solid())
                    {
                        edge_centers[dir].color = color * shadow_factor;
                        edge_status[dir] = 1;
                    }
                    if (near_blocks_up[dir].Solid() + near_blocks_up[(dir+1)%4].Solid() + near_corners_up[dir].Solid() >= 2)
                    {
                        corners[dir].color = color * shadow_factor;
                        status[dir] = 1;
                    }
                }

                bool any_shadows = 0;

                for (int dir = 0; dir < 4; dir++)
                {
                    if (edge_status[dir] && status[dir] && edge_status[(dir+1)%4])
                        corners[dir].color -= color - corners[dir].color;
                    if (edge_status[dir] || status[dir])
                        any_shadows = 1;
                }

                if (any_shadows)
                {
                    PushQuad(edge_centers[0], corners[0], edge_centers[1], center);
                    PushQuad(edge_centers[1], corners[1], edge_centers[2], center);
                    PushQuad(edge_centers[2], corners[2], edge_centers[3], center);
                    PushQuad(edge_centers[3], corners[3], edge_centers[0], center);
                }
                else
                {
                    PushQuad(corners[0], corners[1], corners[2], corners[3]);
                }
            }
        };

        for (int yy = 0; yy < Chunk::depth; yy++)
        {
            for (int xx = 0; xx < Chunk::width; xx++)
            {
                for (int zz = 0; zz < Chunk::width; zz++)
                {
                    Block this_block = GetBlock(chunk_offset+ivec3(xx,yy,zz));
                    if (this_block.Visible())
                    {
                        GenSide(this_block, ivec3(xx,yy,zz), Dir::x, Dir::y, Dir::z, Dir::_y, Dir::_z);
                        GenSide(this_block, ivec3(xx,yy,zz), Dir::_x, Dir::y, Dir::_z, Dir::_y, Dir::z);
                        GenSide(this_block, ivec3(xx,yy,zz), Dir::y, Dir::x, Dir::_z, Dir::_x, Dir::z);
                        GenSide(this_block, ivec3(xx,yy,zz), Dir::_y, Dir::x, Dir::z, Dir::_x, Dir::_z);
                        GenSide(this_block, ivec3(xx,yy,zz), Dir::z, Dir::x, Dir::y, Dir::_x, Dir::_y);
                        GenSide(this_block, ivec3(xx,yy,zz), Dir::_z, Dir::x, Dir::_y, Dir::_x, Dir::y);
                    }
                }
            }
        }

        if (vertices % 3)
            Error("Chunk mesh generator provided an amount of vertices which is not a multiply of 3.");

        it->second.triangles = vertices / 3;

        glBindBuffer(GL_ARRAY_BUFFER, it->second.vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices * sizeof (Vertex), 0, GL_DYNAMIC_DRAW);
        for (unsigned int i = 0; i < buffer.size(); i++)
        {
            if (i != buffer.size() - 1 && vertices % vertices_per_buffer)
                glBufferSubData(GL_ARRAY_BUFFER, vertices_per_buffer * sizeof (Vertex) * i, vertices_per_buffer * sizeof (Vertex), buffer[i]);
            else
                glBufferSubData(GL_ARRAY_BUFFER, vertices_per_buffer * sizeof (Vertex) * i, (vertices % vertices_per_buffer) * sizeof (Vertex), buffer[i]);
        }
    }

  public:
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
        for (auto &it : chunks)
        {
            if (it.second.dirty_mesh)
            {
                it.second.dirty_mesh = 0;
                UpdateChunkMesh(it.first);
            }
            it.second.Render(view, pos + vec3(it.first.x * Chunk::width, 0, it.first.y * Chunk::width));
        }
    }
};