#include "simple_soc.h"
#include "master_direct.h"
#include "master_blocking.h"
#include "master_non_blocking.h"
#include "bus.h"
#include "arbiter.h"
#include "fast_mem.h"
#include "slow_mem.h"
#include "uart.h"

SimpleSoC::SimpleSoC(sc_module_name name) :
  rx_valid("rx_valid"),
  rx_data("rx_data"),
  tx_valid("tx_valid"),
  tx_data("tx_data") {
  // Allocate submodule and channels.
  master_direct_ = new MasterDirect("master_direct");
  master_blocking_ = new MasterBlocking("master_blocking");
  master_non_blocking_ = new MasterNonBlocking("master_non_blocking");
  bus_ = new Bus("bus");
  arbiter_ = new Arbiter("arbiter");
  fast_mem_ = new FastMem("fast_mem", kFastMemStartAddress, kFastMemEndAddress);
  slow_mem_ = new SlowMem("slow_mem", kSlowMemStartAddress, kSlowMemEndAddress,
      kSlowMemResponseCycle);
  uart_ = new UART("uart", kUARTStartAddress, kUARTEndAddress);
  // Connect up submodules and channels.
  master_direct_->direct_port(*bus_);
  master_blocking_->blocking_port(*bus_);
  master_non_blocking_->non_blocking_port(*bus_);
  bus_->arbiter_port(*arbiter_);
  bus_->slave_port(*fast_mem_);
  bus_->slave_port(*slow_mem_);
  bus_->slave_port(*uart_);
  uart_->rx_valid(rx_valid);
  uart_->rx_data(rx_data);
  uart_->tx_valid(tx_valid);
  uart_->tx_data(tx_data);
}

SimpleSoC::~SimpleSoC() {
  if (master_direct_) delete master_direct_;
  master_direct_ = (MasterDirect*)0;
  if (master_blocking_) delete master_blocking_;
  master_blocking_ = (MasterBlocking*)0;
  if (master_non_blocking_) delete master_non_blocking_;
  master_non_blocking_ = (MasterNonBlocking*)0;
  if (bus_) delete bus_;
  bus_ = (Bus*)0;
  if (fast_mem_) delete fast_mem_;
  fast_mem_ = (FastMem*)0;
  if (slow_mem_) delete slow_mem_;
  slow_mem_ = (SlowMem*)0;
  if (uart_) delete uart_;
  uart_ = (UART*)0;
}
