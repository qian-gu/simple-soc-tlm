#ifndef _SIMPLE_SOC_TLM_MASTER_NON_BLOCKING_H_
#define _SIMPLE_SOC_TLM_MASTER_NON_BLOCKING_H_

#include "non_blocking_if.h"
#include <systemc>

using namespace sc_core;

// Submodule forward declarations.

class MasterNonBlocking : public sc_module {
  public:
    // Port declarations.
    sc_port<NonBlockingIF> non_blocking_port;

    SC_HAS_PROCESS(MasterNonBlocking);

    // Constructor/destructor declarations.
    MasterNonBlocking(sc_module_name name);

    // Process declarations.
    void ReadThread();
    void WriteThread();

    // Helper declarations.

  private:
    static constexpr const char sentence[] = "Hello, I am MasterNonBlocking.";

    // Channel/Submodule *declarations.
};

#endif /* _SIMPLE_SOC_TLM_MASTER_NON_BLOCKING_H_ */
