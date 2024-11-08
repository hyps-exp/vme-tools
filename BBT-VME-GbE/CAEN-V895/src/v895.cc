// -*- C++ -*-

#include "v895.h"

#include "bbt-vme-gbe.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <stdexcept>

#include <curl/curl.h>

std::string host = "";
int port = -999;
//const int port = 24;
unsigned int width = 200;

namespace
{
std::string line_protocol;

void add_line_protocol(const std::string& line) {
  line_protocol += line;
}

void send_to_influxdb(const std::string& buf) {
  const std::string python_script = "python /home/sks/software/vme-tools/python/send_to_influxdb.py";
  std::ostringstream command;
  command << python_script << " \"" << buf << "\"";
  // std::cout << "Executing command: " << command.str() << std::endl;
  int result = std::system(command.str().c_str());
  if (result != 0) {
    std::cerr << "Error: Python script failed with exit code " << result << std::endl;
  }
}
}

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

  std::ofstream ofs("last-param.txt");
  ofs << "# " << file_path << std::endl << std::endl;

  std::string line;
  unsigned int baseaddr = 0;
  std::vector<int> thresholds(n_ch);
  std::vector<int> enables(n_ch);

  while (std::getline(ifs, line)) {
    ofs << line << std::endl;
    std::istringstream iss(line);
    if (line.find("VME") == 0) {
      std::string token;
      iss >> token >> std::hex >> baseaddr;
      std::cout << "VME base address: "
                << std::hex << baseaddr << std::endl;
      // get_version_serial(baseaddr);
    } else if (line == "END") {
      unsigned int channelmask = 0;
      for (int i = 0; i < n_ch; ++i) {
        unsigned int address = baseaddr + (i * 2);
	std::stringstream line_protocol_ss;
	line_protocol_ss << "v895,vme_address=" << std::hex << baseaddr
			 << ",channel=" << std::dec << std::setw(2) << std::setfill('0') << i
			 << " enable=" << enables[i] << ",threshold=" << thresholds[i]
			 << ",width=" << width << std::endl;
	add_line_protocol(line_protocol_ss.str());
        if (enables[i]) {
	  channelmask |= (0x1 << i);

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
      std::cout << "channel mask  " << std::hex << channelmask << std::endl;
      if (bbt::vmeg::write(AM | DM, baseaddr+0x4a, &channelmask,
			   sizeof(channelmask)) < 0) {
	throw std::runtime_error("Failed to set channel mask");
      }

      unsigned int address_width = baseaddr + 0x40;
      if (bbt::vmeg::write(AM | DM, address_width, &width,
			   sizeof(width)) < 0) {
	throw std::runtime_error("Failed to set width for channel 0-7");
      }
      address_width = baseaddr + 0x42;
      if (bbt::vmeg::write(AM | DM, address_width, &width,
			   sizeof(width)) < 0) {
	throw std::runtime_error("Failed to set width for channel 8-15");
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

  send_to_influxdb(line_protocol);
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
