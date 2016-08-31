/*
 * @author Egor
 * @author Barney Wilks
 */

#pragma once
#ifdef _MSC_VER
#pragma warning(disable: 4996)	// fopen is 'deprecated'
#pragma warning(disable: 4003)  // Empty F() macro warning disable
#endif

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
    inline std::ostream &operator<<(std::ostream &s, prefix##vec2 obj) {s << "vec2: [" << obj.x << ',' << obj.y << ']'; return s;} \
    inline std::ostream &operator<<(std::ostream &s, prefix##vec3 obj) {s << "vec3: [" << obj.x << ',' << obj.y << ',' << obj.z << ']'; return s;} \
    inline std::ostream &operator<<(std::ostream &s, prefix##vec4 obj) {s << "vec4: [" << obj.x << ',' << obj.y << ',' << obj.z << ',' << obj.w << ']'; return s;}
F() F(d) F(i) F(u) F(b) F(i64) // Prefixes for types of vectors to print.
#undef F

#ifdef _MSC_VER
#define FORCEINLINE __forceinline
#elif (__MINGW32__) || (__GNUC__)
#define FORCEINLINE __attribute__((always_inline))
#endif

// A useful macro for MSVC users to print to the VS Output window (not the console). Good for debugging
// If there is a GCC/CodeBlocks equivilant then this should be renamed to DEBUG
#ifdef _MSC_VER
#ifdef _DEBUG
#define MSVC_LOG(s)            \
{                             \
   std::ostringstream stream;    \
   stream << "[DEBUG] [" << __FILE__ " (" << __LINE__ << ")]: " << s << std::endl;                   \
   OutputDebugString( stream.str().c_str() );  \
}
#else
#define MSVC_LOG(s)
#endif
#else
#define MSVC_LOG(s)
#endif

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

inline int StrToInt(const char *_char)
{
	std::istringstream stream(_char);
	int out;
	stream >> out;
	return out;
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


template <typename T> 
__forceinline T proper_div(T a, T b)
{
	if (a >= 0)
		return a / b;
	else
		return (a + 1 - std::abs(b)) / b;
}

template <typename T> 
__forceinline T proper_mod(T a, T b)
{
	if (a >= 0)
		return a % b;
	else
		return std::abs(b) - 1 + (a + 1) % b;
}

template <typename T> T clamp(T val, std::common_type_t<T> min, std::common_type_t<T> max)
{
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
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

template <typename M, typename V>
void MapToVector(const  M & m, V & v) {
	for (typename M::const_iterator it = m.begin(); it != m.end(); ++it) {
		v.push_back(it->second);
	}
}

inline uint16_t Noise(uint16_t in)
{
    return (unsigned int)((in * 1103515245 + 12345)/65536) % 32768;
}

uint32_t Noise32(uint32_t in);
inline uint32_t HashVec2(ivec2 v)
{
    return v.x ^ Noise32(v.y);
}
inline uint32_t HashVec3(ivec3 v)
{
    return v.x ^ Noise32(v.y ^ Noise32(v.z));
}

inline float Noise32f(uint32_t in, unsigned int precision = 4096)
{
    return (int(Noise32(in) % (precision * 2 + 1)) - int(precision)) / float(precision);
}

FORCEINLINE float HermiteInterpolationWithDerivatives(const float& a, const float& b, const float& da, const float& db, const float& t)
{
	float t3 = t*t*t;
	float t2 = t*t;
	return (2 * t3 - 3 * t2 + 1) * a + (t3 - 2 * t2 + t) * da + (-2 * t3 + 3 * t2) * b + (t3 - t2) * db;
}
FORCEINLINE float HermiteInterpolation(const float& aa, const float& a, const float& b, const float& bb, const float& t)
{
    return HermiteInterpolationWithDerivatives(a, b, (b-aa)/2, (bb-a)/2, t);
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

namespace io
{
	struct File
	{
		char *data;
		File(const char *filepath)
		{
			Open(filepath);
		}
		File() {}
		~File() { Close(); }

		void Open(const char *filepath)
		{
			FILE *file = fopen(filepath, "rb");
			fseek(file, 0, SEEK_END);
			long fileSize = ftell(file);
			fseek(file, 0, SEEK_SET);
			data = new char[fileSize + 1];
			fread(data, fileSize, 1, file);
			fclose(file);
			data[fileSize] = 0;
		}

		void Close()
		{
			delete[] data;
			data = NULL;
		}
	};
}

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