#ifndef _SIMPLE_SOC_TLM_ARBITER_H_
#define _SIMPLE_SOC_TLM_ARBITER_H_

#include "arbiter_if.h"
#include "types.h"

class Arbiter : public ArbiterIF, public sc_prim_channel {
  public:
    // Constructor/destructor declarations.
    Arbiter(sc_module_name name) : sc_prim_channel(name) {};

    Request *Arbitrate(Request requests[]);
};

#endif /* _SIMPLE_SOC_TLM_ARBITER_H_ */
