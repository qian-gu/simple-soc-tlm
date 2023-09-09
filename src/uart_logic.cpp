#include "uart_logic.h"
#include "types.h"
#include <vcml.h>

UARTLogic::UARTLogic(sc_module_name name, unsigned int start_address, unsigned int end_address) :
  sc_module(name),
  start_address_(start_address),
  end_address_(end_address),
  status_(0),
  configure_(0),
  rx_valid("rx_valid"),
  rx_data("rx_data"),
  tx_valid("tx_valid"),
  tx_data("tx_data") {
  sc_assert(start_address_ <= end_address_);
  SC_THREAD(ReceiveThread);
  SC_THREAD(TransmitThread);
}

// XXX: sensitive to sc_in<bool> rx_valid
/// @brief Receive thread of UARTLogic.
inline void UARTLogic::ReceiveThread() {
  int8_t received_data = 0;
  unsigned shift_count = 0;
  while (true) {
    wait(1*kClockPeriodNS, SC_NS);
    if (rx_valid) {
      if (shift_count == 9) {  // Write received data into rxfifo after assemble.
        rxfifo_write_port-> write(received_data);
        shift_count = 0;
      } else {
        received_data = (received_data << 1) + rx_data;
        shift_count++;
      }
    }
  }
}

// XXX: sensitive to sc_in<bool> rx_valid
/// @brief Transmit thread of UARTLogic.
inline void UARTLogic::TransmitThread() {
  int8_t transmit_data = 0;
  bool transmit_busy = 0;
  bool transmit_last = 0;
  while (true) {
    wait(1*kClockPeriodNS, SC_NS);
    if (transmit_busy) { // Transmit current data.
      // TODO: transmit
      if (transmit_last) transmit_busy = 0;  // Reset transmit status when finish.
    } else { // Pop data from txfifo for a new transmition.
      txfifo_read_port->read(transmit_data);
      transmit_busy = 1;
    }
  }
}

/// @brief DirectIF read function implemented by UARTLogic.
///
/// @param data Readed data.
/// @param address Read address.
///
/// @return Status of read operation.
inline bool UARTLogic::DirectRead(int8_t *data, unsigned int address) {
  return (Read(data, address) == TRANSFER_OK);
}

/// @brief DirectIF write function implemented by UARTLogic.
///
/// @param data Write data.
/// @param address Write address.
///
/// @return Status of write operation.
inline bool UARTLogic::DirectWrite(int8_t *data, unsigned int address) {
  return (Write(data, address) == TRANSFER_OK);
}

/// @brief SlaveIF read function implemented by UARTLogic.
///
/// @param data Readed data.
/// @param address Read address.
///
/// @return Status of read operation.
inline Status UARTLogic::Read(int8_t *data, unsigned int address) {
  if ((address < start_address_) || (address > end_address_)) {
    *data = 0;
    return TRANSFER_ERROR;
  }
  Status status;
  switch (address) {
    case kUARTStatusPort:
      *data = status_;
      vcml::log.info("    |--> [UART] read data: %c from StatusPort(0x%x)", *data, address);
      status = TRANSFER_OK;
      break;
    case kUARTConfigurePort:
      *data = configure_;
      vcml::log.info("    |--> [UART] read data: %c from ConfigurePort(0x%x)", *data, address);
      status = TRANSFER_OK;
      break;
    default:
      int8_t temp;
      if (rxfifo_read_port->nb_read(temp)) {
        *data = temp;
        vcml::log.info("    |--> [UART] read data: %c from DataPort(0x%x)", *data, address);
        status = TRANSFER_OK;
      } else {
        status = TRANSFER_ERROR;
      }
  }
  return status;
}

/// @brief SlaveIF write function implemented by UARTLogic.
///
/// @param data Write data.
/// @param address Write address.
///
/// @return Status of write operation.
inline Status UARTLogic::Write(int8_t *data, unsigned int address) {
  Status status;
  switch (address) {
    case kUARTStatusPort:
      status_ = *data;
      vcml::log.info("    |--> [UART] write data: %c to StatusPort(0x%x)", *data, address);
      status = TRANSFER_OK;
      break;
    case kUARTConfigurePort:
      configure_ = *data;
      vcml::log.info("    |--> [UART] write data: %c to ConfigurePort(0x%x)", *data, address);
      status = TRANSFER_OK;
      break;
    default:
      if (txfifo_write_port->nb_write(*data)) {
        vcml::log.info("    |--> [UART] write data: %c to DataPort(0x%x)", *data, address);
        status = TRANSFER_OK;
      } else {
        status = TRANSFER_ERROR;
      }
  }
  return status;
}

/// @brief Get start address of UART region.
///
/// @return Start address of UART region.
inline unsigned int UARTLogic::GetStartAddress() const {
  return start_address_;
}

/// @brief Get end address of UART region.
///
/// @return End address of UART region.
inline unsigned int UARTLogic::GetEndAddress() const {
  return end_address_;
}
