#ifndef TRAFFIC_PERMUTATIONALPOISSON_H_
#define TRAFFIC_PERMUTATIONALPOISSON_H_

#include "driver/types.h"
#include "util/Math.h"
#include <vector>

class PermutationalPoisson {
  
  public:

    static PermutationalPoisson* create(Driver& driver);
    static void announce_options(Driver& driver);

    ~PermutationalPoisson();

    void create_traffic();

    void assign_packet(Node& node);

  private:

    PermutationalPoisson(Driver& driver, double lambda);

    Driver& driver;
    Math::PoissonGenerator poisson;
    Math::Engine& engine;
    double lambda;

};

#endif
