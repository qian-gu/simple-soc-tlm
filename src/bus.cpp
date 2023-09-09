#include "bus.h"
#include <vcml.h>

Bus::Bus(sc_module_name name) :
  sc_module(name),
  arbiter_port("arbiter_port"),
  slave_port("slave_port") {
  for (unsigned int i = 0; i < kPriorityNum; i++) {
    requests_[i].status = TRANSFER_OK;
    requests_[i].lock = LOCK_NO;
  }
  current_request_ = (Request *)NULL;
  SC_THREAD(MainThread);
}

/// @brief Main thread of Bus to arbite and handle requests.
void Bus::MainThread() {
  while (true) {
    wait(1*kClockPeriodNS, SC_NS);
    // Get next request to handle.
    current_request_ = GetNextRequest();
    // Handle the selected request if it 's a pending request.
    if (current_request_->status == TRANSFER_REQUEST) {
      HandleRequest();
    }
  }
}

/**
 * @brief Interface function: DirectIF read immediately without arbitration.
 *
 * @param data Read request data
 * @param address Read request address
 *
 * @return Status of read request, successed or failed.
 */
bool Bus::DirectRead(int8_t *data, unsigned int address) {
  SlaveIF *slave = GetSlave(address);
  if (!slave) return false;
  return slave->DirectRead(data, address);
}

/**
 * @brief Interface function: DirectIF write immediately without arbitration.
 *
 * @param data Write request data
 * @param address Write request address
 *
 * @return Status of write request, successed or failed.
 */
bool Bus::DirectWrite(int8_t *data, unsigned int address) {
  SlaveIF *slave = GetSlave(address);
  if (!slave) return false;
  return slave->DirectWrite(data, address);
}

/// @brief Interface function: BlockingIF read return unitl current request is transferred
///        to target address.
///
/// @param unique_priority Priority of master sending current request
/// @param data Read request data
/// @param start_address Read request start address
/// @param length Read request length
/// @param lock Read request lock
///
/// @return Status of current request
Status Bus::BurstRead(unsigned int unique_priority,
        int8_t *data,
        unsigned int start_address,
        unsigned int length,
        bool lock) {
  vcml::log.info("[Bus with BlockingIF] write data: %c to address: 0x%x", *data, start_address);
  return BurstOperation(unique_priority, data, start_address, length, lock, READ);
}

/// @brief Interface function: BlockingIF write return unitl current request is transferred to
///        target address.
///
/// @param unique_priority Priority of master sending current request
/// @param data Write request data
/// @param start_address Write request start address
/// @param length Write request length
/// @param lock Write request lock
///
/// @return Status of current request
Status Bus::BurstWrite(unsigned int unique_priority,
    int8_t *data,
    unsigned int start_address,
    unsigned int length,
    bool lock) {
  return BurstOperation(unique_priority, data, start_address, length, lock, WRITE);
}

/// @brief Interface function: BlockingIF request template operation for read and write
///        requests.
///
/// @param unique_priority Priority of master sending current request
/// @param data Request data
/// @param start_address Request start address
/// @param length Request length
/// @param lock Request lock
/// @param operation Request type, read or write
///
/// @return Status of current request
Status Bus::BurstOperation(unsigned int unique_priority,
    int8_t *data,
    unsigned int start_address,
    unsigned int length,
    bool lock,
    Operation operation) {
  Request *request = requests_ + unique_priority;
  // Abort if previous request is not finished.
  sc_assert((request->status == TRANSFER_OK) || (request->status == TRANSFER_ERROR));
  request->operation = operation;
  request->data = data;
  request->start_address = start_address;
  request->length = length;
  if (lock) {
    request->lock = (request->lock == LOCK_SET) ? LOCK_GRANTED : LOCK_SET;
  }
  request->status = TRANSFER_REQUEST;
  // BlockingIF wait for transfer done of current request.
  wait(request->transfer_done);
  return request->status;
}

/**
 * @brief Interface function: NonBlockingIF read return after written into request array.
 *
 * @param unique_priority Priority of master sending current request
 * @param data Read request data
 * @param start_address Read request start address
 * @param lock Read request lock
 */
void Bus::Read(unsigned int unique_priority,
    int8_t *data,
    unsigned int start_address,
    bool lock) {
  NonBlockingOperation(unique_priority, data, start_address, lock, READ);
}

/**
 * @brief Interface function: NonBlockingIF write return after written into request array.
 *
 * @param unique_priority Priority of master sending current request
 * @param data Write request data
 * @param start_address Write request start address
 * @param lock Write request lock
 */
void Bus::Write(unsigned int unique_priority,
    int8_t *data,
    unsigned int start_address,
    bool lock) {
  NonBlockingOperation(unique_priority, data, start_address, lock , WRITE);
}

/**
 * @brief Interface function: Get status of previous request for master with the specific priority.
 *
 * @param unique_priority Priority of master
 *
 * @return Status of the master
 */
Status Bus::GetStatus(unsigned int unique_priority) {
  return requests_[unique_priority].status;
}

/**
 * @brief Helper function: NonBlocking interface operation template for read and write.
 *
 * @param unique_priority Priority of master sending current request
 * @param data Operation data
 * @param start_address Operation start address
 * @param lock Operation lock
 * @param operation Operation type
 */
void Bus::NonBlockingOperation(unsigned int unique_priority,
    int8_t *data,
    unsigned int start_address,
    bool lock,
    Operation operation) {
  Request *request = requests_ + unique_priority;
  // Abort if previous request is not finished.
  sc_assert((request->status == TRANSFER_OK) || (request->status == TRANSFER_ERROR));
  // Lock before recording current new request into request array.
  request->operation = operation;
  request->data = data;
  request->start_address = start_address;
  request->length = 1;  // NonBlockingIF read/write only 1 byte in each request.
  if (lock) {
    request->lock = (request->lock == LOCK_SET) ? LOCK_GRANTED : LOCK_SET;
  }
  request->status = TRANSFER_REQUEST;
  // NonBlockingIF finish immediately after written request.
}

/**
 * @brief Helper function: Get slave according to request address.
 *
 * @param address Request address
 *
 * @return Slave Pointer
 */
SlaveIF *Bus::GetSlave(unsigned int address) {
  for (int i = 0; i < slave_port.size(); i++) {
    SlaveIF *slave = slave_port[i];
    if ((slave->GetStartAddress() <= address) && (address <= slave->GetEndAddress())) {
      return slave;
    }
  }
  return (SlaveIF*)0;
}

inline Request *Bus::GetNextRequest() {
  return arbiter_port->Arbitrate(requests_);
}

void Bus::HandleRequest() {
  SlaveIF *slave = GetSlave(current_request_->start_address);
  if (!slave) {
    current_request_->status = TRANSFER_ERROR;
  } else {
    // Translate request into SlaveIF operation.
    for (unsigned i = 0; i < current_request_->length; i++) {
      if (current_request_->operation == READ) {
        vcml::log.info("[Bus HandleRequest] read from 0x%x",
            current_request_->start_address+i);
        current_request_->status = slave->Read(current_request_->data+i,
            current_request_->start_address+i);
      } else {
        vcml::log.info("[Bus HandleRequest] write %c to 0x%x",
            *(current_request_->data+i), current_request_->start_address+i);
        current_request_->status = slave->Write(current_request_->data+i,
            current_request_->start_address+i);
      }
      if (current_request_->status == TRANSFER_ERROR) break;
    }
  }
  current_request_->transfer_done.notify(SC_ZERO_TIME);
}
