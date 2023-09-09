#include "slow_mem.h"
#include <vcml.h>

SlowMem::SlowMem(sc_module_name name, unsigned int start_address, unsigned int end_address, unsigned int response_cycle) :
  sc_module(name),
  start_address_(start_address),
  end_address_(end_address),
  response_cycle_(response_cycle) {
  sc_assert(start_address_ <= end_address_);
  unsigned int size = end_address_ - start_address_ + 1;
  mem_ = new int8_t[size];
  for (unsigned int i = 0; i < size; i++) mem_[i] = 0;
  SC_THREAD(WaitThread);
  sensitive << restart_count_;
  dont_initialize();
}

SlowMem::~SlowMem() {
  if (mem_) delete [] mem_;
  mem_ = (int8_t*)0;
}

inline void SlowMem::WaitThread() {
  while (true) {
    wait(1*kClockPeriodNS, SC_NS);
    if (wait_count_ >= 0) wait_count_--;
  }
}

inline bool SlowMem::DirectRead(int8_t *data, unsigned int address) {
  return (Read(data, address) == TRANSFER_OK);
}

inline bool SlowMem::DirectWrite(int8_t *data, unsigned int address) {
  return (Write(data, address) == TRANSFER_OK);
}

inline Status SlowMem::Read(int8_t *data, unsigned int address) {
  // Accpet new request if previous request is done.
  if (wait_count_ < 0) {
    wait_count_ = response_cycle_;
    restart_count_.notify(SC_ZERO_TIME);  // Restart to downcount for each new request.
    return TRANSFER_WAIT;
  }
  if (wait_count_ == 0) {
    *data = mem_[address - start_address_];
    vcml::log.info("    |--> [SlowMem] read data: %c from 0x%x", *data, address);
    return TRANSFER_OK;
  }
  return TRANSFER_WAIT;
}

inline Status SlowMem::Write(int8_t *data, unsigned int address) {
  // Accpet new request if previous request is done.
  if (wait_count_ < 0) {
    wait_count_ = response_cycle_;
    restart_count_.notify(SC_ZERO_TIME);  // Restart to downcount for each new request.
    return TRANSFER_WAIT;
  }
  if (wait_count_ == 0) {
    vcml::log.info("    |--> [SlowMem] write data: %c to 0x%x", *data, address);
    mem_[address - start_address_] = *data;
    return TRANSFER_OK;
  }
  return TRANSFER_WAIT;
}

inline unsigned int SlowMem::GetStartAddress() const {
  return start_address_;
}

inline unsigned int SlowMem::GetEndAddress() const {
  return end_address_;
}
