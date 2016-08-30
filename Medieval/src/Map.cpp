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
    for (int yy = -generation_distance; yy <= generation_distance; yy++)
    {
        for (int xx = -generation_distance; xx <= generation_distance; xx++)
        {
            ivec2 chunk = center_chunk + ivec2(xx,yy);
            auto it = chunks.find(chunk);
            if (it != chunks.end())
                continue;

            it = AddChunk(chunk);

            // Generation code start

            struct NoiseData
            {
                float noise[4][4], noise2[4][4], noise4[4][4], noise8[4][4];
                float noise_sub2[5][5];
                float noise_sub4[7][7];
                float noise_sub8[11][11];
                uint32_t seed;
                float micro_noise_additional_factor;
                float (*factor_modifier)(float);
            };

            auto GenNoise = [&](NoiseData *noise, uint32_t seed, float (*f_mod)(float), float micro_f = 1)
            {
                for (int x = 0; x < 4; x++)
                {
                    for (int y = 0; y < 4; y++)
                    {
                        noise->noise[y][x] = Noise32f(seed ^ HashVec2(chunk + ivec2(x,y)));
                        noise->noise2[y][x] = Noise32f(seed ^ HashVec2(ivec2(proper_div(chunk.x,2), proper_div(chunk.y,2)) + ivec2(x,y)));
                        noise->noise4[y][x] = Noise32f(seed ^ HashVec2(ivec2(proper_div(chunk.x,4), proper_div(chunk.y,4)) + ivec2(x,y)));
                        noise->noise8[y][x] = Noise32f(seed ^ HashVec2(ivec2(proper_div(chunk.x,8), proper_div(chunk.y,8)) + ivec2(x,y)));
                    }
                }
                for (int x = 0; x < 5; x++) for (int y = 0; y < 5; y++) noise->noise_sub2[y][x] = Noise32f(seed ^ HashVec2(chunk * 2 + ivec2(x,y)));
                for (int x = 0; x < 7; x++) for (int y = 0; y < 7; y++) noise->noise_sub4[y][x] = Noise32f(seed ^ HashVec2(chunk * 4 + ivec2(x,y)));
                for (int x = 0; x < 11; x++) for (int y = 0; y < 11; y++) noise->noise_sub8[y][x] = Noise32f(seed ^ HashVec2(chunk * 8 + ivec2(x,y)));
                noise->seed = seed;
                noise->micro_noise_additional_factor = micro_f;
                noise->factor_modifier = f_mod;
            };

            NoiseData terrain_noise, color_noise;
            GenNoise(&terrain_noise, seed, [](float in) -> float {return in / 1.7;}, 0);
            GenNoise(&color_noise, Noise32(seed), [](float in) -> float {return in;}, 1);

            for (int y = 0; y < Chunk::depth; y++)
            {
                for (int x = chunk.x*Chunk::width; x < (chunk.x+1)*Chunk::width; x++)
                {
                    for (int z = chunk.y*Chunk::width; z < (chunk.y+1)*Chunk::width; z++)
                    {
                        auto InterpolateSubArray = [&](const float *arr, int grid_size)
                        {
                            int arr_size = Chunk::width/grid_size + 3;
                            ivec2 sub_pos = ivec2(proper_div(proper_mod(x, Chunk::width), grid_size), proper_div(proper_mod(z, Chunk::width), grid_size));
                            float noise_line[4];
                            for (int i = 0; i < 4; i++)
                            {
                                noise_line[i] = HermiteInterpolation(arr[i+sub_pos.x+arr_size*(0+sub_pos.y)],
                                                                     arr[i+sub_pos.x+arr_size*(1+sub_pos.y)],
                                                                     arr[i+sub_pos.x+arr_size*(2+sub_pos.y)],
                                                                     arr[i+sub_pos.x+arr_size*(3+sub_pos.y)], proper_mod(z, grid_size) / float(grid_size));
                            }
                            return HermiteInterpolation(noise_line[0], noise_line[1], noise_line[2], noise_line[3], proper_mod(x, grid_size) / float(grid_size));
                        };
                        auto InterpolateSuperArray = [&](const float (*arr)[4], int grid_size)
                        {
                            float noise_line[4];
                            for (int i = 0; i < 4; i++)
                            {
                                noise_line[i] = HermiteInterpolation(arr[i][0], arr[i][1], arr[i][2], arr[i][3], proper_mod(x, grid_size) / float(grid_size));
                            }
                            return HermiteInterpolation(noise_line[0], noise_line[1], noise_line[2], noise_line[3], proper_mod(z, grid_size) / float(grid_size));
                        };
                        auto GetNoise = [&](const NoiseData &noise)
                        {
                            float ret = 0, factor = 1;
                            ret += InterpolateSuperArray(noise.noise8, Chunk::width * 8) * factor;
                            factor = noise.factor_modifier(factor);
                            ret += InterpolateSuperArray(noise.noise4, Chunk::width * 4) * factor;
                            factor = noise.factor_modifier(factor);
                            ret += InterpolateSuperArray(noise.noise2, Chunk::width * 2) * factor;
                            factor = noise.factor_modifier(factor);
                            ret += InterpolateSuperArray(noise.noise,  Chunk::width * 1) * factor;
                            factor = noise.factor_modifier(factor);
                            ret += InterpolateSubArray(*noise.noise_sub2, Chunk::width / 2) * factor;
                            factor = noise.factor_modifier(factor);
                            ret += InterpolateSubArray(*noise.noise_sub4, Chunk::width / 4) * factor;
                            factor = noise.factor_modifier(factor);
                            ret += InterpolateSubArray(*noise.noise_sub8, Chunk::width / 8) * factor;
                            factor = noise.factor_modifier(factor);
                            ret += Noise32f(noise.seed ^ HashVec2(chunk + ivec2(x,z))) * factor;
                            return ret;
                        };

                        float height = GetNoise(terrain_noise);

                        float block_type_noise = GetNoise(color_noise)/11+0.5;

                        SetBlock_NoMeshUpdate(it, {x,y,z}, Block{Block::Type(int(y < height * 24 + Chunk::depth/2 ? int(Block::Type::grass_a) + clamp(4*block_type_noise,0,3) : 0))});
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