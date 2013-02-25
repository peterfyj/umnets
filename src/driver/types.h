#ifndef DRIVER_TYPES_H_
#define DRIVER_TYPES_H_

#include <utility>

class Hlogger;
class Hmotion;
class Hnetwork;
class Hnode;
class Hpacket;
class Hscheduler;
class Htraffic;
class Driver;

typedef Hlogger logger_t;
typedef Hmotion motion_t;
typedef Hnetwork network_t;
typedef Hnode node_t;
typedef Hpacket packet_t;
typedef Hscheduler scheduler_t;
typedef Htraffic traffic_t;

typedef std::pair<int, int> dpos_t;
typedef std::pair<double, double> cpos_t;
typedef struct {
  dpos_t discrete;
  cpos_t continuous;
} pos_t;

#endif
