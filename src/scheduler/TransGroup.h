#ifndef SCHEDULER_TRANSGROUP_H_
#define SCHEDULER_TRANSGROUP_H_

#include "driver/types.h"

class TransGroup {
  
  public:

    static TransGroup* create(Driver& driver);
    static void announce_options(Driver& driver);

  private:

    TransGroup(Driver& driver, int alpha_x, int alpha_y);

    Driver& driver;
    int alpha_x;
    int alpha_y;

};

#endif
