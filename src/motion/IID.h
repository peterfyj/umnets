#ifndef MOTION_IID_H_
#define MOTION_IID_H_

#include "driver/types.h"

class IID {
  
  public:

    static IID* create(Driver& driver);
    static void announce_options(Driver& driver);

    IntPos random_place(Node& node) const;
    IntPos random_move(Node& node);

  private:

    IID(Driver& driver, int step);

    Driver& driver;
    int step;

};

#endif
