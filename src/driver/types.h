#ifndef DRIVER_TYPES_H_
#define DRIVER_TYPES_H_

/**
 * @file driver/types.h
 * @brief Header for basic type definitions.
 */

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

/**
 * @brief Reference for @ref logger.
 */
typedef Hlogger Logger;

/**
 * @brief Reference for @ref motion.
 */
typedef Hmotion Motion;

/**
 * @brief Reference for @ref network.
 */
typedef Hnetwork Network;

/**
 * @brief Reference for @ref node.
 */
typedef Hnode Node;

/**
 * @brief Reference for @ref packet.
 */
typedef Hpacket Packet;

/**
 * @brief Reference for @ref scheduler.
 */
typedef Hscheduler Scheduler;

/**
 * @brief Reference for @ref traffic.
 */
typedef Htraffic Traffic;

/**
 * @brief std::unique_ptr for @ref logger.
 */
typedef std::unique_ptr<Logger> LoggerPtr;

/**
 * @brief std::unique_ptr for @ref motion.
 */
typedef std::unique_ptr<Motion> MotionPtr;

/**
 * @brief std::unique_ptr for @ref network.
 */
typedef std::unique_ptr<Network> NetworkPtr;

/**
 * @brief std::unique_ptr for @ref node.
 */
typedef std::unique_ptr<Node> NodePtr;

/**
 * @brief std::unique_ptr for @ref packet.
 */
typedef std::unique_ptr<Packet> PacketPtr;

/**
 * @brief std::unique_ptr for @ref scheduler.
 */
typedef std::unique_ptr<Scheduler> SchedulerPtr;

/**
 * @brief std::unique_ptr for @ref traffic.
 */
typedef std::unique_ptr<Traffic> TrafficPtr;

/**
 * @brief std::pair of integer that represents a discrete position.
 */
typedef std::pair<int, int> IntPos;

/**
 * @brief std::pair of double that represents a continuous position.
 */
typedef std::pair<double, double> DoublePos;

/**
 * @brief Struct for a position that is both discrete and continuous.
 */
typedef struct {
  /**
   * @brief Integer part of the position.
   */
  IntPos i;

  /**
   * @brief Double part of the position.
   */
  DoublePos d;
} Pos;

#endif
