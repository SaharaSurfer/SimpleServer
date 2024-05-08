#include "header/client.h"

#include <iostream>
#include <string>
#include <utility>
#include <boost/asio.hpp>

void Client::connect_to_server(const std::string& host, const std::string& port) {
  boost::asio::ip::tcp::resolver resolver(io_context_);
  boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);
  connect(socket_, endpoints);
}
