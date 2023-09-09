#ifndef _SIMPLE_SOC_TLM_NON_BLOCKING_IF_H_
#define _SIMPLE_SOC_TLM_NON_BLOCKING_IF_H_

#include "types.h"
#include <systemc>

using namespace sc_core;

class NonBlockingIF : public sc_interface {
  public:
    virtual void Read(unsigned int unique_priority, int8_t *data, unsigned int start_address,
        bool lock = false) = 0;

    virtual void Write(unsigned int unique_priority, int8_t *data, unsigned int start_address,
        bool lock = false) = 0;

    virtual Status GetStatus(unsigned int unique_priority) = 0;
};

#endif /* _SIMPLE_SOC_TLM_NON_BLOCKING_IF_H_ */
