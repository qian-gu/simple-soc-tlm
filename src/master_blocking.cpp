#include "master_blocking.h"
#include <cstring>
#include <vcml.h>

MasterBlocking::MasterBlocking(sc_module_name name) :
  sc_module(name),
  blocking_port("blocking_port") {
  SC_THREAD(ReadThread);
  SC_THREAD(WriteThread);
}

void MasterBlocking::ReadThread() {
  // TODO: implement
}

/**
 * @brief Write method write whole self sentence in burst style.
 */
void MasterBlocking::WriteThread() {
  wait(1*kClockPeriodNS, SC_NS);
  vcml::log.info("[MasterBlocking] write data: %s to FastMem address: 0x%x", sentence,
      kFastMemStartAddress);
  blocking_port->BurstWrite(kMasterBlockingPriority, (int8_t*)sentence, kFastMemStartAddress,
      strlen(sentence), false);
}
