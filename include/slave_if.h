#ifndef _SIMPLE_SOC_TLM_SLAVE_IF_H_
#define _SIMPLE_SOC_TLM_SLAVE_IF_H_

#include "direct_if.h"
#include "types.h"

class SlaveIF : public DirectIF {
  public:
    virtual Status Read(int8_t *data, unsigned int address) = 0;
    virtual Status Write(int8_t *data, unsigned int address) = 0;

    virtual unsigned int GetStartAddress() const = 0;
    virtual unsigned int GetEndAddress() const = 0;
};

#endif /* _SIMPLE_SOC_TLM_SLAVE_IF_H_ */
