#ifndef NODE_HHRTAUNODE_H_
#define NODE_HHRTAUNODE_H_

#include "driver/types.h"
#include "driver/Driver.h"
#include <list>
#include <unordered_map>

class HHRtauNode {

  public:

    static HHRtauNode* create(Driver& driver);

    static void announce_options(Driver& driver);

    ~HHRtauNode();

    void set_tag(int tag);

    int get_tag() const;

    const IntPos& get_pos() const;

    void set_pos(IntPos& pos);

    void set_dest(HHRtauNode& node);

    void add_packet(PacketPtr&& packet);

    HHRtauNode& get_dest();

    void scheduled();

    void receive(HHRtauNode& src, PacketPtr&& packet);

  private:

    typedef std::list<PacketPtr> Queue;
    typedef std::unordered_map<HHRtauNode*, Queue> QueueMap;
    typedef std::unordered_map<HHRtauNode*, Driver::TimerToken> TokenMap;

    HHRtauNode(Driver& driver, int tau, int beta, int beta_local, double q);
    Driver::Callback get_local_time_out_callback();
    Driver::Callback get_relay_time_out_callback(HHRtauNode& dest);
    void SD(HHRtauNode& dest);
    void SR();
    void RD(HHRtauNode& other_dest);

    Driver& driver;
    QueueMap queue_map;
    TokenMap token_map;

    int node_tag;
    HHRtauNode* dest_node;
    IntPos pos;
    int tau;
    int beta, beta_local;
    double q;
    int seq;

};

#endif
