#include "master_non_blocking.h"
#include <vcml.h>

MasterNonBlocking::MasterNonBlocking(sc_module_name name) :
  sc_module(name),
  non_blocking_port("non_blocking_port") {
  SC_THREAD(ReadThread);
  SC_THREAD(WriteThread);
}

void MasterNonBlocking::ReadThread() {
  // TODO: implement
}

/**
 * @brief Write thread write self sentence to slow memory every 1 clock cycles.
 */
void MasterNonBlocking::WriteThread() {
  unsigned int write_index = 0;
  while (write_index < sizeof(sentence)-1) {
    wait(1*kClockPeriodNS, SC_NS);
    // Check previous request stuatus before sending new request.
    Status previous_request_status = non_blocking_port->GetStatus(kMasterNonBlockingPriority);
    if ((previous_request_status == TRANSFER_REQUEST) ||
        (previous_request_status == TRANSFER_WAIT)) {
      vcml::log.warn("[MasterNonBlocking] previous request is not finished");
    } else {
      vcml::log.info("[MasterNonBlocking] write data: %c to SlowMem address: 0x%x",
          sentence[write_index], kSlowMemStartAddress+write_index);
      non_blocking_port->Write(kMasterNonBlockingPriority, (int8_t *)(sentence+write_index),
          kSlowMemStartAddress+write_index, false);
      write_index++;
    }
  }
}
