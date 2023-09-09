#include "uart.h"
#include "uart_logic.h"

UART::UART(sc_module_name name, unsigned int start_address, unsigned int end_address) :
  sc_module(name),
  rx_valid("rx_valid"),
  rx_data("rx_data"),
  tx_valid("tx_valid"),
  tx_data("tx_data") {
    uart_logic = new UARTLogic("uart_logic", start_address, end_address);
    rxfifo = new sc_fifo<int8_t>(100);  // UART local rx FIFO depth is 100
    txfifo = new sc_fifo<int8_t>(100);  // UART local tx FIFO depth is 100
    // Rx connecting up.
    uart_logic->rx_valid(rx_valid);
    uart_logic->rx_data(rx_data);
    uart_logic->rxfifo_read_port(*rxfifo);
    uart_logic->rxfifo_write_port(*rxfifo);
    // Tx connecting up.
    uart_logic->tx_valid(tx_valid);
    uart_logic->tx_data(tx_data);
    uart_logic->txfifo_read_port(*txfifo);
    uart_logic->txfifo_write_port(*txfifo);
}

UART::~UART() {
  if (uart_logic) {
    delete uart_logic;
    uart_logic = (UARTLogic*)0;
  }
  if (rxfifo) {
    delete rxfifo;
    rxfifo = (sc_fifo<int8_t>*)0;
  }
  if (txfifo) {
    delete txfifo;
    txfifo = (sc_fifo<int8_t>*)0;
  }
}

inline bool UART::DirectRead(int8_t *data, unsigned int address) {
  return uart_logic->DirectRead(data, address);
}

inline bool UART::DirectWrite(int8_t *data, unsigned int address) {
  return uart_logic->DirectWrite(data, address);
}

inline Status UART::Read(int8_t *data, unsigned int address) {
  return uart_logic->Read(data, address);
}

inline Status UART::Write(int8_t *data, unsigned int address) {
  return uart_logic->Write(data, address);
}

inline unsigned int UART::GetStartAddress() const {
  return uart_logic->GetStartAddress();
}

inline unsigned int UART::GetEndAddress() const {
  return uart_logic->GetEndAddress();
}
