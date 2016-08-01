#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <unordered_set>
#include <Utils.h>
#include <rendering/Primitives.h>
#include <glm/glm.hpp>

// Some weird MSVC thing where it defines min & max
// macros which conflict with std::max and std::min.
#ifdef _MSC_VER
#undef max
#undef min
#endif

constexpr int units_per_voxel = 256;
constexpr double gravity = .1f; // This is measured in voxels/ticks^2.

struct AABB
{
    i64vec3 pos;
    ivec3 size;

    AABB() : pos(0,0,0), size(1,1,1) {}
    AABB(const i64vec3 &position, const ivec3 &sz) : pos(position), size(sz) {}

    bool intersects(const AABB &other, const i64vec3 &assume_self_offset = i64vec3(0,0,0)) const
    {
        ivec3 size_sum{std::max(size.x,0) + std::max(other.size.x,0),
                       std::max(size.y,0) + std::max(other.size.y,0),
                       std::max(size.z,0) + std::max(other.size.z,0)};

        size_sum /= 2;

        i64vec3 abs_pos_diff = pos + assume_self_offset - other.pos;
        abs_pos_diff.x = std::abs(abs_pos_diff.x);
        abs_pos_diff.y = std::abs(abs_pos_diff.y);
        abs_pos_diff.z = std::abs(abs_pos_diff.z);

        return abs_pos_diff.x < size_sum.x &&
               abs_pos_diff.y < size_sum.y &&
               abs_pos_diff.z < size_sum.z;
    }

    void debugDraw(const glm::mat4 &view, const vec3 &color, const i64vec3 &camera_pos = {0,0,0}) const
    {
        Primitives::drawCube(view, vec3(pos - camera_pos) / float(units_per_voxel), vec3(size) / 2.f, color);
    }
};

enum class Solid              {no=0, yes=1};
enum class AffectedByGravity  {no=0, yes=1};

class PhysicalBody : public AABB
{
  public:
    using ObjectListType = std::unordered_set<PhysicalBody *>;
  private:
    static ObjectListType &MutableObjectList()
    {
        static ObjectListType list;
        return list;
    }
  public:
    static const ObjectListType &ObjectList()
    {
        return MutableObjectList();
    }


    // `vel` and `vel_lag` are measured in voxels, but `pos` and `size` are not. Those are multiplied by `units_per_voxel`.

    vec3 vel = vec3(0,0,0), vel_lag = vec3(0,0,0);

    bool solid, affected_by_gravity;

    PhysicalBody() {MutableObjectList().insert(this);}

    PhysicalBody(const i64vec3 &position, const ivec3 &sz, Solid solid = Solid::no, AffectedByGravity grav = AffectedByGravity::yes)
		: AABB(position, sz), solid(solid!= Solid::no), affected_by_gravity(grav != AffectedByGravity::no) {MutableObjectList().insert(this);}

    // Deleted those for now to make my life easier. :P
    PhysicalBody(const PhysicalBody &o) = delete;
    PhysicalBody(PhysicalBody &&o) = delete;
    PhysicalBody &operator=(const PhysicalBody &o) = delete;
    PhysicalBody &operator=(PhysicalBody &&o) = delete;

    ~PhysicalBody()
    {
        auto it = MutableObjectList().find(this);
        if (it == MutableObjectList().end())
            Error("PhysicalBody can't find itself in objects list when executing the destructor.");
        MutableObjectList().erase(it);
    }

    void tick()
    {
        vec3 sum_vel = (vel + vel_lag) * float(units_per_voxel);
        ivec3 int_vel(iround<int>(sum_vel.x),
                      iround<int>(sum_vel.y),
                      iround<int>(sum_vel.z));
        pos += int_vel;
        vel_lag = (sum_vel - vec3(int_vel)) / float(units_per_voxel);

        if (affected_by_gravity)
            vel.y -= gravity;
    }
};
