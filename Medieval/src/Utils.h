#pragma once
#ifdef _MSC_VER
#pragma warning(disable: 4003)
#endif

// Utility stuff by Egor

#include <iostream>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <type_traits>
#include <random>

#include <../extern/glad/glad.h>
#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::ivec2;
using glm::ivec3;
using glm::ivec4;
using glm::dvec2;
using glm::dvec3;
using glm::dvec4;
using glm::uvec2;
using glm::uvec3;
using glm::uvec4;
using glm::bvec2;
using glm::bvec3;
using glm::bvec4;

using i64vec2 = glm::tvec2<int64_t, glm::highp>;
using i64vec3 = glm::tvec3<int64_t, glm::highp>;
using i64vec4 = glm::tvec4<int64_t, glm::highp>;

#define dynamic_assert(expression) \
	{ if(!(expression)) { assert(false); } }

// Following enables printing glm vectors with stadard streams like cout.
#define F(prefix) \
    inline std::ostream &operator<<(std::ostream &s, prefix##vec2 obj) {s << '[' << obj.x << ',' << obj.y << ']'; return s;} \
    inline std::ostream &operator<<(std::ostream &s, prefix##vec3 obj) {s << '[' << obj.x << ',' << obj.y << ',' << obj.z << ']'; return s;} \
    inline std::ostream &operator<<(std::ostream &s, prefix##vec4 obj) {s << '[' << obj.x << ',' << obj.y << ',' << obj.z << ',' << obj.w << ']'; return s;}
F() F(d) F(i) F(u) F(b) F(i64) // Prefixes for types of vectors to print.
#undef F

namespace Internal
{
    // Internal stuff for Jo().
    extern std::stringstream ss;
    extern const std::stringstream::fmtflags stdfmt;
}

// Convenient string joiner.
// Works like std::cout, but uses commas to separate arguments.
// Example:  Jo("1 + 2 == ", 1+2) == "1 + 2 == 3"
template <typename ...P>
const char *Jo(P &&... p)
{
    static constexpr int ret_buffers_c = 32;
    static std::string ret_buffers[ret_buffers_c];
    static int ret_pos = 0;
	Internal::ss.clear();
	Internal::ss.flags(Internal::stdfmt);
	Internal::ss.str("");
    int dummy[] {(Internal::ss << p, 0)...};
    (void)dummy;
    ret_buffers[ret_pos] = Internal::ss.str();
    const char *ret = ret_buffers[ret_pos].c_str();
    ret_pos = (ret_pos + 1) % ret_buffers_c;
    return ret;
}

// Prints error and terminates application. Same syntax as Jo().
template <typename ...P>
[[noreturn]]
void Error(P &&... p)
{
	std::cout << Jo(p...) << std::endl;
	system("pause");
	std::exit(0);
}

template <typename T> int sign(T val)
{
    return (val > 0) - (val < 0);
}

template <typename I, typename F> I iround(F f)
{
    static_assert(std::is_integral<I>::value, "Non integral template parameter I makes no sense for this function.");
    return I(f + .5 * sign(f));
}


template <typename T> T proper_div(T a, T b)
{
	if (a >= 0)
		return a / b;
	else
		return (a + 1 - std::abs(b)) / b;
}

template <typename T> T proper_mod(T a, T b)
{
	if (a >= 0)
		return a % b;
	else
		return std::abs(b) - 1 + (a + 1) % b;
}


inline std::mt19937 &Rng()
{
    static std::mt19937 gen(std::random_device{}());
    return gen;
}

inline void setRngSeed(uint32_t val = std::random_device()())
{
    Rng().seed(val);
}

inline uint32_t Random()
{
    return Rng()();
}


extern double pi;

namespace str
{
	inline bool strcmp(const char *one, const char *two, int len)
	{
		if (one[0] != two[0])
			return false;

		for (int i = 1; i <= len; ++i)
			if (one[i] != two[i]) return false;

		return true;
	}
}

namespace Colors
{
    const glm::vec3 red       (1 ,0 ,0 ),
                    orange    (1 ,.5,0 ),
                    yellow    (1 ,1 ,0 ),
                    green     (0 ,1 ,0 ),
                    cyan      (0 ,1 ,1 ),
                    cerulean  (0 ,.5,1 ),
                    blue      (0 ,0 ,1 ),
                    magenta   (1 ,0 ,1 ),
                    white     (1 ,1 ,1 ),
                    light_gray(.75,.75,.75),
                    light_grey = light_gray,
                    gray      (.5,.5,.5),
                    grey       = gray,
                    dark_gray (.25,.25,.25),
                    dark_grey = dark_gray,
                    black     (0 ,0 ,0 );
}


class Bounds2D
{
  public:
	glm::vec2 top_left, bottom_right;

	Bounds2D(const glm::vec2 &top_left, const glm::vec2 &bottom_right)
		: top_left(top_left), bottom_right(bottom_right) {}

	bool Contains(const glm::vec2 &point) const
	{
		return (top_left.x < point.x && top_left.y < point.y && bottom_right.x > point.x && bottom_right.y > point.y);
	}
};


namespace std
{
    template <> struct hash<glm::ivec2>
    {
        std::size_t operator()(const glm::ivec2 &ref) const
        {
            GLuint tmp = ref.x, ret = 0;
            while (tmp)
            {
                ret = ret * 2 + tmp % 2;
                tmp /= 2;
            }
            return std::size_t(tmp + GLuint(ref.y));
        }
    };
}