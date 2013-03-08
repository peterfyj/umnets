#ifndef SCHEDULER_TRANSGROUP_H_
#define SCHEDULER_TRANSGROUP_H_

/**
 * @file scheduler/TransGroup.h
 * @brief Header for TransGroup.
 */

#include "driver/types.h"

/**
 * @brief Transmission group scheduling algorithm.
 * @ingroup scheduler
 */
class TransGroup {
  
  public:

    /**
     * @brief Create instance.
     * @return The instance that should be deleted by user.
     */
    static TransGroup* create(Driver& driver);

    /**
     * @brief Announce the options that is recognized by the class.
     */
    static void announce_options(Driver& driver);

    /**
     * @brief Schedule the senders whose scheduled() will be called.
     */
    void schedule();

  private:

    TransGroup(Driver& driver, int alpha_x, int alpha_y);

    Driver& driver;
    int alpha_x;
    int alpha_y;
    int start_x;
    int start_y;

};

#endif
