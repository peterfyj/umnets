#ifndef LOGGER_PRINTER_H_
#define LOGGER_PRINTER_H_

#include "driver/types.h"
#include "stdio.h"

class Printer {

  public:

    typedef struct {
      bool log_msg;
      bool log_before_simulation;
      bool log_after_simulation;
      bool log_before_loop;
      bool log_after_loop;
      bool log_node_moved;
      bool log_packet_generated;
      bool log_packet_dispatched;
      bool log_packet_relayed;
      bool log_packet_received;
    } Switch;

    static Printer* create(Driver& driver);
    static void announce_options(Driver& driver);

    template<typename...T>
    void log_msg(const char* format, T...arg) const {
      printf(format, arg...);
    }

  private:

    Printer(Driver& driver, const Switch& control);
    Switch control;
    Driver& driver;

};

#endif
