// -*- C++ -*-

#ifndef BBT_VME_GbE_H
#define BBT_VME_GbE_H

#include <string>

namespace bbt
{
namespace vmeg
{
const int MAXBUFSIZ = 256;
const int HEADER_SIZE = 12;

enum AddressMode {
  A16 = 0x0000,
  A24 = 0x0100,
  A32 = 0x0200
};

enum DataMode {
  D8  = 0x0000,
  D16 = 0x0400,
  D32 = 0x0800
};

enum TransferMode {
  BLT = 0x0020,
  FIX = 0x0080
};

void close();
int open(const std::string& host, int port);
int read(unsigned short mode, unsigned int address,
         void *data, int bytes);
int write(unsigned short mode, unsigned int address,
          void *data, int bytes);

} // namespace vmeg
} // namespace bbt

#endif // BBT_VME_GbE_H
