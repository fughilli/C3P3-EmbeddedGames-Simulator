#include <stdint.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "control_message.pb.h"
#include "utils/udp_server.h"

int main(void) {
  utils::UdpServer server{5533};
  if (!server.Start()) {
    std::cerr << "Failed to start server" << std::endl;
    return 1;
  }

  std::vector<uint8_t> buffer;
  while (true) {
    if (server.Read(&buffer)) {
      std::cout << "Received buffer of " << buffer.size() << " bytes"
                << std::endl;
      Control control;
      control.ParseFromString(std::string(buffer.begin(), buffer.end()));

      std::cout << control.ShortDebugString() << std::endl;
    }

    std::this_thread::yield();
  }

  return 0;
}
