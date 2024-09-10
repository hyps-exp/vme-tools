#include "v895.h"
#include "vmeg_lib.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <stdexcept>

const std::string host = "172.25.27.163";
const int port = 24;

namespace caen
{
namespace v895
{
void get_version_serial(int sock, unsigned int baseaddr) {
  unsigned short ver;
  unsigned int address = baseaddr + kVersionSerial;
  if (VMEGread(sock, A24 | D16, address, &ver, 2) < 0) {
    throw std::runtime_error("Error reading version!");
  }

  // auto start = std::chrono::high_resolution_clock::now();

  if (VMEGread(sock, A24 | D16, address, &ver, 2) < 0) {
    throw std::runtime_error("Error reading version (retry)!");
  }

  // auto end = std::chrono::high_resolution_clock::now();
  // std::chrono::duration<double> elapsed = end - start;

  std::cout << "Version: " << std::hex << ((ver >> 12) & 0xf) << "\n";
  std::cout << "Serial: " << std::hex << (ver & 0xfff) << "\n";
  // std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

//_____________________________________________________________________________
void set_thresholds(int sock, const std::string& file_path) {
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
      // get_version_serial(sock, baseaddr);
    } else if (line == "END") {
      for (int i = 0; i < n_ch; ++i) {
        unsigned int address = baseaddr + (i * 2);
        if (enables[i]) {
          if (VMEGwrite(sock, A24 | D16, address, &thresholds[i],
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

  try {
    int sock = VMEGopen(host, port);
    caen::v895::set_thresholds(sock, file_path);
    VMEGclose(sock);
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
