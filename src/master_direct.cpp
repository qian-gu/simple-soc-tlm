#include "master_direct.h"
#include "types.h"
#include <vcml.h>

MasterDirect::MasterDirect(sc_module_name name) :
  sc_module(name),
  direct_port("direct_port") {
  SC_THREAD(ReadThread);
  SC_THREAD(WriteThread);
}

void MasterDirect::ReadThread() {
  // TODO: implement
}

/**
 * @brief Write thread write each character of self sentence to UART DataPort every 2 clock cycles.
 */
void MasterDirect::WriteThread() {
  unsigned write_index = 0;
  vcml::log.debug("[MasterDirect] sentence size = %ld", sizeof(sentence));
  while (write_index < sizeof(sentence)-1) {
    wait(2*kClockPeriodNS, SC_NS);
    vcml::log.info("[MasterDirect] write data: %c to address: 0x%x", sentence[write_index],
        kUARTDataPort);
    direct_port->DirectWrite((int8_t*)(sentence+write_index), kUARTDataPort);
    write_index++;
  }
}
