#ifndef _SIMPLE_SOC_TLM_UART_LOGIC_H_
#define _SIMPLE_SOC_TLM_UART_LOGIC_H_

#include "slave_if.h"
#include <systemc>
#include <cstdint>

using namespace sc_core;

// Submodule forward declarations.

class UARTLogic : public SlaveIF, public sc_channel {
  public:
    // Port declarations.
    sc_in<bool> rx_valid;  // receive transaction
    sc_in<bool> rx_data;
    sc_port<sc_fifo_in_if<int8_t> > rxfifo_read_port;
    sc_port<sc_fifo_out_if<int8_t> > rxfifo_write_port;
    sc_out<bool> tx_valid;  // transmit transaction
    sc_out<bool> tx_data;
    sc_port<sc_fifo_in_if<int8_t> > txfifo_read_port;
    sc_port<sc_fifo_out_if<int8_t> > txfifo_write_port;

    SC_HAS_PROCESS(UARTLogic);

    // Constructor/destructor declarations.
    UARTLogic(sc_module_name name, unsigned int start_address, unsigned int end_address);

    // Process declarations.
    void ReceiveThread();
    void TransmitThread();

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
    unsigned int start_address_;
    unsigned int end_address_;
    int8_t status_;
    int8_t configure_;

    // Channel/Submodule* definitions.
};

#endif /* _SIMPLE_SOC_TLM_UART_LOGIC_H_ */
