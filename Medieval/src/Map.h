#pragma once

#include <deque>
#include <unordered_map>
#include <vector>

#include "physics/Physics.h"
#include "Utils.h"
#include "rendering/Shader.h"
#include <zlib.h>
#include "MapUtils.h"

#define BLOCK_LIST \
    BLOCK(air     , "Air"   , INVISIBLE          , PASSABLE ) \
    BLOCK(grass_a , "Grass" , COLOR( 65,150, 88) , SOLID    ) \
    BLOCK(grass_b , "Grass" , COLOR( 58,129, 76) , SOLID    ) \
    BLOCK(grass_c , "Grass" , COLOR(106,144, 80) , SOLID    ) \
    BLOCK(grass_d , "Grass" , COLOR( 84,103, 51) , SOLID    ) \

class Block
{
    static const bool *visible_table;
    static const vec3 *color_table;
    static const bool *solid_table;

  public:
    #define BLOCK(token, name, color, solid) token,
    enum class Type : uint8_t {BLOCK_LIST};
    #undef BLOCK

	// A problem with MSVC - error cannot evaluate non constant type.
	// Done temporary fix, as I cannot be bothered to change the system.
	#define BLOCK(...) +1
	static constexpr int type_count = 0 + BLOCK_LIST;
    #undef BLOCK

    Type type;

    bool Visible() const {return visible_table[(int)type];}

    vec3 Color() const {return color_table[(int)type];}

    bool Solid() const {return solid_table[(int)type];}
};

class Chunk
{
    friend class Map;

  public:
    static constexpr int width = 24, depth = 96;

  private:
    //       y      x      z
    Block (*data)[width][width];
    bool dirty_mesh = 0, in_mesh_update_queue = 0;
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

    void Set(ivec3 pos, Block block)
    {
        if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= width || pos.z >= width || pos.y >= depth)
            return; // Out of range.
        data[pos.y][pos.x][pos.z] = block;
    }
    const Block get(ivec3 pos) const
    {
        if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x >= width || pos.z >= width || pos.y >= depth)
            return Block{}; // Out of range.
        return data[pos.y][pos.x][pos.z];
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

    void *Data() const
    {
        return data;
    }

    static constexpr std::size_t ByteSize()
    {
        return width * width * depth;
    }

    static ShaderProgram &Shader();
};



class Map
{
    int mesh_update_timer = 0;
    uint32_t seed = Noise32(Random());

    using MeshUpdateDequeType = std::deque<ivec2>;
    MeshUpdateDequeType mesh_gen_que, mesh_upd_que; // Separate update queues for fresh chunks and just modified chunks, because quickly giving fresh chunks is more important.

    using MapArchiveType = std::unordered_map<ivec2, std::vector<Bytef>>;
    MapArchiveType archive;

    ivec2 *PollChunkForMeshUpdate()
    {
        static ivec2 ret;
        if (mesh_gen_que.size())
        {
            ret = mesh_gen_que[0];
            mesh_gen_que.pop_front();
            return &ret;
        }
        if (mesh_upd_que.size())
        {
            ret = mesh_upd_que[0];
            mesh_upd_que.pop_front();
            return &ret;
        }
        return 0;
    }
  public:
    using ChunksMapType = std::unordered_map<ivec2, Chunk>;
    ChunksMapType chunks;

    static constexpr int load_distance = 4; // 0 means 1x1, 1 means 3x3, etc.
    static constexpr int unload_distance = 6;
    static constexpr int render_distance = 4;
    static constexpr int mesh_update_period = 2; // If multiple new meshes are needed, the map will generate new one each `mesh_update_period` ticks. Setting it to 0 will gen everything instantly, leading to freezes.

    ChunksMapType::iterator AddChunk(ivec2 pos)
    {
        auto it = chunks.find(pos);
        if (it != chunks.end())
            return it; // Already exists.

        using T = decltype(chunks)::value_type;
        return chunks.insert((T &&) T{pos, (Chunk &&) Chunk{}}).first;
    }

    ChunksMapType::iterator SaveChunk(ivec2 pos, bool del_src = 1) // If `del_src`, returns an iterator to the next valid chunk (like erase()).
    {
		printf("Doing saveing!");
        auto it = chunks.find(pos);
        if (it == chunks.end())
            return chunks.end();
        auto &dst = archive[pos];
        uLongf dst_buf_sz = compressBound(it->second.ByteSize());
        dst.resize(dst_buf_sz);
        if (compress(dst.data(), &dst_buf_sz, (const Bytef *)it->second.Data(), it->second.ByteSize()) != Z_OK)
            Error("Zlib is unable to compress a chunk.");
        dst.resize(dst_buf_sz); // Dupe intended.
        if (del_src)
            return chunks.erase(it);
        return chunks.end();
    }
    void LoadChunk(ivec2 pos, bool del_archived = 1)
    {
        auto it = archive.find(pos);
        if (it == archive.end())
            return;
        auto &dst = chunks[pos];
        uLongf dst_buf_sz = dst.ByteSize();
        if (uncompress((Bytef *)dst.Data(), &dst_buf_sz, it->second.data(), it->second.size()) != Z_OK)
            Error("Zlib is unable to decompress a chunk.");
        if (dst_buf_sz != dst.ByteSize())
            Error("Compressed chunk was corrupted.");
        if (del_archived)
            archive.erase(it);
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
		// This looks awesome with it to 1. Just saying.
        #define AMBIENT_OCCLUSION 0

        auto it = chunks.find(chunk_pos);
        if (it == chunks.end())
            return; // No such chunk.


        static constexpr int vertices_per_buffer = 4096 / sizeof (Vertex);

        #if AMBIENT_OCCLUSION == 1
        static constexpr float shadow_factor = 0.8f;
        #endif

        int vertices = 0;

        std::vector<VertexArray<vertices_per_buffer>> buffer;

       /* auto PushVertex = [&](const Vertex &vertex)
        {
            if (vertices % vertices_per_buffer == 0)
                buffer.push_back({});
            buffer[vertices/vertices_per_buffer][vertices%vertices_per_buffer] = vertex;

            vertices++;
        };*/

        /*auto PushQuad = [&](const Vertex &a, const Vertex &b, const Vertex &c, const Vertex &d)
        {
			MapFuncs::PushVertex<vertices_per_buffer>(a, buffer, vertices);
			MapFuncs::PushVertex<vertices_per_buffer>(b, buffer, vertices);
			MapFuncs::PushVertex<vertices_per_buffer>(d, buffer, vertices);
			MapFuncs::PushVertex<vertices_per_buffer>(b, buffer, vertices);
			MapFuncs::PushVertex<vertices_per_buffer>(c, buffer, vertices);
			MapFuncs::PushVertex<vertices_per_buffer>(d, buffer, vertices);
        };*/

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

		/*ChunksMapType::const_iterator chunk_iters_3x3[3 * 3]
		{
			chunks.find(chunk_pos + ivec2(-1,-1)), chunks.find(chunk_pos + ivec2(-1,0)), chunks.find(chunk_pos + ivec2(-1,1)),
		    chunks.find(chunk_pos + ivec2(0,-1)), it,                                   chunks.find(chunk_pos + ivec2(0,1)),
			chunks.find(chunk_pos + ivec2(1,-1)), chunks.find(chunk_pos + ivec2(1,0)), chunks.find(chunk_pos + ivec2(1,1))
		};*/
        ChunksMapType::const_iterator chunk_iters_3x3[3][3]
        {
            {chunks.find(chunk_pos + ivec2(-1,-1)), chunks.find(chunk_pos + ivec2(-1,0)), chunks.find(chunk_pos + ivec2(-1,1))},
            {chunks.find(chunk_pos + ivec2( 0,-1)), it,                                   chunks.find(chunk_pos + ivec2( 0,1))},
            {chunks.find(chunk_pos + ivec2( 1,-1)), chunks.find(chunk_pos + ivec2( 1,0)), chunks.find(chunk_pos + ivec2( 1,1))},
        };
		
        auto GetBlockFast = [&](ivec3 pos) -> Block
        {
            ivec2 chunk_index = GetChunkPosForBlock(pos) - chunk_pos + 1;
			//std::cout << chunk_index.x << std::endl;
			//return GetBlockI(chunk_iters_3x3[chunk_index.x + chunk_index.y * 3], pos);
			return GetBlockI(chunk_iters_3x3[chunk_index.x][chunk_index.y], pos);
        };


        auto GenSide = [&](Block this_block, ivec3 pos, Dir up, Dir a, Dir b, Dir c, Dir d)
        {
            if (GetBlockFast(chunk_offset + pos + dir11[up]).type == Block::Type::air)
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

                #if AMBIENT_OCCLUSION == 1
                Vertex edge_centers[4]{{vec3(pos+dir01[up]+dir01[d]+dir01[a]) - 0.5f*vec3(dir11[d]), dir11[up], color},
                                       {vec3(pos+dir01[up]+dir01[a]+dir01[b]) - 0.5f*vec3(dir11[a]), dir11[up], color},
                                       {vec3(pos+dir01[up]+dir01[b]+dir01[c]) - 0.5f*vec3(dir11[b]), dir11[up], color},
                                       {vec3(pos+dir01[up]+dir01[c]+dir01[d]) - 0.5f*vec3(dir11[c]), dir11[up], color}};

                Vertex center{vec3(pos+dir01[up]+dir01[a]+dir01[b]) - 0.5f*vec3(dir11[a]) - 0.5f*vec3(dir11[b]), dir11[up], color};

                /*
                Block near_blocks[4]{GetBlockFast(chunk_offset + pos + dir11[a]),
                                     GetBlockFast(chunk_offset + pos + dir11[b]),
                                     GetBlockFast(chunk_offset + pos + dir11[c]),
                                     GetBlockFast(chunk_offset + pos + dir11[d])};

                Block near_corners[4]{GetBlockFast(chunk_offset + pos + dir11[a]+dir11[b]),
                                      GetBlockFast(chunk_offset + pos + dir11[b]+dir11[c]),
                                      GetBlockFast(chunk_offset + pos + dir11[c]+dir11[d]),
                                      GetBlockFast(chunk_offset + pos + dir11[d]+dir11[a])};
                */

                Block near_blocks_up[4]{GetBlockFast(chunk_offset + pos + dir11[a] + dir11[up]),
                                        GetBlockFast(chunk_offset + pos + dir11[b] + dir11[up]),
                                        GetBlockFast(chunk_offset + pos + dir11[c] + dir11[up]),
                                        GetBlockFast(chunk_offset + pos + dir11[d] + dir11[up])};

                Block near_corners_up[4]{GetBlockFast(chunk_offset + pos + dir11[a]+dir11[b] + dir11[up]),
                                         GetBlockFast(chunk_offset + pos + dir11[b]+dir11[c] + dir11[up]),
                                         GetBlockFast(chunk_offset + pos + dir11[c]+dir11[d] + dir11[up]),
                                         GetBlockFast(chunk_offset + pos + dir11[d]+dir11[a] + dir11[up])};

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
                #endif
                MapFuncs::PushQuad<vertices_per_buffer>(corners[0], corners[1], corners[2], corners[3], buffer, vertices);
            }
            #undef AMBIENT_OCCLUSION
        };

        for (int yy = 0; yy < Chunk::depth; yy++)
        {
            for (int xx = 0; xx < Chunk::width; xx++)
            {
                for (int zz = 0; zz < Chunk::width; zz++)
                {
                    Block this_block = GetBlockFast(chunk_offset+ivec3(xx,yy,zz));
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
        for (unsigned int i = 0; i < buffer.size(); ++i)
        {
            if (i != buffer.size() - 1)
                glBufferSubData(GL_ARRAY_BUFFER, vertices_per_buffer * sizeof (Vertex) * i, vertices_per_buffer * sizeof (Vertex), buffer[i]);
            else
                glBufferSubData(GL_ARRAY_BUFFER, vertices_per_buffer * sizeof (Vertex) * i, ((vertices-1) % vertices_per_buffer + 1) * sizeof (Vertex), buffer[i]);
        }
    }

    void GenerateChunks(vec3 center);
  public:
    void SetBlock_NoMeshUpdateI(ChunksMapType::iterator it, ivec3 pos, Block block)
    {
        if (pos.y < 0 || pos.y >= Chunk::depth)
            return;
        if (it == chunks.end())
            return;
        ivec3 block_pos(proper_mod(pos.x, Chunk::width),
                        pos.y, // Note the lack of proper_mod().
                        proper_mod(pos.z, Chunk::width));
        it->second.Set(block_pos, block);
        it->second.dirty_mesh = 1;
    }
    void SetBlock_NoMeshUpdate(ivec3 pos, Block block)
    {
        SetBlock_NoMeshUpdateI(chunks.find({proper_div(pos.x, Chunk::width), proper_div(pos.z, Chunk::width)}), pos, block);
    }

    void SetBlockI(ChunksMapType::iterator it, ivec3 pos, Block block)
    {
        if (it == chunks.end())
            return;
        SetBlock_NoMeshUpdateI(it, pos, block);

        ChunksMapType::iterator tmp_it;

        ivec2 block_pos(proper_mod(pos.x, Chunk::width),
                        proper_mod(pos.z, Chunk::width));

        if (block_pos.x == 0)
        {
            tmp_it = chunks.find(it->first + ivec2(-1,0));
            if (tmp_it != chunks.end())
                tmp_it->second.dirty_mesh = 1;
        }
        else if (block_pos.x == Chunk::width-1)
        {
            tmp_it = chunks.find(it->first + ivec2(1,0));
            if (tmp_it != chunks.end())
                tmp_it->second.dirty_mesh = 1;
        }
        if (block_pos.x == 0)
        {
            tmp_it = chunks.find(it->first + ivec2(0,-1));
            if (tmp_it != chunks.end())
                tmp_it->second.dirty_mesh = 1;
        }
        else if (block_pos.x == Chunk::width-1)
        {
            tmp_it = chunks.find(it->first + ivec2(0,1));
            if (tmp_it != chunks.end())
                tmp_it->second.dirty_mesh = 1;
        }
    }
    void SetBlock(ivec3 pos, Block block)
    {
        SetBlockI(chunks.find({proper_div(pos.x, Chunk::width), proper_div(pos.z, Chunk::width)}), pos, block);
    }

    Block GetBlockI(ChunksMapType::const_iterator it, ivec3 pos) const
    {
        if (pos.y < 0 || pos.y >= Chunk::depth)
            return Block{};
        if (it == chunks.end())
            return Block{};
        ivec3 block_pos(proper_mod(pos.x, Chunk::width),
                        pos.y, // Note the lack of proper_mod().
                        proper_mod(pos.z, Chunk::width));
        return it->second.get(block_pos);
    }
    Block GetBlock(ivec3 pos) const
    {
        return GetBlockI(chunks.find({proper_div(pos.x, Chunk::width), proper_div(pos.z, Chunk::width)}), pos);
    }

    bool ChunkExists(ivec2 pos) const
    {
        return chunks.find(pos) != chunks.end();
    }

    static ivec2 GetChunkPosForBlock(ivec3 pos)
    {
        return {proper_div(pos.x, Chunk::width), proper_div(pos.z, Chunk::width)};
    }

    void Render(const glm::mat4 &view, vec3 pos, vec3 center)
    {
        ivec2 center_chunk = GetChunkPosForBlock(center);
        for (int y = -render_distance; y <= render_distance; y++)
        {
            for (int x = -render_distance; x <= render_distance; x++)
            {
                auto it = chunks.find(center_chunk + ivec2(x,y));
                if (it == chunks.end())
                    continue;
                if (it->second.dirty_mesh && !it->second.in_mesh_update_queue)
                {
                    it->second.in_mesh_update_queue = 1;
                    mesh_upd_que.push_back(it->first);
                }
                it->second.Render(view, pos + vec3(it->first.x * Chunk::width, 0, it->first.y * Chunk::width));
            }
        }
    }

    void Tick(vec3 center)
    {
        GenerateChunks(center);

        if (mesh_update_timer)
            mesh_update_timer--;
        else
        {
          repeat:
            ivec2 *pos = PollChunkForMeshUpdate();
            if (!pos)
                return;
            auto it = chunks.find(*pos);
            if (it == chunks.end() || !it->second.dirty_mesh)
                goto repeat;
            it->second.dirty_mesh = 0;
            it->second.in_mesh_update_queue = 0;
            UpdateChunkMesh(*pos);
            mesh_update_timer = mesh_update_period;
        }
    }
};