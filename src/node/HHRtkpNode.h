#ifndef NODE_HHRTKPNODE_H_
#define NODE_HHRTKPNODE_H_

#include "driver/types.h"
#include "driver/Driver.h"
#include <list>
#include <unordered_map>

class HHRtkpNode {

  public:

    static HHRtkpNode* create(Driver& driver);

    static void announce_options(Driver& driver);

    ~HHRtkpNode();

    void set_tag(int tag);

    int get_tag() const;

    const IntPos& get_pos() const;

    void set_pos(IntPos& pos);

    void set_dest(HHRtkpNode& node);

    void add_packet(PacketPtr&& packet);

    HHRtkpNode& get_dest();

    void scheduled();

    void receive(HHRtkpNode& src, PacketPtr&& packet);

  private:

    typedef std::list<PacketPtr> Queue;
    typedef std::unordered_map<HHRtkpNode*, Queue> QueueMap;
    typedef std::unordered_map<HHRtkpNode*, Driver::TimerToken> TokenMap;

    HHRtkpNode(Driver& driver, int t, int k, double p);
    Driver::Callback get_local_time_out_callback();
    Driver::Callback get_relay_time_out_callback(HHRtkpNode& dest);
    void SD(HHRtkpNode& dest);
    void SR();
    void RD(HHRtkpNode& other_dest);

    Driver& driver;
    QueueMap queue_map;
    TokenMap token_map;

    int node_tag;
    HHRtkpNode* dest_node;
    IntPos pos;
    int t, k;
    double p;
    int seq;

};

#endif
