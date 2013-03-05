#ifndef PACK_SINGLEPACKET_H_
#define PACK_SINGLEPACKET_H_

#include "driver/types.h"
#include <vector>

class SinglePacket {
  
  public:

    typedef std::vector<int> TimeStamp;

    static SinglePacket* create(Driver& driver);
    static void announce_options(Driver& driver);

    TimeStamp& get_time_stamp();
    void set_src(Node& src);
    Node& get_src() const;
    void set_dest(Node& dest);
    Node& get_dest() const;
    void set_tag(int tag);
    int get_tag() const;
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
