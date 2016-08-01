#include "Utils.h"

namespace Internal
{
    // Internal stuff for Jo().
    std::stringstream ss;
    const std::stringstream::fmtflags stdfmt = ss.flags();
}

double pi = std::atan(1)*4;