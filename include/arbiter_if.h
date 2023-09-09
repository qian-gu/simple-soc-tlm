#ifndef _SIMPLE_SOC_TLM_ARBITER_IF_
#define _SIMPLE_SOC_TLM_ARBITER_IF_

#include "types.h"
#include <vector>
#include <systemc>

using namespace sc_core;

class ArbiterIF : public sc_interface {
  public:
    virtual Request *Arbitrate(Request requests[]) = 0;
};

#endif /* _SIMPLE_SOC_TLM_ARBITER_IF_ */
