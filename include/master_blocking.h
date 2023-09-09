#ifndef _SIMPLE_SOC_TLM_MASTER_BLOCKING_H_
#define _SIMPLE_SOC_TLM_MASTER_BLOCKING_H_

#include "blocking_if.h"
#include <systemc>

using namespace sc_core;

// Submodule forward declarations.

class MasterBlocking : public sc_module {
  public:
    // Port declarations.
    sc_port<BlockingIF> blocking_port;

    SC_HAS_PROCESS(MasterBlocking);

    // Constructor/destructor declarations.
    MasterBlocking(sc_module_name name);

    // Process declarations
    void ReadThread();
    void WriteThread();

    // Helper declarations.

  private:
    static constexpr const char *sentence = "Hello, I am MasterBlocking.";

    // Channel/Submodule *declarations.
};

#endif /* _SIMPLE_SOC_TLM_MASTER_BLOCKING_H_ */
