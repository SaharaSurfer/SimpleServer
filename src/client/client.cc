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

void Client::read_reply() {
  boost::system::error_code ec;

  // Clear input buffer
  input_buffer_.consume(input_buffer_.size());

  boost::asio::read_until(socket_, input_buffer_, "\r\n\r\n", ec);
    
  if (ec) {
    if (ec == boost::asio::error::eof) {
      std::cerr << "Server disconnected." << std::endl;
      socket_.close();
    } else {
      std::cerr << "Error reading response: " << ec.message() << std::endl;
    }
    return;
  }

  std::istream is(&input_buffer_);
  std::string response;
  std::getline(is, response, '\r');

  std::cout << "Received response:\n" << response << std::endl;
}

void Client::connect_to_server(const std::string& host, const std::string& port) {
  boost::asio::ip::tcp::resolver resolver(io_context_);
  boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);
  connect(socket_, endpoints);
}
