#ifndef NODE_HHRFNODE_H_
#define NODE_HHRFNODE_H_

#include "driver/types.h"
#include <list>
#include <unordered_map>

class HHRfNode {
  
  public:

    typedef std::list<PacketPtr> Queue;
    typedef std::unordered_map<HHRfNode*, Queue> RelayMap;
    typedef std::unordered_map<HHRfNode*, int> RequestMap;

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
    void scheduled();
    void receive(HHRfNode& src, PacketPtr&& packet);

  private:

    HHRfNode(Driver& driver, int f);

    void SD(HHRfNode& dest);
    void SR(HHRfNode& relay);
    void RD(HHRfNode& other_dest);
    Queue::iterator find_in_sequent_queue(Queue& q, int tag);

    Driver& driver;
    Queue waiting_queue;
    Queue sent_queue;
    RelayMap relay_map;

    /**
     * @brief Request packet tag for a given destination node.
     *
     * If the given destination node is dest_node, the tag is considered for
     * the next locally generated packet.
     *
     * If the given destination node is this, the tag is considered for the
     * packet this is requesting.
     */
    RequestMap request_map;
    int node_tag;
    HHRfNode* dest_node;
    IntPos pos;
    int f, dispatched;

};

#endif
