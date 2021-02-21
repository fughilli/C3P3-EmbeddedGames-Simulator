#ifndef UTILS_UDP_SERVER_H_
#define UTILS_UDP_SERVER_H_

#include <stdint.h>

#include <vector>

namespace utils {

class UdpServer {
 public:
  UdpServer(int port) : port_(port), receive_buffer_(65536) {}

  bool Start();

  bool Read(std::vector<uint8_t>* buffer);

 private:
  int port_;
  int socket_fd_;

  std::vector<uint8_t> receive_buffer_;
};

}  // namespace utils

#endif  // UTILS_UDP_SERVER_H_
