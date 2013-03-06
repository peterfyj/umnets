#include "packet/SinglePacket.h"
#include "driver/Driver.h"

SinglePacket* SinglePacket::create(Driver& driver) {
  return new SinglePacket();
}

void SinglePacket::announce_options(Driver& driver) {
}

auto SinglePacket::get_time_stamp() -> TimeStamp& {
  return time_stamp;
}

void SinglePacket::set_src(Node& src) {
  this->src = &src;
}

Node& SinglePacket::get_src() const {
  return *src;
}

void SinglePacket::set_dest(Node& dest) {
  this->dest = &dest;
}

Node& SinglePacket::get_dest() const {
  return *dest;
}

void SinglePacket::set_tag(int tag) {
  packet_tag = tag;
}

int SinglePacket::get_tag() const {
  return packet_tag;
}

SinglePacket* SinglePacket::clone() const {
  return new SinglePacket(*this);
}

SinglePacket::SinglePacket() {
}

SinglePacket::SinglePacket(const SinglePacket& ref)
  : src(ref.src), dest(ref.dest), time_stamp(ref.time_stamp)
  , packet_tag(ref.packet_tag) {
}
