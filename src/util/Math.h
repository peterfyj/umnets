#ifndef UTIL_MATH_H_
#define UTIL_MATH_H_

/**
 * @file util/Math.h
 * @brief Header for Math.
 */

#include <random>

/**
 * @brief Mathematical tools.
 * @ingroup util
 */
class Math {

  public:

    /**
     * @brief Return true with probability @a numerator / @a denominator.
     * @param numerator Numerator for the probability.
     * @param denominator Denominator for the probability.
     * @return True if it happens.
     */
    static bool happen(int numerator, int denominator);

    /**
     * @brief Return true with probability @prob.
     * @param prob The probability.
     * @return True if it happens.
     */
    static bool happen(double prob);

    /**
     * @brief Return a random integer uniformly within [@a start, @a end].
     * @param start Starting integer.
     * @param end Ending inclusive integer.
     * @return The random number.
     */
    static int get_random_int(int start, int end);

    /**
     * @brief Return a random double uniformly within [0, 1].
     * @return The random number.
     */
    static double get_random_probability();

  private:

    static std::default_random_engine engine;
    static std::uniform_real_distribution<double> prob_distribution;

};

#endif // UTIL_MATH_H_
