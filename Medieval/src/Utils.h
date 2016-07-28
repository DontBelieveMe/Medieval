#pragma once
// Utility stuff by Egor

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <glm/glm.hpp>

// Following enables printing glm vectors with stadard streams like cout.
#define F(prefix) \
    inline std::ostream &operator<<(std::ostream &s, glm::prefix##vec2 obj) {s << '[' << obj.x << ',' << obj.y << ']'; return s;} \
    inline std::ostream &operator<<(std::ostream &s, glm::prefix##vec3 obj) {s << '[' << obj.x << ',' << obj.y << ',' << obj.z << ']'; return s;} \
    inline std::ostream &operator<<(std::ostream &s, glm::prefix##vec4 obj) {s << '[' << obj.x << ',' << obj.y << ',' << obj.z << ',' << obj.w << ']'; return s;}
F() F(d) F(i) F(u) F(b) // Prefixes for types of vectors to print.
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
template <typename ...P> const char *Jo(P &&... p)
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
template <typename ...P> [[noreturn]] void Error(P &&... p)
{
	std::cout << Jo(p...) << std::endl;
	system("pause");
	std::exit(0);
}

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