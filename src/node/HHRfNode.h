#ifndef NODE_HHRFNODE_H_
#define NODE_HHRFNODE_H_

#include "driver/types.h"
#include <list>

class HHRfNode {
  
  public:

    typedef std::list<PacketPtr> Queue;

    static HHRfNode* create(Driver& driver);
    static void announce_options(Driver& driver);

    ~HHRfNode();

    void set_tag(int tag);
    int get_tag() const;
    const IntPos& get_pos() const;
    void set_pos(IntPos& pos);
    void set_dest(HHRfNode& node);
    void add_packet(PacketPtr&& packet);
    HHRfNode& get_dest();

  private:

    explicit HHRfNode(Driver& driver);

    Driver& driver;
    int next_packet_tag;
    Queue waiting_queue;
    Queue sent_queue;
    int node_tag;
    HHRfNode* dest_node;
    IntPos pos;

};

#endif
