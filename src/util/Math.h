#ifndef UTIL_MATH_H_
#define UTIL_MATH_H_

#include <random>

class Math {

  public:

    static bool happen(int numerator, int denominator);

    static bool happen(double prob);

    static int get_random_int(int start, int end);

    static double get_random_probability();

  private:

    static std::default_random_engine engine;
    static std::uniform_real_distribution<double> prob_distribution;

};

#endif // UTIL_MATH_H_
