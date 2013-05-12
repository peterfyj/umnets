#ifndef TRAFFIC_PERMUTATIONALGEOMETRIC_H_
#define TRAFFIC_PERMUTATIONALGEOMETRIC_H_

/**
 * @file traffic/PermutationalGeometric.h
 * @brief Header for PermutationalGeometric.
 */

#include "driver/types.h"
#include "util/Math.h"
#include <vector>

/**
 * @brief Permutational link pairs with Geometric traffic flow.
 * @ingroup traffic
 */
class PermutationalGeometric {
  
  public:

    /**
     * @brief Create instance.
     * @return The instance that should be deleted by user.
     */
    static PermutationalGeometric* create(Driver& driver);

    /**
     * @brief Announce the options that is recognized by the class.
     */
    static void announce_options(Driver& driver);

    ~PermutationalGeometric();

    /**
     * @brief Create permutational links.
     */
    void create_traffic();

    /**
     * @brief Assign locally generated packets to @a node.
     */
    void assign_packet(Node& node);

  private:

    PermutationalGeometric(Driver& driver, double lambda, int tag);

    Driver& driver;
    double lambda;
    int tagged_sender;

};

#endif
