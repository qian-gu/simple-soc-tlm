#ifndef _SIMPLE_SOC_TLM_UART_H_
#define _SIMPLE_SOC_TLM_UART_H_

#include "slave_if.h"
#include <systemc>
#include <cstdint>

using namespace sc_core;

// Submodule forward declarations.
class UARTLogic;

class UART : public SlaveIF, public sc_channel {
  public:
    // Port declarations.
    sc_in<bool> rx_valid;  // receive transaction
    sc_in<bool> rx_data;
    sc_out<bool> tx_valid;  // transmit transaction
    sc_out<bool> tx_data;

    // Constructor/destructor declarations.
    UART(sc_module_name name, unsigned int start_address, unsigned int end_address);
    ~UART();

    // DirectIF
    bool DirectRead(int8_t *data, unsigned int address);
    bool DirectWrite(int8_t *data, unsigned int address);

    // SlaveIF
    Status Read(int8_t *data, unsigned int address);
    Status Write(int8_t *data, unsigned int address);
    unsigned int GetStartAddress() const;
    unsigned int GetEndAddress() const;

    // Helper declarations.

  private:
    // Channel/Submodule* definitions.
    UARTLogic *uart_logic;
    sc_fifo<int8_t> *rxfifo;
    sc_fifo<int8_t> *txfifo;
};

#endif /* _SIMPLE_SOC_TLM_UART_H_ */
