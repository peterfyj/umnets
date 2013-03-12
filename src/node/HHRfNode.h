#ifndef NODE_HHRFNODE_H_
#define NODE_HHRFNODE_H_

/**
 * @file node/HHRfNode.h
 * @brief Header for HHRfNode.
 */

#include "driver/types.h"
#include <list>
#include <unordered_map>

/**
 * @brief Network node with 2HR-f algorithm.
 * @ingroup node
 */
class HHRfNode {
  
  public:

    /**
     * @brief Create instance.
     * @return The instance that should be deleted by user.
     */
    static HHRfNode* create(Driver& driver);

    /**
     * @brief Announce the options that is recognized by the class.
     */
    static void announce_options(Driver& driver);

    /**
     * @brief Default destructor.
     */
    ~HHRfNode();

    /**
     * @brief Set the tag number of this node.
     */
    void set_tag(int tag);

    /**
     * @brief Get the tag number of this node.
     */
    int get_tag() const;

    /**
     * @brief Get the discrete position of this node.
     */
    const IntPos& get_pos() const;

    /**
     * @brief Set the discrete position of this node.
     *
     * @ref network should take care of positioning this node and setting its
     * postion.
     */
    void set_pos(IntPos& pos);

    /**
     * @brief Set the destination of this node.
     *
     * This should be called regarding the @ref traffic model.
     */
    void set_dest(HHRfNode& node);

    /**
     * @brief Add a packet to this node as its locally generated packet.
     */
    void add_packet(PacketPtr&& packet);
    
    /**
     * @brief Get the destination of this node.
     */
    HHRfNode& get_dest();

    /**
     * @brief Take action when being scheduled as the transmitter.
     */
    void scheduled();

    /**
     * @brief Take action when ready for receiving a packet.
     * @param src Where the packet comes from.
     * @param packet The packet to be received.
     */
    void receive(HHRfNode& src, PacketPtr&& packet);

  private:

    typedef std::list<PacketPtr> Queue;
    typedef std::unordered_map<HHRfNode*, Queue> RelayMap;
    typedef std::unordered_map<HHRfNode*, int> RequestMap;

    HHRfNode(Driver& driver, int f);

    void SD(HHRfNode& dest);
    void SR(HHRfNode& relay);
    void RD(HHRfNode& other_dest);
    Queue::iterator find_in_sequent_queue(Queue& q, int tag);
    void log_send_start(int tag);
    void log_send_end(int tag);
    void log_receive_start(int tag);
    void log_receive_end(int tag);

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
