#include "header/client.h"

#include <iostream>
#include <string>
#include <utility>
#include <boost/asio.hpp>

void Client::send_request(const std::string& request) {
  try {
    boost::asio::write(socket_, boost::asio::buffer(request + "\r\n"));
  } catch (const boost::system::system_error& e) {
    std::cerr << "Error sending request: " << e.what() << std::endl;
  }
}

void Client::read_reply() {
  try {
    // Clear input buffer
    input_buffer_.consume(input_buffer_.size());

    boost::asio::read_until(socket_, input_buffer_, "\r\n\r\n");

    std::istream is(&input_buffer_);
    std::string response;
    std::getline(is, response, '\r');

    // Ignore remaining '\n'
    if (is.peek() == '\n') {
      is.get();
    }

    std::cout << "Received response:\n" << response << std::endl;

  } catch (const boost::system::system_error& e) {
    if (e.code() == boost::asio::error::eof) {
      std::cerr << "Server disconnected." << std::endl;
      socket_.close();
    } else {
      std::cerr << "Error reading response: " << e.what() << std::endl;
    }
  }
}

void Client::connect_to_server(const std::string& host, const std::string& port) {
  try {
    boost::asio::ip::tcp::resolver resolver(io_context_);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);
    
    boost::asio::connect(socket_, endpoints);
    std::cout << "Connected to the server successfully." << std::endl;

  } catch (const std::exception& e) {
    std::cerr << "Error connecting to server: " << e.what() << std::endl;
    return;
  }
}

void Client::run() {
  if (!socket_.is_open()) {
    std::cerr << "Cannot run client, no connection established." << std::endl;
    return;
  }

  // Read welcome message from server
  read_reply();

  // Main loop
  while (socket_.is_open()) {
    std::cout << "Enter your request: ";

    // Read customer input
    std::string request;
    std::getline(std::cin, request);

    if (request.empty()) {
      std::cerr << "Empty request, please enter a valid command." << std::endl;
      continue;
    }

    send_request(request);
    read_reply();
  }
}
