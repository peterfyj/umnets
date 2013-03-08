#ifndef PACK_SINGLEPACKET_H_
#define PACK_SINGLEPACKET_H_

/**
 * @file packet/SinglePacket.h
 * @brief Header for SinglePacket.
 */

#include "driver/types.h"
#include <vector>

/**
 * @brief Single packet with time stamp functionality.
 * @ingroup packet
 */
class SinglePacket {
  
  public:

    /**
     * @brief A vector for storing tick stamps.
     */
    typedef std::vector<int> TimeStamp;

    /**
     * @brief Create instance.
     * @return The instance that should be deleted by user.
     */
    static SinglePacket* create(Driver& driver);

    /**
     * @brief Announce the options that is recognized by the class.
     */
    static void announce_options(Driver& driver);

    /**
     * @brief Return the time stamp vector of this packet.
     * @return The standard integer vector.
     */
    TimeStamp& get_time_stamp();

    /**
     * @brief Set the source of this packet.
     */
    void set_src(Node& src);

    /**
     * @brief Return the source of this packet.
     */
    Node& get_src() const;

    /**
     * @brief Set the destination of this packet.
     */
    void set_dest(Node& dest);

    /**
     * @brief Return the destination of this packet.
     */
    Node& get_dest() const;

    /**
     * @brief Tag this packet with a given tag number.
     * 
     * The tag number is defined by other components. It may be either globally
     * effective or link effective.
     */
    void set_tag(int tag);

    /**
     * @brief Return the tag number of this packet.
     */
    int get_tag() const;

    /**
     * @brief Return a cloned packet.
     * @return The cloned packet that should be deleted by user.
     */
    SinglePacket* clone() const;

  private:

    SinglePacket();
    SinglePacket(const SinglePacket& ref);

    Node* src;
    Node* dest;
    TimeStamp time_stamp;
    int packet_tag;

};

#endif
