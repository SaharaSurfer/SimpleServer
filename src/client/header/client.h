#ifndef SIMPLESERVER_HEADER_CLIENT_H_
#define SIMPLESERVER_HEADER_CLIENT_H_

#include <string>
#include <utility>
#include <boost/asio.hpp>

class Client {
 private:
  boost::asio::io_context io_context_;
  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf input_buffer_;

  void send_request(const std::string& request);
  void read_reply();

 public:
  Client() : socket_(io_context_) {}

  void connect_to_server(const std::string& host, const std::string& port);
};

#endif // SIMPLESERVER_HEADER_CLIENT_H_
