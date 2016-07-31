#pragma once

#include <algorithm>
#include <glm/glm.hpp>

using i64vec2 = glm::tvec2<int64_t, glm::highp>;
using i64vec3 = glm::tvec3<int64_t, glm::highp>;
using i64vec4 = glm::tvec4<int64_t, glm::highp>;

struct AABB
{
    i64vec3 pos, size;

    bool intersects(const AABB &other, const i64vec3 &assume_self_offset = i64vec3(0,0,0)) const
    {
        i64vec3 size_sum{std::max(size.x,0LL) + std::max(other.size.x,0LL),
                         std::max(size.y,0LL) + std::max(other.size.y,0LL),
                         std::max(size.z,0LL) + std::max(other.size.z,0LL)};

        size_sum /= 2;

        i64vec3 abs_pos_diff = pos + assume_self_offset - other.pos;
        abs_pos_diff.x = std::abs(abs_pos_diff.x);
        abs_pos_diff.y = std::abs(abs_pos_diff.y);
        abs_pos_diff.z = std::abs(abs_pos_diff.z);

        return abs_pos_diff.x < size_sum.x &&
               abs_pos_diff.y < size_sum.y &&
               abs_pos_diff.z < size_sum.z;
    }

	AABB(const glm::vec3& center, const glm::vec3& sz)
		: pos(center), size(sz) {}
};
