#ifndef _SIMPLE_SOC_TLM_BLOCKING_IF_H_
#define _SIMPLE_SOC_TLM_BLOCKING_IF_H_

#include "types.h"
#include <systemc>

using namespace sc_core;

class BlockingIF : public sc_interface {
  public:
    virtual Status BurstRead(unsigned int unique_priority, int8_t *data, unsigned int start_address,
        unsigned int length = 1, bool lock = false) = 0;

    virtual Status BurstWrite(unsigned int unique_priority, int8_t *data,
        unsigned int start_address, unsigned int length = 1, bool lock = false) = 0;
};

#endif /* _SIMPLE_SOC_TLM_BLOCKING_IF_H_ */
