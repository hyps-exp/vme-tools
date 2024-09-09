// test utility for VME-GbE
// Author: Shuhei Ajimura, RCNP, Osaka University

#include "vmeg_lib.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <stdexcept>

const unsigned int BASEADDR = 0xdd000000;
const unsigned int ADDRVER = 0x000000fe;

void set_thresholds_from_file(int sock, const std::string& file_path) {
  std::ifstream inputFile(file_path);
  if (!inputFile.is_open()) {
    throw std::runtime_error("Failed to open the file: " + file_path);
  }

  std::string line;
  unsigned int vmeBaseAddress = 0;
  std::vector<int> thresholds(16);
  std::vector<int> enables(16);

  while (std::getline(inputFile, line)) {
    std::istringstream iss(line);

    if (line.find("VME") == 0) {
      std::string token;
      iss >> token >> std::hex >> vmeBaseAddress;
      std::cout << "VME base address: " << std::hex << vmeBaseAddress << std::endl;
    } else if (line == "END") {
      // Set thresholds and enable/disable flags for each channel on the current board
      for (int i = 0; i < 16; ++i) {
        unsigned int address = vmeBaseAddress + (i * 2);  // Assume address offset for each channel
        if (enables[i]) {
          if (VMEGwrite(sock, A24 | D16, address, &thresholds[i], sizeof(thresholds[i])) < 0) {
            throw std::runtime_error("Failed to set threshold for channel " + std::to_string(i));
          }
          std::cout << "Set threshold for channel " << i << ": " << thresholds[i] << " mV, Enabled\n";
        } else {
          std::cout << "Channel " << i << " disabled.\n";
        }
      }
    } else if (std::isdigit(line[0])) {
      int channel, threshold, enable;
      iss >> channel >> threshold >> enable;

      if (channel >= 0 && channel < 16) {
        thresholds[channel] = threshold;
        enables[channel] = enable;
      }
    }
  }

  inputFile.close();
}


int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "v895 [param_file]" << std::endl;
    return 1;
  }

  const std::string host = "172.25.27.163"; // argv[1];
  const int port = 24; // std::stoi(argv[2]);
  const std::string file_path = argv[1];

  try {
    int sock = VMEGopen(host, port);
    unsigned short ver;
    unsigned int address = BASEADDR + ADDRVER;

    // Get current status
    if (VMEGread(sock, A24 | D16, address, &ver, 2) < 0) {
      throw std::runtime_error("Error reading version!");
    }

    // Time tracking with chrono
    auto start = std::chrono::high_resolution_clock::now();

    // Wait and re-read
    if (VMEGread(sock, A24 | D16, address, &ver, 2) < 0) {
      throw std::runtime_error("Error reading version (retry)!");
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Print version information
    std::cout << "Version: " << std::hex << ((ver >> 12) & 0xf) << "\n";
    std::cout << "Serial: " << std::hex << (ver & 0xfff) << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    set_thresholds_from_file(sock, file_path);

    VMEGclose(sock);

  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
