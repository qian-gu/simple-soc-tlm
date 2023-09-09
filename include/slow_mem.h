#ifndef _SIMPLE_SOC_TLM_SLOW_MEM_H_
#define _SIMPLE_SOC_TLM_SLOW_MEM_H_

#include <systemc>
#include "slave_if.h"
#include "types.h"

using namespace sc_core;

// Submodule forward declarations.

class SlowMem : public SlaveIF, public sc_channel {
  public:
    // Port declarations.

    SC_HAS_PROCESS(SlowMem);

    // Constructor/destructor declarations.
    SlowMem(sc_module_name name, unsigned int start_address, unsigned int end_address,
        unsigned int response_cycle);
    ~SlowMem();

    // Process declarations.
    void WaitThread();

    // DirectIF
    bool DirectRead(int8_t *data, unsigned int address);
    bool DirectWrite(int8_t *data, unsigned int address);

    // SlaveIF
    Status Read(int8_t *data, unsigned int address);
    Status Write(int8_t *data, unsigned int address);
    unsigned int GetStartAddress() const;
    unsigned int GetEndAddress() const;

    // Helper declarations.

  private:
    int8_t *mem_;
    unsigned int start_address_;
    unsigned int end_address_;
    unsigned int response_cycle_;
    int wait_count_;
    sc_event restart_count_;

    // Channel/Submodule* definitions.
};

#endif /* _SIMPLE_SOC_TLM_SLOW_MEM_H_ */
