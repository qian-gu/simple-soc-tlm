#include "simple_soc.h"
#include <systemc>

using namespace sc_core;

int sc_main(int argc, char *argv[])
{
  SimpleSoC simple_soc("simple_soc");
  sc_start(100, SC_NS);
  return 0;
}
