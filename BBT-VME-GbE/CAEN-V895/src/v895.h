// -*- C++ -*-

#ifndef CAEN_V895_H
#define CAEN_V895_H

#include <string>

namespace caen
{
namespace v895
{
const int n_ch = 16;

enum EAddressOffset {
  kVersionSerial = 0xfe,
};

void get_version_serial(int sock, unsigned int baseaddr);
void set_thresholds(int sock, const std::string& file_path);
}
}


#endif
