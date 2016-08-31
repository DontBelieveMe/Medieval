#include "MapUtils.h"

#include "Utils.h"

void MapFuncs::GenNoise(NoiseData *noise, uint32_t seed, float(*f_mod)(float), const ivec2& chunk, float micro_f)
{
	for (int x = 0; x < 4; ++x)
	{
		for (int y = 0; y < 4; ++y)
		{
			ivec2 pos(x, y);
			noise->noise[y][x] = Noise32f(seed ^ HashVec2(chunk + pos));
			noise->noise2[y][x] = Noise32f(seed ^ HashVec2(ivec2(proper_div(chunk.x, 2), proper_div(chunk.y, 2)) + pos));
			noise->noise4[y][x] = Noise32f(seed ^ HashVec2(ivec2(proper_div(chunk.x, 4), proper_div(chunk.y, 4)) + pos));
			noise->noise8[y][x] = Noise32f(seed ^ HashVec2(ivec2(proper_div(chunk.x, 8), proper_div(chunk.y, 8)) + pos));
		}
	}
	for (int y = 0; y < 5; ++y) 
		for (int x = 0; x < 5; ++x) 
			noise->noise_sub2[y][x] = Noise32f(seed ^ HashVec2(chunk * 2 + ivec2(x, y)));
	for (int y = 0; y < 7; ++y) 
		for (int x = 0; x < 7; ++x) 
			noise->noise_sub4[y][x] = Noise32f(seed ^ HashVec2(chunk * 4 + ivec2(x, y)));
	for (int y = 0; y < 11; ++y) 
		for (int x = 0; x < 11; ++x) 
			noise->noise_sub8[y][x] = Noise32f(seed ^ HashVec2(chunk * 8 + ivec2(x, y)));
	noise->seed = seed;
	noise->micro_noise_additional_factor = micro_f;
	noise->factor_modifier = f_mod;
}

float MapFuncs::InterpolateSuperArray(const float(*arr)[4], int grid_size, int x, int z)
{
	float noise_line[4];
	float mod = proper_mod(x, grid_size) / (float)grid_size;
	for (int i = 0; i < 4; ++i)
	{
		noise_line[i] = HermiteInterpolation(arr[i][0], arr[i][1], arr[i][2], arr[i][3], mod);
	}
	return HermiteInterpolation(noise_line[0], noise_line[1], noise_line[2], noise_line[3], proper_mod(z, grid_size) / float(grid_size));
};