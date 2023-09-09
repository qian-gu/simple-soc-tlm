#ifndef _SIMPLE_SOC_TLM_SIMPLE_SOC_H_
#define _SIMPLE_SOC_TLM_SIMPLE_SOC_H_

#include <systemc>

using namespace sc_core;

class MasterDirect;
class MasterBlocking;
class MasterNonBlocking;
class Bus;
class Arbiter;
class FastMem;
class SlowMem;
class UART;

class SimpleSoC : public sc_module {
  public:

    SimpleSoC(sc_module_name name);
    ~SimpleSoC();

  private:
    MasterDirect *master_direct_;
    MasterBlocking *master_blocking_;
    MasterNonBlocking *master_non_blocking_;
    Bus *bus_;
    Arbiter *arbiter_;
    FastMem *fast_mem_;
    SlowMem *slow_mem_;
    UART *uart_;
    // receive transaction
    sc_signal<bool> rx_valid;
    sc_signal<bool> rx_data;
    // transmit transaction
    sc_signal<bool> tx_valid;
    sc_signal<bool> tx_data;
};

#endif /* _SIMPLE_SOC_TLM_SIMPLE_SOC_H_ */

