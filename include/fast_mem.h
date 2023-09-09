#ifndef _SIMPLE_SOC_TLM_FAST_MEM_H_
#define _SIMPLE_SOC_TLM_FAST_MEM_H_

#include <systemc>
#include "slave_if.h"
#include "types.h"

using namespace sc_core;

class FastMem : public SlaveIF, public sc_prim_channel {
  public:

    // Constructor/destructor declarations.
    FastMem(sc_module_name name, unsigned int start_address, unsigned int end_address);
    ~FastMem();

    // DirectIF
    bool DirectRead(int8_t *data, unsigned int address);
    bool DirectWrite(int8_t *data, unsigned int address);

    // SlaveIF
    Status Read(int8_t *data, unsigned int address);
    Status Write(int8_t *data, unsigned int address);
    unsigned int GetStartAddress() const;
    unsigned int GetEndAddress() const;

  private:
    int8_t *mem_;
    unsigned int start_address_;
    unsigned int end_address_;
};

#endif /* _SIMPLE_SOC_TLM_FAST_MEM_H_ */
