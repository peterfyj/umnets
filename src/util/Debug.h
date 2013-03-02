#ifndef UTIL_DEBUG_H_
#define UTIL_DEBUG_H_

/**
 * @file util/Debug.h
 * @brief Header for Debug.
 */

#include <cstdio>
#include <cstdlib>

/**
 * @brief Debugging tools.
 * @ingroup util
 */
class Debug {

  public:

    /**
     * @brief Assert test conditions.
     * @param condition The condition to be tested.
     * @param fmt Format for fprintf().
     * @param arg Contents for fprintf().
     *
     * If the condition is not satisified, the message formated by @a fmt will
     * be printed to stderr, and abort() will be called to terminate the
     * program.
     */
    template<typename...T>
    static void test(bool condition, const char* fmt, T...arg) {
      if (!condition) {
        fprintf(stderr, fmt, arg...);
        abort();
      }
    }

};

#endif
