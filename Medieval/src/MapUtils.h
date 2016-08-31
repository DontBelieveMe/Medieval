#pragma once

#include "Utils.h"
#include <vector>

struct Vertex
{
	vec3 pos, normal, color;
};

template<int vertices_per_buffer>
class VertexArray
{
	Vertex *arr;
public:
	operator Vertex *() { return arr; }
	operator const Vertex *() const { return arr; }
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
		new (this) VertexArray((VertexArray &&)o);
		return *this;
	}
	~VertexArray()
	{
		if (arr)
			delete[] arr;
	}
};

struct NoiseData
{
	float noise[4][4], noise2[4][4], noise4[4][4], noise8[4][4];
	float noise_sub2[5][5];
	float noise_sub4[7][7];
	float noise_sub8[11][11];
	uint32_t seed;
	float(*factor_modifier)(float);
	float micro_noise_additional_factor;
};

namespace MapFuncs
{
	template <int vertices_per_buffer>
	void PushVertex(const Vertex &vertex, std::vector<VertexArray<vertices_per_buffer>>& buffer, int& vertices)
	{
		if (vertices % vertices_per_buffer == 0)
			buffer.push_back({});
		buffer[vertices / vertices_per_buffer][vertices % vertices_per_buffer] = vertex;

		vertices++;
	}

	template <int vertices_per_buffer>
	void PushQuad(const Vertex &a, const Vertex &b, const Vertex &c, const Vertex &d, std::vector<VertexArray<vertices_per_buffer>>& buffer, int& vertices)
	{
		MapFuncs::PushVertex<vertices_per_buffer>(a, buffer, vertices);
		MapFuncs::PushVertex<vertices_per_buffer>(b, buffer, vertices);
		MapFuncs::PushVertex<vertices_per_buffer>(d, buffer, vertices);
		MapFuncs::PushVertex<vertices_per_buffer>(b, buffer, vertices);
		MapFuncs::PushVertex<vertices_per_buffer>(c, buffer, vertices);
		MapFuncs::PushVertex<vertices_per_buffer>(d, buffer, vertices);
	};

	void GenNoise(NoiseData *noise, uint32_t seed, float(*f_mod)(float), const ivec2& chunk, float micro_f = 1);

	template<typename T>
	float InterpolateSubArray(const float *arr, int grid_size, int x, int z)
	{
		int arr_size = Chunk::width / grid_size + 3;
		ivec2 sub_pos = ivec2(proper_div(proper_mod(pos.x, Chunk::width), grid_size), proper_div(proper_mod(pos.y, Chunk::width), grid_size));
		float noise_line[4];
		for (int i = 0; i < 4; i++)
		{
			noise_line[i] = HermiteInterpolation(arr[i + sub_pos.x + arr_size*(0 + sub_pos.y)],
				arr[i + sub_pos.x + arr_size*(1 + sub_pos.y)],
				arr[i + sub_pos.x + arr_size*(2 + sub_pos.y)],
				arr[i + sub_pos.x + arr_size*(3 + sub_pos.y)], proper_mod(pos.y, grid_size) / float(grid_size));
		}

		return HermiteInterpolation(noise_line[0], noise_line[1], noise_line[2], noise_line[3], proper_mod(pos.x, grid_size) / float(grid_size));
	}

	float InterpolateSuperArray(const float(*arr)[4], int grid_size, int x, int z);

	template <typename Chunk>
	float GetNoise(const NoiseData &noise, const ivec2& chunk, int x, int z)
	{
		float ret = 0, factor = 1;
		ret += MapFuncs::InterpolateSuperArray(noise.noise8, Chunk::width * 8, x, z) * factor;
		factor = noise.factor_modifier(factor);
		ret += MapFuncs::InterpolateSuperArray(noise.noise4, Chunk::width * 4, x, z) * factor;
		factor = noise.factor_modifier(factor);
		ret += MapFuncs::InterpolateSuperArray(noise.noise2, Chunk::width * 2, x, z) * factor;
		factor = noise.factor_modifier(factor);
		ret += MapFuncs::InterpolateSuperArray(noise.noise, Chunk::width * 1, x, z) * factor;
		factor = noise.factor_modifier(factor);
		ret += MapFuncs::InterpolateSubArray<Chunk>(*noise.noise_sub2, Chunk::width / 2, x, z) * factor;
		factor = noise.factor_modifier(factor);
		ret += MapFuncs::InterpolateSubArray<Chunk>(*noise.noise_sub4, Chunk::width / 4, x, z) * factor;
		factor = noise.factor_modifier(factor);
		ret += MapFuncs::InterpolateSubArray<Chunk>(*noise.noise_sub8, Chunk::width / 8, x, z) * factor;
		factor = noise.factor_modifier(factor);
		ret += Noise32f(noise.seed ^ HashVec2(chunk + ivec2(x, z))) * factor;
		return ret;
	};
}