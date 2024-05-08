#include "header/client.h"

#include <iostream>
#include <string>
#include <utility>
#include <boost/asio.hpp>

void Client::send_request(const std::string& request) {
  boost::system::error_code ec;
  boost::asio::write(socket_, boost::asio::buffer(request + "\r\n"), ec);

  if (ec) {
    std::cerr << "Error sending request: " << ec.message() << std::endl;
    return;
  }
}

void Client::connect_to_server(const std::string& host, const std::string& port) {
  boost::asio::ip::tcp::resolver resolver(io_context_);
  boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);
  connect(socket_, endpoints);
}
