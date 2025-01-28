// Rei Kacani, 12219813
#include "gate.h"
#include "type.h"
#include "connection.h"
#include <string>

int main (int argc, char *argv[])
{
  if (argc < 6)
  { cerr << "Error, arguments invalid or wrong.\n";
    exit (1);
  }
  ConnectionList cl(argv[2]);

  GateList gl(argv[1]);

  TypeList tl(argv[3]);

  cl.gates_and_delay(gl, tl, cl, stoi(argv[5]), stoi(argv[6]));
}
