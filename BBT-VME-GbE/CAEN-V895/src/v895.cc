// -*- C++ -*-

#include "v895.h"

#include "bbt-vme-gbe.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <stdexcept>

std::string host = "";
int port = -999;
//const int port = 24;

namespace caen
{
namespace v895
{
const auto AM = bbt::vmeg::A24;
const auto DM = bbt::vmeg::D16;

// std::stringstream ss;
// void log(const std::string& msg) {
//   static std::ofstream ofs("last.log");
//   std::cout << msg; ofs << msg;
// }

void get_version_serial(unsigned int baseaddr) {
  unsigned short ver;
  unsigned int address = baseaddr + kVersionSerial;
  if (bbt::vmeg::read(AM | DM, address, &ver, 2) < 0) {
    throw std::runtime_error("Error reading version!");
  }
  if (bbt::vmeg::read(AM | DM, address, &ver, 2) < 0) {
    throw std::runtime_error("Error reading version (retry)!");
  }
  std::cout << "Version: " << std::hex << ((ver >> 12) & 0xf) << "\n";
  std::cout << "Serial: " << std::hex << (ver & 0xfff) << "\n";
}

//_____________________________________________________________________________
void get_ipaddress(const std::string& file_path) {
  std::ifstream ifs(file_path);
  if (!ifs.is_open()) {
    throw std::runtime_error("Failed to open the file: " + file_path);
  }
  std::string line;
  std::string ip = "";
  int pt = -999;
  while (std::getline(ifs, line)){
    std::istringstream iss(line);
    std::string key;
    iss >> key;
    if(key == "IP"){
      iss >> ip;
    } else if(key == "PORT"){
      iss >> pt;
    }
  }
  ifs.close();
  if(!ip.empty()){
    host = ip;
    std::cout << " IP: " << host << std::endl;
  } else {
    throw std::runtime_error("Failed to read ip address in the file: " + file_path);
  }
  if(!(pt<0)){
    port = pt;
    std::cout << " port: " << port << std::endl;
  } else {
    throw std::runtime_error("Failed to read port in the file: " + file_path);
  }

}

//_____________________________________________________________________________
void set_thresholds(const std::string& file_path) {
  std::ifstream ifs(file_path);
  if (!ifs.is_open()) {
    throw std::runtime_error("Failed to open the file: " + file_path);
  }

  std::string line;
  unsigned int baseaddr = 0;
  std::vector<int> thresholds(n_ch);
  std::vector<int> enables(n_ch);

  while (std::getline(ifs, line)) {
    std::istringstream iss(line);
    if (line.find("VME") == 0) {
      std::string token;
      iss >> token >> std::hex >> baseaddr;
      std::cout << "VME base address: "
                << std::hex << baseaddr << std::endl;
      // get_version_serial(baseaddr);
    } else if (line == "END") {
      for (int i = 0; i < n_ch; ++i) {
        unsigned int address = baseaddr + (i * 2);
        if (enables[i]) {
          if (bbt::vmeg::write(AM | DM, address, &thresholds[i],
                        sizeof(thresholds[i])) < 0) {
            throw std::runtime_error("Failed to set threshold for channel "
                                     + std::to_string(i));
          }
          std::cout << "Set threshold for channel "
                    << std::dec << std::setw(2) << i << ": "
                    << std::setw(3) << thresholds[i] << " mV, Enabled\n";
        } else {
          std::cout << "Channel " << i << " disabled\n";
        }
      }
    } else if (std::isdigit(line[0])) {
      int channel, threshold, enable;
      iss >> channel >> threshold >> enable;
      if (channel >= 0 && channel < n_ch) {
        thresholds[channel] = threshold;
        enables[channel] = enable;
      }
    }
  }
}
} // v895
} // caen

//_____________________________________________________________________________
int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "v895 [param_file]" << std::endl;
    return 1;
  }

  const std::string file_path = argv[1];

  auto now = std::chrono::system_clock::now();
  auto now_time = std::chrono::system_clock::to_time_t(now);
  std::cout << std::ctime(&now_time);

  try {
    caen::v895::get_ipaddress(file_path);
    bbt::vmeg::open(host, port);
    caen::v895::set_thresholds(file_path);
    bbt::vmeg::close();
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
