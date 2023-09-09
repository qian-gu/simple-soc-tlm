#include "fast_mem.h"
#include <vcml.h>

FastMem::FastMem(sc_module_name name, unsigned int start_address, unsigned int end_address) :
  sc_prim_channel(name),
  start_address_(start_address),
  end_address_(end_address) {
  sc_assert(start_address_ <= end_address_);
  unsigned int size = end_address_ - start_address_ + 1;
  mem_ = new int8_t[size];
  for (unsigned int i = 0; i < size; i++) mem_[i] = 0;
}

inline FastMem::~FastMem() {
  if (mem_) delete [] mem_;
  mem_ = (int8_t*)0;
}

inline bool FastMem::DirectRead(int8_t *data, unsigned int address) {
  return (Read(data, address) == TRANSFER_OK);
}

inline bool FastMem::DirectWrite(int8_t *data, unsigned int address) {
  return (Write(data, address) == TRANSFER_OK);
}

inline Status FastMem::Read(int8_t *data, unsigned int address) {
  if ((address < start_address_) || (address > end_address_)) {
    *data = 0;
    return TRANSFER_ERROR;
  }
  *data = mem_[address-start_address_];
  vcml::log.info("    |--> [FastMem] read data: %c from 0x%x", *data, address);
  return TRANSFER_OK;
}

inline Status FastMem::Write(int8_t *data, unsigned int address) {
  if ((address < start_address_) || (address > end_address_))
    return TRANSFER_ERROR;
  vcml::log.info("    |--> [FastMem] write data: %c to 0x%x", *data, address);
  mem_[address-start_address_] = *data;
  return TRANSFER_OK;
}

inline unsigned int FastMem::GetStartAddress() const {
  return start_address_;
}

inline unsigned int FastMem::GetEndAddress() const {
  return end_address_;
}
