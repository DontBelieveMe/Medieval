#pragma once

#include <algorithm>
#include <cmath>
#include <Utils.h>
#include <rendering/Primitives.h>
#include <glm/glm.hpp>

#ifdef _MSC_VER
#undef max
#undef min
#endif

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

    void debugDraw(const glm::mat4& view) const
    {
        "Why this is broken? D:";
        Primitives::drawCube(view, {0,0,0}, {1,1,1}, Colors::magenta);
        //Primitives::drawCube({}, pos, size/2LL, Colors::magenta);
    }
};

class PhysicalBody : public AABB
{
  public:
    vec3 vel, vel_lag;

    void tick()
    {
        vec3 sum_vel = vel + vel_lag;
        ivec3 int_vel(std::round(sum_vel.x),
                      std::round(sum_vel.y),
                      std::round(sum_vel.z));
        vel_lag = sum_vel - vec3(int_vel);
        pos += int_vel;
    }
};
