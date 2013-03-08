#ifndef TRAFFIC_PERMUTATIONALPOISSON_H_
#define TRAFFIC_PERMUTATIONALPOISSON_H_

/**
 * @file traffic/PermutationalPoisson.h
 * @brief Header for PermutationalPoisson.
 */

#include "driver/types.h"
#include "util/Math.h"
#include <vector>

/**
 * @brief Permutational link pairs with Poisson traffic flow.
 * @ingroup traffic
 */
class PermutationalPoisson {
  
  public:

    /**
     * @brief Create instance.
     * @return The instance that should be deleted by user.
     */
    static PermutationalPoisson* create(Driver& driver);

    /**
     * @brief Announce the options that is recognized by the class.
     */
    static void announce_options(Driver& driver);

    ~PermutationalPoisson();

    /**
     * @brief Create permutational links.
     */
    void create_traffic();

    /**
     * @brief Assign locally generated packets to @a node.
     */
    void assign_packet(Node& node);

  private:

    PermutationalPoisson(Driver& driver, double lambda, int tag);

    Driver& driver;
    Math::PoissonGenerator poisson;
    Math::Engine& engine;
    double lambda;
    int tagged_sender;

};

#endif
