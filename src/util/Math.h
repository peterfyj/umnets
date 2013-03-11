#ifndef UTIL_MATH_H_
#define UTIL_MATH_H_

/**
 * @file util/Math.h
 * @brief Header for Math.
 */

#include <random>
#include <vector>

/**
 * @brief Mathematical tools.
 * @ingroup util
 */
class Math {

  public:

    /**
     * @brief The generator for Poisson process generation with a given lambda.
     */
    typedef std::poisson_distribution<int> PoissonGenerator;

    /**
     * @brief Uniform generator for a given range.
     */
    typedef std::uniform_real_distribution<double> UniformGenerator;

    /**
     * @brief The engine for the random number fetch.
     */
    typedef std::default_random_engine Engine;

    /**
     * @brief Return true with probability @a numerator / @a denominator.
     * @param numerator Numerator for the probability.
     * @param denominator Denominator for the probability.
     * @return True if it happens.
     */
    static bool happen(int numerator, int denominator);

    /**
     * @brief Return true with probability @a prob.
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

    /**
     * @brief Return a poisson generator.
     * @param lambda The lambda for the poisson distribution.
     * @return The poisson generator containing operator().
     */
    static PoissonGenerator get_poisson_generator(double lambda);

    /**
     * @brief Return a uniform generator between [@a s, @a e].
     * @param s Starting point for the distribution.
     * @param e Ending point for the distribution.
     * @return The uniform generator containing operator().
     */
    static UniformGenerator get_uniform_generator(double s, double e);

    /**
     * @brief Return a derangement of [0, size).
     * @return The derangement vector.
     */
    static std::vector<int> get_random_derangement(int size);

    /**
     * @brief Test whether @a v is a derangement.
     * @return True if it is a derangement.
     */
    static bool is_derangement(std::vector<int>& v);

    /**
     * @brief Default initialized engine.
     * @return The engin that may be used.
     */
    static Engine& get_engine();

  private:

    static Engine engine;
    static UniformGenerator prob_distribution;

};

#endif // UTIL_MATH_H_
