#ifndef TRAFFIC_PERMUTATIONALPOISSON_H_
#define TRAFFIC_PERMUTATIONALPOISSON_H_

#include "driver/types.h"
#include <vector>

class PermutationalPoisson {
  
  public:

    static PermutationalPoisson* create(Driver& driver);
    static void announce_options(Driver& driver);

    ~PermutationalPoisson();

    void create_traffic();

  private:

    PermutationalPoisson(Driver& driver, double lambda);

    Driver& driver;
    double lambda;

};

#endif
