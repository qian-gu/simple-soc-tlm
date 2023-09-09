#ifndef _SIMPLE_SOC_TLM_TYPES_H_
#define _SIMPLE_SOC_TLM_TYPES_H_

#include <cstdint>
#include <systemc>

using namespace sc_core;

enum Status {
  TRANSFER_OK = 0,
  TRANSFER_REQUEST,
  TRANSFER_WAIT,
  TRANSFER_ERROR
};

enum Operation {
  READ = 0,
  WRITE
};

enum Lock {
  LOCK_NO = 0,
  LOCK_SET,
  LOCK_GRANTED
};

struct Request {
  public:
  Operation operation;
  int8_t *data;
  unsigned int start_address;
  unsigned int length;
  Lock lock;
  Status status;
  sc_event transfer_done;
};

//////////////////////////////
// SoC System configuration //
//////////////////////////////

// memory map
// Fast Memory(4KB)
static const unsigned int kFastMemStartAddress = 0x2000;
static const unsigned int kFastMemEndAddress = 0x3FFF;
// Slow Memory(4KB)
static const unsigned int kSlowMemStartAddress = 0x4000;
static const unsigned int kSlowMemEndAddress = 0x5FFF;
// UART(12B)
static const unsigned int kUARTStartAddress = 0x1000;
static const unsigned int kUARTEndAddress = 0x1002;
static const unsigned int kUARTStatusPort = 0x1000;  // offset 0x0
static const unsigned int kUARTConfigurePort = 0x1001;  // offset 0x1
static const unsigned int kUARTDataPort = 0x1002;  // offset 0x2

// Slow memory response time
static const unsigned int kSlowMemResponseCycle = 100;

static const unsigned int kPriorityNum = 2;  // only 2 master need priority
static const unsigned int kMasterBlockingPriority = 0;
static const unsigned int kMasterNonBlockingPriority = 1;

//////////////////////////////
// Simulation configuration //
//////////////////////////////

static const double kClockFrequencyGHz = 1.0;

// generated const
static const double kClockPeriodNS = 1/kClockFrequencyGHz;

#endif /* _SIMPLE_SOC_TLM_TYPES_H_ */
