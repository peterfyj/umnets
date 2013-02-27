#ifndef DRIVER_TYPES_H_
#define DRIVER_TYPES_H_

#include <utility>
#include <memory>

class Hlogger;
class Hmotion;
class Hnetwork;
class Hnode;
class Hpacket;
class Hscheduler;
class Htraffic;
class Driver;

typedef Hlogger Logger;
typedef Hmotion Motion;
typedef Hnetwork Network;
typedef Hnode Node;
typedef Hpacket Packet;
typedef Hscheduler Scheduler;
typedef Htraffic Traffic;

typedef std::unique_ptr<Logger> LoggerPtr;
typedef std::unique_ptr<Motion> MotionPtr;
typedef std::unique_ptr<Network> NetworkPtr;
typedef std::unique_ptr<Node> NodePtr;
typedef std::unique_ptr<Packet> PacketPtr;
typedef std::unique_ptr<Scheduler> SchedulerPtr;
typedef std::unique_ptr<Traffic> TrafficPtr;

typedef std::pair<int, int> IntPos;
typedef std::pair<double, double> DoublePos;
typedef struct {
  IntPos i;
  DoublePos d;
} Pos;

#endif
