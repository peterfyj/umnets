#ifndef UTIL_DEBUG_H_
#define UTIL_DEBUG_H_

#include <cstdio>
#include <cstdlib>

class Debug {

  public:

    template<typename...T>
    static void test(bool condition, const char* fmt, T...arg) {
      if (!condition) {
        fprintf(stderr, fmt, arg...);
        abort();
      }
    }

};

#endif
