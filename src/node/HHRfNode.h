#ifndef NODE_HHRFNODE_H_
#define NODE_HHRFNODE_H_

#include "driver/types.h"

class HHRfNode {
  
  public:

    static HHRfNode* create(Driver& driver, int tag);
    static void announce_options(Driver& driver);

    ~HHRfNode();

    int get_tag() const;
    const IntPos& get_pos() const;
    void set_pos(IntPos& pos);

  private:

    explicit HHRfNode(int tag);

    int my_tag;
    int dst_tag;
    IntPos pos;

};

#endif
