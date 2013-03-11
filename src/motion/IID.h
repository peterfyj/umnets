#ifndef MOTION_IID_H_
#define MOTION_IID_H_

/**
 * @file motion/IID.h
 * @brief Header for IID.
 */

#include "driver/types.h"

/**
 * @brief I.I.D. motion pattern.
 * @ingroup motion
 */
class IID {
  
  public:

    /**
     * @brief Create instance.
     * @return The instance that should be deleted by user.
     */
    static IID* create(Driver& driver);

    /**
     * @brief Announce the options that is recognized by the class.
     */
    static void announce_options(Driver& driver);

    /**
     * @brief Randomly place a node.
     */
    IntPos random_place(Node& node) const;

    /**
     * @brief Randomly move a node.
     */
    IntPos random_move(Node& node);

  private:

    IID(Driver& driver, int step);

    Driver& driver;
    int step;

};

#endif
