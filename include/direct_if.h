#ifndef _SIMPLE_SOC_TLM_DIRECT_IF_H_
#define _SIMPLE_SOC_TLM_DIRECT_IF_H_

#include <systemc>

using namespace sc_core;

class DirectIF : public sc_interface {
  public:
    virtual bool DirectRead(int8_t *data, unsigned int address) = 0;
    virtual bool DirectWrite(int8_t *data, unsigned int address) = 0;
};

#endif /* _SIMPLE_SOC_TLM_DIRECT_IF_H_ */
