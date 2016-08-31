#include "Map.h"

#include "AssetData.h"

#define BLOCK(token, name, color, solid) color
#define INVISIBLE 0,
#define COLOR(...) 1,
const bool *Block::visible_table = new bool[type_count]{BLOCK_LIST};
#undef INVISIBLE
#undef COLOR
#define INVISIBLE {},
#define COLOR(r,g,b) {r/255.f, g/255.f, b/255.f},
const vec3 *Block::color_table = new vec3[type_count]{BLOCK_LIST};
#undef INVISIBLE
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



void Map::GenerateChunks(vec3 center)
{
    ivec2 center_chunk = GetChunkPosForBlock(center);
    for (int yy = -generation_distance; yy <= generation_distance; ++yy)
    {
        for (int xx = -generation_distance; xx <= generation_distance; ++xx)
        {
            ivec2 chunk = center_chunk + ivec2(xx,yy);
            auto it = chunks.find(chunk);
            if (it != chunks.end())
                continue;

            it = AddChunk(chunk);

            // Generation code start

            NoiseData terrain_noise, color_noise;
            MapFuncs::GenNoise(&terrain_noise, seed, [](float in) -> float { return in / 1.7; }, chunk, 0);
			MapFuncs::GenNoise(&color_noise, Noise32(seed), [](float in) -> float { return in; }, chunk, 1);

            for (int y = 0; y < Chunk::depth; ++y)
            {
                for (int x = chunk.x*Chunk::width; x < (chunk.x+1)*Chunk::width; ++x)
                {
                    for (int z = chunk.y*Chunk::width; z < (chunk.y+1)*Chunk::width; ++z)
                    {
                        float height = MapFuncs::GetNoise<Chunk>(terrain_noise, chunk, x, z);
						float block_type_noise = MapFuncs::GetNoise<Chunk>(color_noise, chunk, x, z)/11+0.5;
						SetBlock_NoMeshUpdateI(it, {x,y,z}, Block{Block::Type(int(y < height * 24 + Chunk::depth/2 ? int(Block::Type::grass_a) + clamp(4*block_type_noise,0,3) : 0))});
                    }
                }
            }

            // Generation code end

            it->second.dirty_mesh = 1;
            it->second.in_mesh_update_queue = 1;
            mesh_gen_que.push_back(it->first);
            auto SetDirtyFlag = [&](ivec2 offset)
            {
                auto it = chunks.find(chunk + offset);
                if (it == chunks.end())
                    return;
                it->second.dirty_mesh = 1;
            };
            SetDirtyFlag({1,0});
            SetDirtyFlag({0,1});
            SetDirtyFlag({-1,0});
            SetDirtyFlag({0,-1});
        }
    }
}