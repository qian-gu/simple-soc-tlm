#ifndef _SIMPLE_SOC_TLM_MASTER_DIRECT_H_
#define _SIMPLE_SOC_TLM_MASTER_DIRECT_H_

#include "direct_if.h"
#include <systemc>

using  namespace sc_core;

// Submodule forward declarations.

class MasterDirect : public sc_module {
  public:
    // Port declarations.
    sc_port<DirectIF> direct_port;

    SC_HAS_PROCESS(MasterDirect);

    // Constructor/destructor declarations.
    MasterDirect(sc_module_name name);

    // Process declarations.
    void ReadThread();
    void WriteThread();

    // Helper declarations.

  private:
    static constexpr const char sentence[] = "Hello, I am MasterDirect.";

    // Channel/Submodule *declarations.
};

#endif /* _SIMPLE_SOC_TLM_MASTER_DIRECT_H_ */
