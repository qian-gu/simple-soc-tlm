#ifndef _SIMPLE_SOC_TLM_BUS_H_
#define _SIMPLE_SOC_TLM_BUS_H_

#include "direct_if.h"
#include "slave_if.h"
#include "blocking_if.h"
#include "non_blocking_if.h"
#include "arbiter_if.h"
#include "types.h"
#include <systemc>

using namespace sc_core;

// Submodule forward declarations.

class Bus : public DirectIF, public NonBlockingIF, public BlockingIF, public sc_channel {
  public:
    // Port declarations.
    sc_port<ArbiterIF> arbiter_port;
    sc_port<SlaveIF, 0> slave_port;

    SC_HAS_PROCESS(Bus);

    // Constructor/destructor declarations.
    Bus(sc_module_name name);

    // Process declarations.
    void MainThread();

    // DirectIF
    bool DirectRead(int8_t *data, unsigned int address);
    bool DirectWrite(int8_t *data, unsigned int address);

    // BlockingIF
    Status BurstRead(unsigned int unique_priority,
        int8_t *data,
        unsigned int start_address,
        unsigned int length = 1,
        bool lock = false);

    Status BurstWrite(unsigned int unique_priority,
        int8_t *data,
        unsigned int start_address,
        unsigned int length = 1,
        bool lock = false);

    // NonBlockingIF
    void Read(unsigned int unique_priority,
        int8_t *data,
        unsigned int start_address,
        bool lock = false);

    void Write(unsigned int unique_priority,
        int8_t *data,
        unsigned int start_address,
        bool lock = false);

    Status GetStatus(unsigned int unique_priority);

  private:
    // Helper declarations.
    SlaveIF *GetSlave(unsigned int address);

    Status BurstOperation(unsigned int unique_priority,
        int8_t *data,
        unsigned int start_address,
        unsigned int length=1,
        bool lock = false,
        Operation operation = READ);

    void NonBlockingOperation(unsigned int unique_priority,
        int8_t *data,
        unsigned int start_address,
        bool lock = false,
        Operation operation = READ);

    Request* GetNextRequest();

    void HandleRequest();

  private:
    Request requests_[kPriorityNum];
    Request *current_request_;

    // Channel/Submodule* definitions.
};

#endif /* _SIMPLE_SOC_TLM_BUS_H_ */
