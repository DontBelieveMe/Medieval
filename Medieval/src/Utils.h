// Utility stuff by Egor

#include <iostream>
#include <glm/glm.hpp>

// Following enables printing glm vectors with stadard streams like cout.
#define F(prefix) \
    inline std::ostream &operator<<(std::ostream &s, glm::prefix##vec2 obj) {s << '[' << obj.x << ',' << obj.y << ']'; return s;} \
    inline std::ostream &operator<<(std::ostream &s, glm::prefix##vec3 obj) {s << '[' << obj.x << ',' << obj.y << ',' << obj.z << ']'; return s;} \
    inline std::ostream &operator<<(std::ostream &s, glm::prefix##vec4 obj) {s << '[' << obj.x << ',' << obj.y << ',' << obj.z << ',' << obj.w << ']'; return s;}
F() F(d) F(i) F(u) F(b) // Prefixes for types of vectors to print.
#undef F